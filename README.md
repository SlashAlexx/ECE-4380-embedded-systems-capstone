# ECE-4380 Embedded Systems – Automated STM32 Hydroponics

This repository contains the firmware for an automated hydroponics system.  
The STM32 runs a lightweight custom RTOS and communicates with an ESP32 dashboard over UART.

---

## Overview

The STM32 is responsible for:

- Reading soil moisture
- Running the pump when the soil is dry
- Monitoring grow light current via the INA219
- Detecting overcurrent situations
- Sending JSON messages to the ESP32
- Receiving UART commands (AUTO / MANUAL)

The ESP32 handles:

- WiFi and hosting the web dashboard
- Logging sensor data and timestamps
- Sending and receiving WebSocket messages
- Storing historical sensor data

---

## State Machine

The STM32 runs a simple state machine as part of an RTOS task. The main states include:

### **1. Initialization (`State_Init`)**
- Sets the default LED color
- Reads the initial moisture value
- Prints status over UART
- Switches to AUTO mode

### **2. Auto Mode (`State_Auto`)**
- Checks moisture readings and decides whether to water
- Dry soil -> pump runs for 2 seconds  
- Wet soil -> no action  
- Disabled if the system is in a fail state

### **3. Manual Mode (`State_Manual`)**
- Automatic watering is disabled
- Pump and LEDs can be controlled from the ESP32 dashboard
- Useful for debugging or user override

### **4. Moisture Update (`State_UpdateMoisture`)**
- Reads raw and averaged ADC values
- Updates the global soil moisture state

### **5. Power & Fail Check (`State_UpdatePower`)**
- Reads voltage, current, and power from the INA219
- Flags overcurrent (>1.5A)
- Updates fail state and flashes error LED if necessary

### **6. State Machine Tick (`State_StateMachineTick`)**
- Runs periodically from the RTOS
- Calls either Auto or Manual state logic

---

## Build and Run Instructions

### STM32 (STM32CubeIDE)

1. **Open the Project**
   - Launch STM32 and open the `stm32/` folder.
   - Open the .project or .ino file

2. **Build & Upload**
   - Click **Verify** to compile  
   - Click **Upload** to flash the STM32  
   - Open Serial Monitor at 115200 baud to see debug output
---

### ESP32 Dashboard (Arduino IDE)

1. **Open the Project**
   - Open `esp32/PlantWatering/PlantWatering.io` in Arduino IDE 

2. **Install Libraries in Package Manager**
   - `AsyncTCP`  
   - `ESPAsyncWebServer`  
   - `ArduinoJson`  

3. **Upload SPIFFS Files**
   - Use the **ESP32 Sketch Data Upload** tool to flash the SPIFFS filesystem

4. **Build & Upload Firmware**
   - Connect the ESP32 via USB  
   - Click **Upload** 
   - The ESP32 will:
     - Connect to WiFi
     - Start the WebSocket server
     - Host the dashboard

5. **Access the Dashboard**
   - After boot, check the Serial Monitor for the ESP32's local IP
   - Open in any browser:
     ```
     http://<ESP32-IP>/
     ```
   - Dashboard displays:
     - Moisture graph
     - LED power graph
     - Watering logs
     - Control buttons

6. **Verify UART Communication**
   - When the STM32 sends JSON, the ESP32 Serial Monitor should display:
     ```
     UART RAW JSON: {"MoistureLevel": 84.0}
     ```

