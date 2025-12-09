#ifndef GROW_LED_H
#define GROW_LED_H

#include "ws28xx.h"
#include "main.h"
#include <stdint.h>

#define WS_LED_COUNT 20   // <-- your current strip size

extern WS28XX_HandleTypeDef ws_led;

// Core functions
void init_grow_leds(void);
void update_grow_leds(uint8_t brightness);
void flash_error_led(void);

// Additional helper functions (safe brightness control)
void grow_set_brightness(uint8_t brightness);
void grow_dim_to_safe(void);     // goes to 120
void grow_full_brightness(void); // goes back to 255


// Color helper for YOUR LED STRIP (BGR hardware)
void SetPixel_BGR(uint16_t index, uint8_t r, uint8_t g, uint8_t b);

#endif
