/*
 * chess_clock.h
 *
 *  Created on: Oct 16, 2025
 *      Author: javis
 */

#ifndef INC_CHESS_CLOCK_H_
#define INC_CHESS_CLOCK_H_

#include <stdint.h>
#include "buttons.h"

void ChessClock_Run(void);

/*Ustaw czas startowy w milisekundach (np. 5 min = 5*60*1000) */
void ChessClock_SetInitialTime(uint32_t ms_per_side);


#endif /* INC_CHESS_CLOCK_H_ */
