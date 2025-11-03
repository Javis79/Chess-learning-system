/*
 * mux.h
 *
 *  Created on: Oct 28, 2025
 *      Author: javis
 */

#ifndef INC_MUX_H_
#define INC_MUX_H_

#include "stm32f4xx_hal.h"

int mux_1_state(void);
int mux_2_state(void);

// Funkcja ustawiająca odpowiednią kombinację bitów (0–7) na pinach sterujących
void set_mux_select(uint8_t selection);

// Funkcja sterująca, która przechodzi przez wszystkie kanały multipleksera
void cycle_mux_channels(void);


#endif /* INC_MUX_H_ */
