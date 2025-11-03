/*
 * buttons.c
 *
 *  Created on: Oct 16, 2025
 *      Author: javis
 */

#include "buttons.h"

int button1_pressed(void) {
    // PB0
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET;  // Przycisk wciśnięty, stan niski
}

int button2_pressed(void) {
	// PA4
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET;  // Przycisk wciśnięty, stan niski

}

int button3_pressed(void) {
    // PA1
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET;  // Przycisk wciśnięty, stan niski
}

int button4_pressed(void) {
	// PA0
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET;  // Przycisk wciśnięty, stan niski
}


