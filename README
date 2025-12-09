# ECE-4380 Embedded Systems – Automated STM32 Hydroponics

This repository contains the STM32 firmware for the automated hydroponics system final project.  
The STM32 runs a lightweight custom RTOS and communicates with an ESP32 dashboard over UART.

---

## Overview

The STM32 is responsible for:
- Reading soil moisture
- Running the pump when soil is dry
- Monitoring the grow light current via INA219
- Detecting overcurrent
- Sending JSON messages to the ESP32
- Receiving UART commands (AUTO / MANUAL)

The ESP32 handles:
- WiFi + Web dashboard
- Logging and timestamping
- WebSocket data
- Storing sensor history

---

## Software Defined State Machine

The STM32 uses a simple state machine executed from an RTOS task.  
The major states are:

### **1. Initialization State (`State_Init`)**
- Sets default LED color  
- Reads initial moisture value  
- Prints initial status over UART debug  
- Transitions to AUTO mode

### **2. Auto Mode (`State_Auto`)**
- Runs the watering decision logic  
- If soil is **dry** → pump for 2 seconds  
- If soil is **wet** → do nothing  
- Disabled if the system is in fail mode

### **3. Manual Mode (`State_Manual`)**
- STM32 disables automatic control  
- ESP32 dashboard can command pump/LED manually  
- Used for debugging or user override

### **4. Moisture Update (`State_UpdateMoisture`)**
- Reads raw and averaged ADC moisture  
- Updates the global soil state  

### **5. Power + Fail Check (`State_UpdatePower`)**
- Reads INA219 voltage/current/power  
- Checks for overcurrent (>1.5A)  
- Updates fail state and flashes error LED if needed  

### **6. State Machine Tick (`State_StateMachineTick`)**
- Called by the RTOS at a fixed rate  
- Runs either Auto or Manual mode

---

## UART Communication Format

The STM32 sends small JSON messages to the ESP32, for example:

```json
{"MoistureLevel": 42}
```

```json
{"AutoWateringLog": true}
```

```json
{"GrowLEDPower": 1180.55}
```

---

## Build Information

- STM32F4 series MCU  
- STM32CubeIDE  
- HAL drivers  
- UART @ **115200 baud**  

---

## Group Members

- Dennis Rivera  
- Alexander Roberts  
- Kohen Yoakum  
