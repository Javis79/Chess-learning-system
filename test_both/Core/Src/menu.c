/*
 * menu.c
 *
 *  Created on: Oct 16, 2025
 *      Author: javis
 */

#include "menu.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "stm32f4xx_hal.h"


//Parametry wyglądu UI:
#define TITLE_BG      DARKGREY
#define TITLE_FG      WHITE
#define ITEM_FG       WHITE
#define ITEM_BG       BLACK
#define ITEM_FG_SEL   WHITE
#define ITEM_BG_SEL   BLUE
#define BORDER_COL    WHITE

#define FONT_SIZE     2
#define CHAR_HEIGHT   5
#define LINE_H        (2*PADDING_Y + CHAR_HEIGHT * FONT_SIZE)  // wysokość wiersza
#define PADDING_X     12
#define PADDING_Y     10

#define Y_TITLE       6
#define Y_LIST        (Y_TITLE + 28)

typedef enum {
    IDX_CHESS_CLOCK = 0,
    IDX_OPENINGS    = 1,
	IDX_MUX			= 2,
    IDX_COUNT
} MenuIndex;

static const char* label_for_index(MenuIndex i)
{
    switch (i) {
        case IDX_CHESS_CLOCK: return "Zegar szachowy";
        case IDX_OPENINGS:    return "Otwarcia szachowe";
        case IDX_MUX:    	  return "Test wykrywania";
        default:              return "";
    }
}

// Prosty stan do detekcji zboczy + debounce
static uint32_t last_change_ms = 0;
static const uint32_t DEBOUNCE_MS = 120;

static uint8_t prev_up = 0, prev_down = 0, prev_sel = 0, prev_back = 0;

static uint8_t edge(uint8_t now, uint8_t *prev)
{
    uint8_t rising = (now && !(*prev));
    *prev = now;
    return rising;
}

static void ui_draw_title(const char *text)
{
    // pasek tytułu
	ILI9341_Draw_Filled_Rectangle_Coord(0, 0, ILI9341_SCREEN_WIDTH-1, 30, TITLE_BG);
    ILI9341_Draw_Text(text, PADDING_X, Y_TITLE, TITLE_FG, FONT_SIZE, TITLE_BG);
    ILI9341_Draw_Horizontal_Line(0, 31, ILI9341_SCREEN_WIDTH, BORDER_COL);
}

static void ui_draw_item(uint16_t y, const char *label, uint8_t selected)
{
    uint16_t bg = selected ? ITEM_BG_SEL : ITEM_BG;
    uint16_t fg = selected ? ITEM_FG_SEL : ITEM_FG;

    // tło wiersza
    ILI9341_Draw_Filled_Rectangle_Coord(0, y, ILI9341_SCREEN_WIDTH-1, y + LINE_H - 1, bg);
    // tekst
    ILI9341_Draw_Text(label, PADDING_X, y + PADDING_Y, fg, FONT_SIZE, bg);
}

static void ui_draw_frame(void)
{
    ILI9341_Fill_Screen(BLACK);                          // czyszczenie tła
    ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
    ui_draw_title("MENU GLOWNE");

    ui_draw_item(Y_LIST + 0*LINE_H, "Zegar szachowy", 1);
    ui_draw_item(Y_LIST + 1*LINE_H, "Otwarcia szachowe", 0);
    ui_draw_item(Y_LIST + 2*LINE_H, "Test wykrywania", 0);
}

void Menu_Init(void)
{
    // TODO
}

static uint8_t read_buttons_once(uint8_t *up_ev, uint8_t *down_ev, uint8_t *sel_ev, uint8_t *back_ev)
{
    uint32_t now = HAL_GetTick();

    uint8_t up   = button1_pressed();
    uint8_t down = button2_pressed();
    uint8_t sel  = button3_pressed();
    uint8_t back = button4_pressed();

    uint8_t e_up   = edge(up,   &prev_up);
    uint8_t e_dn   = edge(down, &prev_down);
    uint8_t e_sel  = edge(sel,  &prev_sel);
    uint8_t e_bak  = edge(back, &prev_back);

    // prosty debounce na wspólnym liczniku
    if (e_up || e_dn || e_sel || e_bak) {
        if (now - last_change_ms >= DEBOUNCE_MS) {
            last_change_ms = now;
            *up_ev = e_up;
            *down_ev = e_dn;
            *sel_ev = e_sel;
            *back_ev = e_bak;
            return 1;
        }
    }
    *up_ev = *down_ev = *sel_ev = *back_ev = 0;
    return 0;
}

MenuResult Menu_ShowMain(void)
{
    MenuIndex sel = IDX_CHESS_CLOCK;
    ui_draw_frame();

    while (1)
    {
        uint8_t ev_up, ev_dn, ev_sel, ev_back;
        if (read_buttons_once(&ev_up, &ev_dn, &ev_sel, &ev_back))
        {
            if (ev_up) {
                if (sel > 0) {
                	// zamiast warunkowych stringów:
                	ui_draw_item(Y_LIST + sel*LINE_H, label_for_index(sel), 0);
                	// po zmianie selekcji:
                	sel--;
                	ui_draw_item(Y_LIST + sel*LINE_H, label_for_index(sel), 1);

                }
            }
            if (ev_dn) {
                if (sel < IDX_COUNT-1) {
                	ui_draw_item(Y_LIST + sel*LINE_H, label_for_index(sel), 0);
                	sel++;
                	ui_draw_item(Y_LIST + sel*LINE_H, label_for_index(sel), 1);
                }
            }
            if (ev_sel) {
            	if (ev_sel) {
            	    if (sel == IDX_CHESS_CLOCK)  return MENU_RESULT_CHESS_CLOCK;
            	    if (sel == IDX_OPENINGS)     return MENU_RESULT_OPENINGS;
            	    if (sel == IDX_MUX)     	 return MENU_RESULT_MUX;
            	}

            }
            if (ev_back) {
                return MENU_RESULT_BACK;
            }
        }

        HAL_Delay(5);
    }
}








