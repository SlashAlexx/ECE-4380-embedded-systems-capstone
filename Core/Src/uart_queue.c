#include "uart_queue.h"
#include <string.h>

#define UART_QUEUE_SIZE 20

static UART_Message_t queue[UART_QUEUE_SIZE];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

// ===============================
// INIT
// ===============================
void UQ_Init(void)
{
    head = 0;
    tail = 0;
}

// ===============================
// PUSH MESSAGE
// returns false on FULL
// ===============================
bool UQ_Push(UART_Message_t msg)
{
    uint8_t next = (head + 1) % UART_QUEUE_SIZE;

    if (next == tail)
        return false; // queue full

    queue[head] = msg;
    head = next;
    return true;
}

// ===============================
// POP MESSAGE
// returns false on EMPTY
// ===============================
bool UQ_Pop(UART_Message_t *out)
{
    if (head == tail)
        return false; // empty

    *out = queue[tail];
    tail = (tail + 1) % UART_QUEUE_SIZE;

    return true;
}
