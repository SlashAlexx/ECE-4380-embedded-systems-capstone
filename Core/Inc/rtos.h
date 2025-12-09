#ifndef RTOS_H
#define RTOS_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Max number of tasks in the scheduler
#define RTOS_MAX_TASKS   10

// Task function type: no args, no return
typedef void (*RTOS_TaskFunc_t)(void);

// Task structure
typedef struct
{
    RTOS_TaskFunc_t func;       // function pointer
    uint8_t         active;     // 1 = run, 0 = disabled
    uint32_t        period_ms;  // how often to run
    uint32_t        lastRun_ms; // last tick when it ran
} RTOS_Task_t;

// Public API
void RTOS_Init(void);
void RTOS_AddTask(RTOS_TaskFunc_t func, uint8_t startActive, uint32_t period_ms);
void RTOS_Run(void);
void RTOS_Tick(void);   // called from SysTick (ISR)

#ifdef __cplusplus
}
#endif

#endif // RTOS_H
