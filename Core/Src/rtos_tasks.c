#include "rtos_tasks.h"
#include "uart_comms.h"
#include "uart_debug.h"
#include "system_state.h"
#include "relay_pump.h"
#include "moisture_model.h"
#include <stdio.h>
#include "moisture_sensor.h"

// ===============================
// 1) UART task
// ===============================
void Task_UART(void)
{
    UART_ProcessReceived();
}

// ===============================
// 2) Moisture task
// ===============================
void Task_Moisture(void)
{
    static uint8_t counter = 0;
    counter++;

    // Read sensor
    MoistureReading_t r = Moisture_Read();

    // Feed "wet or dry" into smoothing model
    MoistureModel_AddState(r.state == MOISTURE_WET);

    // Get smoothed moisture percent
    float percent = MoistureModel_GetPercent();

    // Print every 10 cycles
    if (counter >= 10)
    {
        Debug_Print("MOIST: raw=");
        Debug_PrintInt(r.raw_adc);

        Debug_Print(" state=");
        Debug_Print(r.state == MOISTURE_WET ? "WET" : "DRY");

        Debug_Print(" P=");
        Debug_PrintFloat("", percent);
        Debug_Print("%\r\n");

        UART_SendMoisturePercent(percent);

        counter = 0;
    }

    // Update state machine (no args needed)
    State_UpdateMoisture();
}


// ===============================
// 3) INA219 task
// ===============================
void Task_INA219(void)
{
    State_UpdatePower();
}

// ===============================
// 4) Pump / Main State-Machine
// ===============================
void Task_Pump(void)
{
    if (State_GetMode() != MODE_MANUAL)
    {
        State_StateMachineTick();
    }

}

// ===============================
// 5) LED task (free for animations)
// ===============================
void Task_LED(void)
{

    // Your LED code can go here
}

// ===============================
// 6) HEARTBEAT task – debug
// ===============================
void Task_Heartbeat(void)
{
    Debug_Print("RTOS: Heartbeat OK\r\n");
}
