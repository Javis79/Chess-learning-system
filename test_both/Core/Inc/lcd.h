/*
 * lcd.h
 *
 *  Created on: Sep 30, 2025
 *      Author: javis
 */

#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void lcd_init(void);                                  // init ILI9341
bool lcd_busy(void);                                  // true gdy SPI1 DMA nadaje
void lcd_set_window(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1);
void lcd_write_dma(const void* rgb565, uint32_t px_count); // start TX DMA (2B/px)

#ifdef __cplusplus
}
#endif
