/*
 * menu.h
 *
 *  Created on: Oct 16, 2025
 *      Author: javis
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_


#include "stdint.h"
#include "buttons.h"
typedef enum {
    MENU_RESULT_NONE = 0,
    MENU_RESULT_CHESS_CLOCK,
    MENU_RESULT_OPENINGS,
	MENU_RESULT_MUX,
    MENU_RESULT_BACK
} MenuResult;

#define SD_SPI_HANDLE hspi2;

void Menu_Init(void);

//Pokazuje i obsługuje główne menu; zwraca wynik po wciśnięciu SELECT/BACK.

MenuResult Menu_ShowMain(void);


#endif /* INC_MENU_H_ */
