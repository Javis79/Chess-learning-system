/*
 * led_scheduler.c
 *
 *  Created on: Sep 30, 2025
 *      Author: javis
 */

#include "led_scheduler.h"
#include "ws2812.h"
#include "stm32f4xx_hal.h"

// prosta paleta tęczy
static void hsv_to_rgb(uint16_t h, uint8_t s, uint8_t v, uint8_t* r, uint8_t* g, uint8_t* b) {
    uint8_t region = h / 60;
    uint16_t f = (h % 60) * 255 / 60;
    uint16_t p = (uint16_t)v*(255-s)/255;
    uint16_t q = (uint16_t)v*(255 - (s*f)/255)/255;
    uint16_t t = (uint16_t)v*(255 - (s*(255 - f))/255)/255;
    switch(region){
        default:
        case 0: *r=v; *g=t; *b=p; break;
        case 1: *r=q; *g=v; *b=p; break;
        case 2: *r=p; *g=v; *b=t; break;
        case 3: *r=p; *g=q; *b=v; break;
        case 4: *r=t; *g=p; *b=v; break;
        case 5: *r=v; *g=p; *b=q; break;
    }
}

static uint32_t t_led=0;
static uint16_t pos=0;
static int br=12, dir=+2;

void led_scheduler_init(void){
    ws2812_init();
    ws2812_set_all(0,0,0);
    ws2812_set_brightness(br);
}

void led_scheduler_tick(void){
    uint32_t now = HAL_GetTick();
    if (!ws2812_busy() && (now - t_led) >= 16) {
        t_led = now;

        // animacja na ledach
        for (uint16_t i=0;i<WS2812_MAX_LED;i++){
            uint16_t hue = (uint16_t)((i*360)/WS2812_MAX_LED + pos) % 360;
            uint8_t r,g,b; hsv_to_rgb(hue, 255, 255, &r,&g,&b);
            ws2812_set(i, r/12, g/12, b/12); // tło lekko podświetlone
        }
        uint8_t r,g,b; hsv_to_rgb((pos)%360, 255, 255, &r,&g,&b);
        ws2812_set(pos % WS2812_MAX_LED, r, g, b);

        pos = (pos + 1) % WS2812_MAX_LED;

        br += dir; if (br>=60){br=60;dir=-2;} if (br<=5){br=5;dir=+2;}
        ws2812_set_brightness((uint8_t)br);

        ws2812_show_async();
    }
}

