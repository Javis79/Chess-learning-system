/*
 * ws2812.h
 *
 *  Created on: Sep 30, 2025
 *      Author: javis
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifndef WS2812_MAX_LED
#define WS2812_MAX_LED 64
#endif

#ifdef __cplusplus
extern "C" {
#endif

void ws2812_init(void);
void ws2812_set(uint16_t idx, uint8_t r, uint8_t g, uint8_t b);
void ws2812_set_all(uint8_t r, uint8_t g, uint8_t b);
void ws2812_set_brightness(uint8_t percent_0_100);
bool ws2812_show_async(void);   //start DMA; false jeśli wciąż trwa poprzedni
bool ws2812_busy(void);

#ifdef __cplusplus
}
#endif
