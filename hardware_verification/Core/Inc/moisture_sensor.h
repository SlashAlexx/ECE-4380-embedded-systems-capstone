#ifndef MOISTURE_SENSOR_H
#define MOISTURE_SENSOR_H

#include <stdint.h>
#include <stdbool.h>

// ----- Moisture State Enum -----
typedef enum {
    MOISTURE_DRY = 0,
    MOISTURE_WET = 1
} MoistureState_t;

// ----- Moisture Reading Struct -----
typedef struct {
    uint32_t raw_adc;       // last single ADC reading
    uint32_t avg_adc;       // averaged oversample result
    float    percent;       // estimated percent moisture
    MoistureState_t state;  // wet/dry state
    uint32_t timestamp_ms;  // HAL_GetTick timestamp
} MoistureReading_t;

// ===== Core Moisture Reading Functions =====
void MoistureSensor_Init(void);
MoistureReading_t Moisture_Read(void);
void Moisture_Print(void);

// ===== Helper Functions (safe additions) =====
bool Moisture_IsWet(void);
bool Moisture_IsDry(void);
float Moisture_GetPercent(void);
uint32_t Moisture_GetAvgADC(void);
uint32_t Moisture_GetRawADC(void);
const char* Moisture_GetStateString(void);

#endif
