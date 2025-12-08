
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onLoad);

let powerGraph;
let powerData = [];

let moistureGraph;
let moistureData = [];

function initWebSocket(){
    console.log("Trying to open a WebSocket connection...");
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event){
    console.log("Connection Opened");
}

function onClose(event) {
    console.log('Connection Closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event){
    console.log("Received Message From ESP32 Client");
    console.log(event.data);
    var obj = JSON.parse(event.data);

    if (obj.MoistureLevel){
        var moistureLevel = obj.MoistureLevel;
        document.getElementById("MoistureLevelValue").textContent = `Moisture Level: ${moistureLevel}%`;
        drawMoistureGauge(moistureLevel);

        updateMoistureGraph(obj.MoistureLevel);
    }
    
    if (obj.WateringLogs) {
        const logs = obj.WateringLogs;
        const ul = document.getElementById("WateringLogsTimes");
        ul.innerHTML = ""; // Clear previous entries

        Object.keys(logs)
            .filter(k => k.startsWith("Log"))
            .sort((a, b) => parseInt(b.replace("Log","")) - parseInt(a.replace("Log",""))) 
            .forEach(key => {
                const log = logs[key];

                const li = document.createElement("li");
                li.textContent = `${log.Date} at ${log.Time} — ${log.Amount} oz (${log.IsManual ? "Manual" : "Automatic"})`;

                ul.appendChild(li);
            });
    }

    if (obj.GrowLEDPower){
        updatePowerGraph(obj.GrowLEDPower);
    }
    
}

function onLoad(event){
    initWebSocket();
    initPowerGraph();
    initMoistureGraph();
}

// Set and Display Time
setInterval(() => {
    const now = new Date();
    const timeString = now.toLocaleTimeString('en-US', {
        hour: 'numeric',
        minute: 'numeric',
        hour12: true
    });
    document.getElementById("TimeValue").textContent = timeString;
}, 1000);

function drawMoistureGauge(value) {
    const canvas = document.getElementById("MoistureLevelCanvas");
    if (!canvas) return;

    const ctx = canvas.getContext("2d");
    const width = canvas.width;
    const height = canvas.height;

    ctx.clearRect(0, 0, width, height);

    // Create horizontal gradient: red -> yellow -> green
    const gradient = ctx.createLinearGradient(0, 0, width, 0);
    gradient.addColorStop(0, "red");
    gradient.addColorStop(0.5, "yellow");
    gradient.addColorStop(0.75, "green");
    gradient.addColorStop(1, "blue");

    // Draw the background gradient
    ctx.fillStyle = gradient;
    ctx.fillRect(0, 0, width, height);

    // Draw marker line at current value
    const markerX = (value / 100) * width;
    ctx.beginPath();
    ctx.moveTo(markerX, 0);
    ctx.lineTo(markerX, height);
    ctx.strokeStyle = "black";
    ctx.lineWidth = 2;
    ctx.stroke();
}

function initPowerGraph(){
    const ctx = document.getElementById("PowerCanvas").getContext("2d");

    powerGraph = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: "Power",
                data: [],
                borderWidth: 2,
                tension: 0.3
            }]
        },
        options: {
            animation: false,
            responsive: false,
            scales: {
                x: { display: false },
                y: { beginAtZero: true }
            }
        }
    });
}

let powerHistoryMax = 100;
function updatePowerGraph(value){

    // Store Data
    powerData.push(value);
    if (powerData.length > powerHistoryMax) {
        powerData.shift();
    }

    // Push to Graph
    powerGraph.data.labels = powerData.map((_,i) => i);
    powerGraph.data.datasets[0].data = powerData;
    powerGraph.update();
}


function initMoistureGraph(){
    const ctx = document.getElementById("MoistureCanvas").getContext("2d");

    moistureGraph = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: "Moisture",
                data: [],
                borderWidth: 2,
                tension: 0.3
            }]
        },
        options: {
            animation: false,
            responsive: false,
            scales: {
                x: { display: false },
                y: { beginAtZero: true }
            }
        }
    });
}

let moistureHistoryMax = 50;
function updateMoistureGraph(value){

    // Store Data
    moistureData.push(value);
    if (moistureData.length > moistureHistoryMax) {
        moistureData.shift();
    }

    // Push to Graph
    moistureGraph.data.labels = moistureData.map((_,i) => i);
    moistureGraph.data.datasets[0].data = moistureData;
    moistureGraph.update();
}

function sendManualWaterCommand(){

    const btn = document.getElementById("ManualWaterButton");
    btn.disabled = true; // Disable  button to prevent multiple presses
    websocket.send("PUMP_ON");

    // Off after 2 seconds
    setTimeout(() => {
        websocket.send("PUMP_OFF");
        btn.disabled = false;
    }, 2000);
}

function sendToggleGrowLEDCommand(){
    websocket.send("LED_FLASHRED")
}