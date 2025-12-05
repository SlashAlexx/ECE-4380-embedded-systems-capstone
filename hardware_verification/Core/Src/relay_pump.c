#include "relay_pump.h"
#include "uart_debug.h"

void Pump_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin = PUMP_RELAY_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(PUMP_RELAY_GPIO_PORT, &GPIO_InitStruct);

    Pump_Off(); // safe state

    Debug_Print("PUMP: Initialized (active LOW)\r\n");
}

// Relay ACTIVE LOW → ON = 0
void Pump_On(void)
{
    HAL_GPIO_WritePin(PUMP_RELAY_GPIO_PORT, PUMP_RELAY_PIN, GPIO_PIN_RESET);
    Debug_Print("PUMP: ON\r\n");
}

// Relay ACTIVE LOW → OFF = 1
void Pump_Off(void)
{
    HAL_GPIO_WritePin(PUMP_RELAY_GPIO_PORT, PUMP_RELAY_PIN, GPIO_PIN_SET);
    Debug_Print("PUMP: OFF\r\n");
}

// Run pump for exactly 2 seconds
void Pump_Run2s(void)
{
    Debug_Print("PUMP: Running for 2 seconds...\r\n");

    Pump_On();
    HAL_Delay(2000);
    Pump_Off();

    Debug_Print("PUMP: 2-second cycle complete\r\n");
}
