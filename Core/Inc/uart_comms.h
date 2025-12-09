#ifndef UART_COMMS_H
#define UART_COMMS_H

#include "stm32l4xx_hal.h"

// Init RX interrupt
void UART_Init(void);

// UART TX
void UART_Send(const char *msg);

// Processor (called in main loop)
void UART_ProcessReceived(void);

// ===== COMMAND HANDLERS =====
void CMD_PumpOn(void);
void CMD_PumpOff(void);
void CMD_PumpRun2s(void);

void CMD_LED_Purple(void);
void CMD_LED_Off(void);
void CMD_LED_FlashRed(void);
void CMD_LED_Full(void);
void CMD_LED_Dim(void);

void CMD_Request_Moisture(void);
void CMD_Request_Power(void);
// Send moisture percent as JSON for Arduino
void UART_SendMoisturePercent(float percent);


#endif
