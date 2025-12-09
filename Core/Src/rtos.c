#include "rtos.h"
#include "stm32l4xx_hal.h"   // for HAL_GetTick()


// Global tick in ms, incremented from SysTick interrupt
static volatile uint32_t g_rtosTickMs = 0;

// Task table
static RTOS_Task_t g_tasks[RTOS_MAX_TASKS];

// ================ RTOS_Init ==================
void RTOS_Init(void)
{
    for (int i = 0; i < RTOS_MAX_TASKS; i++)
    {
        g_tasks[i].func       = NULL;
        g_tasks[i].active     = 0;
        g_tasks[i].period_ms  = 0;
        g_tasks[i].lastRun_ms = 0;
    }
    g_rtosTickMs = 0;
}

// ================ RTOS_AddTask ===============
void RTOS_AddTask(RTOS_TaskFunc_t func, uint8_t startActive, uint32_t period_ms)
{
    if (func == NULL || period_ms == 0)
        return;

    for (int i = 0; i < RTOS_MAX_TASKS; i++)
    {
        if (g_tasks[i].func == NULL)
        {
            g_tasks[i].func       = func;
            g_tasks[i].active     = startActive ? 1 : 0;
            g_tasks[i].period_ms  = period_ms;
            g_tasks[i].lastRun_ms = 0;
            break;
        }
    }
}

// ================ RTOS_Tick ==================
void RTOS_Tick(void)
{
    g_rtosTickMs++;
}

// ================ RTOS_Run ===================
void RTOS_Run(void)
{
    uint32_t now = g_rtosTickMs;

    for (int i = 0; i < RTOS_MAX_TASKS; i++)
    {
        if (g_tasks[i].func != NULL && g_tasks[i].active)
        {
            uint32_t elapsed = now - g_tasks[i].lastRun_ms;

            if (elapsed >= g_tasks[i].period_ms)
            {
                g_tasks[i].lastRun_ms = now;
                g_tasks[i].func();   // run task
            }
        }
    }

    // Optional: tiny idle hint
    __NOP();
}
