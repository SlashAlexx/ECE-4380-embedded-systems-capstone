
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
window.addEventListener('load', onLoad);

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
    var moistureLevel = obj.MoistureLevel;
    
    document.getElementById("MoistureLevelValue").textContent = `Moisture Level: ${moistureLevel}%`;
    
}

function onLoad(event){
    initWebSocket();
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
