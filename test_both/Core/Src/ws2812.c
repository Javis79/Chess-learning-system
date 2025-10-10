/*
 * ws2812.c
 *
 *  Created on: Sep 30, 2025
 *      Author: javis
 *
 *      Kod do obsługi matrycy LEDów ARGB
 */


#include "ws2812.h"
#include "stm32f4xx_hal.h"

extern TIM_HandleTypeDef htim1;
#define T1H_TICKS 60
#define T0H_TICKS 30
#define RESET_SLOTS 50

static uint8_t  s_rgb[WS2812_MAX_LED][3];       // [G,R,B]
static uint8_t  s_rgb_scaled[WS2812_MAX_LED][3];
static uint16_t s_pwm[(WS2812_MAX_LED*24)+RESET_SLOTS];
static volatile bool s_busy = false;
static uint8_t s_brightness = 100;

static inline void encode_to_pwm(void) {
    // jasność
    if (s_brightness >= 100) {
        for (uint16_t i=0;i<WS2812_MAX_LED;i++){
            s_rgb_scaled[i][0]=s_rgb[i][0]; s_rgb_scaled[i][1]=s_rgb[i][1]; s_rgb_scaled[i][2]=s_rgb[i][2];
        }
    } else {
        for (uint16_t i=0;i<WS2812_MAX_LED;i++){
            s_rgb_scaled[i][0]=(uint16_t)s_rgb[i][0]*s_brightness/100;
            s_rgb_scaled[i][1]=(uint16_t)s_rgb[i][1]*s_brightness/100;
            s_rgb_scaled[i][2]=(uint16_t)s_rgb[i][2]*s_brightness/100;
        }
    }
    uint32_t k=0;
    for (uint16_t i=0;i<WS2812_MAX_LED;i++){
        uint32_t grb = ((uint32_t)s_rgb_scaled[i][0]<<16) | ((uint32_t)s_rgb_scaled[i][1]<<8) | s_rgb_scaled[i][2];
        for (int8_t b=23;b>=0;b--) s_pwm[k++] = (grb&(1u<<b))?T1H_TICKS:T0H_TICKS;
    }
    for (uint16_t i=0;i<RESET_SLOTS;i++) s_pwm[k++]=0;
}

void ws2812_init(void) {}

//ustawienie koloru pojedynczej diody
void ws2812_set(uint16_t idx,uint8_t r,uint8_t g,uint8_t b){
    if(idx>=WS2812_MAX_LED) return;
    s_rgb[idx][0]=g; s_rgb[idx][1]=r; s_rgb[idx][2]=b;
}

//ustawienie tego samego koloru dla wszystkich diod
void ws2812_set_all(uint8_t r,uint8_t g,uint8_t b){
    for(uint16_t i=0;i<WS2812_MAX_LED;i++){ s_rgb[i][0]=g; s_rgb[i][1]=r; s_rgb[i][2]=b; }
}
void ws2812_set_brightness(uint8_t p){ if(p>100)p=100; s_brightness=p; }
bool ws2812_busy(void){ return s_busy; }


//asynchroniczne wyświetlanie
bool ws2812_show_async(void){
    if(s_busy) return false;
    s_busy = true;
    encode_to_pwm();
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)s_pwm, (WS2812_MAX_LED*24)+RESET_SLOTS);
    return true;
}

//callback HAL po zakończeniu bufora DMA
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
    if(htim->Instance==TIM1){
        HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
        s_busy=false;
    }
}
