#include <string.h>
#include <uart_debug.h>
#include <stdio.h>

// IMPORTANT: This handle is created in usart.c by CubeMX
extern UART_HandleTypeDef huart2;

/**
 * @brief  Sends a string out USART2 (blocking).
 * @note   Uses HAL_MAX_DELAY so transmit will never timeout.
 */
void Debug_Print(const char *msg)
{
    if (msg == NULL)
        return;

    HAL_UART_Transmit(&huart2,
                      (uint8_t *)msg,
                      (uint16_t)strlen(msg),
                      HAL_MAX_DELAY);
}

/**
 * @brief  Sends a string followed by CR LF.
 */
void Debug_Println(const char *msg)
{
    Debug_Print(msg);
    Debug_Print("\r\n");
}

void Debug_PrintFloat(const char *label, float value)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%s%.2f", label, value);
    Debug_Print(buf);
}
void Debug_PrintInt(int32_t v)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%ld", (long)v);
    Debug_Print(buf);
}
