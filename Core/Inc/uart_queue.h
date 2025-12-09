#ifndef UART_QUEUE_H
#define UART_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

// ========================
// 1) Message Types
// ========================
typedef enum {
    MSG_MOISTURE,
    MSG_POWER
} UART_MessageType_t;

// ========================
// 2) Moisture Payload
// ========================
typedef struct {
    bool isWet;
} MoistureMsg_t;

// ========================
// 3) Power Payload
// ========================
typedef struct {
    float current;
    float voltage;
} PowerMsg_t;

// ========================
// 4) The unified queue element
// ========================
typedef struct {
    UART_MessageType_t type;

    union {
        MoistureMsg_t moisture;
        PowerMsg_t    power;
    };
} UART_Message_t;

// ========================
// Queue API
// ========================
void UQ_Init(void);
bool UQ_Push(UART_Message_t msg);
bool UQ_Pop(UART_Message_t *out);


typedef struct {
	UART_MessageType_t type;
	float value;
} UART_queue_msg;


#endif
