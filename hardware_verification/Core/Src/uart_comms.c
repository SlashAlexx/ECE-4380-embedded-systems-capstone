#include "uart_comms.h"
#include "relay_pump.h"
#include "grow_led.h"
#include "moisture_sensor.h"
#include "ina219.h"
#include "uart_debug.h"

#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart1;

static uint8_t rxByte;
static char uart_rx_buffer[128];
static volatile uint8_t uart_rx_index = 0;

/****************************************************
 * SAFE BUFFER RESET
 ****************************************************/
static void UART_ClearBuffer(void)
{
    memset(uart_rx_buffer, 0, sizeof(uart_rx_buffer));
    uart_rx_index = 0;
}

/****************************************************
 * UART Init
 ****************************************************/
void UART_Init(void)
{
    UART_ClearBuffer();
    HAL_UART_Receive_IT(&huart1, &rxByte, 1);
}

/****************************************************
 * UART Send
 ****************************************************/
void UART_Send(const char *msg)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

/****************************************************
 * RX Callback
 ****************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        if (rxByte == '\n')
        {
            uart_rx_buffer[uart_rx_index] = '\0';
        }
        else if (uart_rx_index < sizeof(uart_rx_buffer) - 1)
        {
            uart_rx_buffer[uart_rx_index++] = rxByte;
        }

        HAL_UART_Receive_IT(&huart1, &rxByte, 1);
    }
}

/****************************************************
 * COMMAND PROCESSOR
 ****************************************************/
void UART_ProcessReceived(void)
{
    if (uart_rx_buffer[0] == '\0')
        return;

    // PUMP
    if (strcmp(uart_rx_buffer, "PUMP_ON") == 0)       CMD_PumpOn();
    else if (strcmp(uart_rx_buffer, "PUMP_OFF") == 0) CMD_PumpOff();
    else if (strcmp(uart_rx_buffer, "PUMP_2S") == 0)  CMD_PumpRun2s();

    // LED
    else if (strcmp(uart_rx_buffer, "LED_PURPLE") == 0)   CMD_LED_Purple();
    else if (strcmp(uart_rx_buffer, "LED_OFF") == 0)      CMD_LED_Off();
    else if (strcmp(uart_rx_buffer, "LED_FLASHRED") == 0) CMD_LED_FlashRed();
    else if (strcmp(uart_rx_buffer, "LED_FULL") == 0)     CMD_LED_Full();
    else if (strncmp(uart_rx_buffer, "LED_DIM", 7) == 0)   CMD_LED_Dim();

    // Sensors
    else if (strcmp(uart_rx_buffer, "REQ_MOIST") == 0)    CMD_Request_Moisture();
    else if (strcmp(uart_rx_buffer, "REQ_POWER") == 0)    CMD_Request_Power();

    UART_ClearBuffer();
}

/****************************************************
 * COMMANDS — Only calling your modules, NO new logic
 ****************************************************/
void CMD_PumpOn(void)
{
    Pump_On();
    UART_Send("{\"pump\":\"on\"}\n");
}

void CMD_PumpOff(void)
{
    Pump_Off();
    UART_Send("{\"pump\":\"off\"}\n");
}

void CMD_PumpRun2s(void)
{
    Pump_Run2s();
    UART_Send("{\"pump\":\"2s\"}\n");
}

/****************************************************
 * LED COMMANDS — Using EXACT existing logic
 ****************************************************/
void CMD_LED_Purple(void)
{
    set_grow_purple(255);
    UART_Send("{\"led\":\"purple\"}\n");
}

void CMD_LED_Off(void)
{
    // FULL WIPE
    for (uint16_t i = 0; i < WS_LED_COUNT; i++)
        SetPixel_BGR(i, 0, 0, 0);

    WS28XX_Update(&ws_led);

    UART_Send("{\"led\":\"off\"}\n");
}

void CMD_LED_FlashRed(void)
{
    flash_error_led();
    UART_Send("{\"led\":\"flash_red\"}\n");
}

void CMD_LED_Full(void)
{
    grow_full_brightness();
    UART_Send("{\"led\":\"full\"}\n");
}

void CMD_LED_Dim(void)
{
    int value = 0;
    sscanf(uart_rx_buffer, "LED_DIM %d", &value);

    if (value < 0) value = 0;
    if (value > 255) value = 255;

    grow_set_brightness(value);

    char msg[64];
    snprintf(msg, sizeof(msg), "{\"led_dim\":%d}\n", value);
    UART_Send(msg);
}

/****************************************************
 * MOISTURE SENSOR
 ****************************************************/
void CMD_Request_Moisture(void)
{
    MoistureReading_t m = Moisture_Read();

    char msg[128];
    snprintf(msg, sizeof(msg),
        "{\"moisture\":%.2f,\"raw\":%lu,\"avg\":%lu,\"state\":\"%s\"}\n",
        m.percent,
        m.raw_adc,
        m.avg_adc,
        (m.state == MOISTURE_WET ? "wet" : "dry"));

    UART_Send(msg);
}

/****************************************************
 * INA219 POWER
 ****************************************************/
void CMD_Request_Power(void)
{
    INA219_Data_t d;
    INA219_Read(&d);

    char msg[128];
    snprintf(msg, sizeof(msg),
        "{\"voltage\":%.2f,\"current\":%.2f,\"power\":%.2f}\n",
        d.bus_V,
        d.current_mA,
        d.power_mW);

    UART_Send(msg);
}
