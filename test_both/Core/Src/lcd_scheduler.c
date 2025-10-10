/*
 * lcd_scheduler.c
 *
 *  Created on: Sep 30, 2025
 *      Author: javis
 */

#include "lcd_scheduler.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "stm32f4xx_hal.h"

// prosta animacja prostokąta
static uint32_t t_gui=0;
static int x=0, y=40, dx=3;
static const uint16_t W = 240, H = 320;

void lcd_scheduler_init(void){
	// Start ekranu i ustawienie orientacji
    ILI9341_Fill_Screen(BLACK);
    ILI9341_Set_Rotation(SCREEN_VERTICAL_2);

    ILI9341_Draw_Text("WS2812B + ILI9341", 8, 8, YELLOW, 2, BLACK);
    ILI9341_Draw_Text("DMA + bez blokad",  8, 28, CYAN,   1, BLACK);
}

//Wstępny testowy scheduler, obraz w około 30 klatkach na sekundę
void lcd_scheduler_tick(void){
    uint32_t now = HAL_GetTick();
    if (now - t_gui >= 33) {
        t_gui = now;

        ILI9341_Draw_Rectangle(x, y, 40, 20, BLACK);
        x += dx;
        if (x <= 0) { x=0; dx = +3; }
        if (x + 40 >= W) { x = W-40; dx = -3; }
        ILI9341_Draw_Rectangle(x, y, 40, 20, MAGENTA);
        ILI9341_Draw_Rectangle(0, 36, W-1, 24, DARKGREY);
    }
}


