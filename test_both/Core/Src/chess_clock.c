/*
 * chess_clock.c
 *
 *  Created on: Oct 16, 2025
 *      Author: javis
 */


#include "chess_clock.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "stm32f4xx_hal.h"

//ustawienia
static uint32_t g_init_time_ms = 5 * 60 * 1000; // start: 5:00
static const uint32_t DEBOUNCE_MS = 100;
#define FONT_CHAR_W 5
#define FONT_CHAR_H 5
#define FONT_SPACING 1

//stan
typedef struct {
    uint32_t top_ms;
    uint32_t bot_ms;
    uint8_t  active;     // 0 = TOP, 1 = BOTTOM
    uint8_t  running;    // 0 = pauza
    uint32_t last_tick;
} ClockState;

//lokalny cache napisów, by rysować tylko zmienione znaki
static uint8_t first_draw = 1;
static char prev_top[6] = "-----";
static char prev_bot[6] = "-----";

static inline int clampi(int v, int lo, int hi) { return (v < lo) ? lo : (v > hi ? hi : v); }
static inline void ms_to_str(uint32_t ms, char *out)
{
    uint32_t total_s = ms / 1000;
    uint32_t mm = total_s / 60;
    uint32_t ss = total_s % 60;
    out[0] = '0' + (mm/10)%10;
    out[1] = '0' + (mm%10);
    out[2] = ':';
    out[3] = '0' + (ss/10);
    out[4] = '0' + (ss%10);
    out[5] = '\0';
}

static void draw_char_box(int x, int y_text, int fs, char c, uint16_t fg, uint16_t bg)
{
    int W = ILI9341_SCREEN_WIDTH;
    int H = ILI9341_SCREEN_HEIGHT;

    int charW = FONT_CHAR_W * fs;
    int charH = FONT_CHAR_H * fs;

    // małe marginesy wokół znaku
    const int PADX = 2, PADY = 2;

    int x0 = clampi(x - PADX, 0, W-1);
    int x1 = clampi(x + charW + PADX - 1, 0, W-1);
    int y0 = clampi(y_text - PADY, 0, H-1);
    int y1 = clampi(y_text + charH + PADY, 0, H-1);

    ILI9341_Draw_Filled_Rectangle_Coord(x0, y0, x1, y1, bg);
    char s[2] = { c, '\0' };
    ILI9341_Draw_Text(s, x, y_text, fg, fs, bg);
}

//pełne tła połówek (gdy start/switch)
static void draw_halves_bg(uint8_t active_top)
{
    int W = ILI9341_SCREEN_WIDTH;
    int H = ILI9341_SCREEN_HEIGHT;
    int halfH = H / 2;

    uint16_t top_bg = WHITE;
    uint16_t bot_bg = BLACK;

    ILI9341_Draw_Filled_Rectangle_Coord(0, 0,         W-1, halfH-1, top_bg);
    ILI9341_Draw_Filled_Rectangle_Coord(0, halfH,     W-1, H-1,     bot_bg);
}

void draw_time_single(uint8_t top, uint32_t ms, uint8_t active_top)
{
    // parametry geometrii
    const int fs = 6;
    const int adv = (FONT_CHAR_W + FONT_SPACING) * fs;
    const int textW_max = fs * (5*FONT_CHAR_W + 4*FONT_SPACING);
    int W = ILI9341_SCREEN_WIDTH;
    int H = ILI9341_SCREEN_HEIGHT;
    int halfH = H / 2;

    //wybór połowy ekranu
    int y_top = top ? 0 : halfH;
    int y_bot = top ? (halfH - 1) : (H - 1);

    uint16_t bg = top ? WHITE : BLACK;

    uint8_t is_active = top ? (active_top == 0) : (active_top == 1);

    uint16_t fg_active   = top ? BLACK : WHITE;
    uint16_t fg_inactive = DARKGREY;
    uint16_t fg = is_active ? fg_active : fg_inactive;

    int y_text = y_top + ( ( (y_bot - y_top + 1) - (FONT_CHAR_H * fs) ) / 2 );
    int x_start = (W - textW_max) / 2;

    // bieżący napis mm:ss
    char cur[6];
    ms_to_str(ms, cur);

    // wybór cache
    char *prev = top ? prev_top : prev_bot;

    for (int i = 0; i < 5; ++i)
    {
        if (first_draw || cur[i] != prev[i]) {
            int x = x_start + i * adv;
            int charW = FONT_CHAR_W * fs;
            int charH = FONT_CHAR_H * fs;
            const int PADX = 2, PADY = 2;

            int x0 = clampi(x - PADX, 0, W-1);
            int x1 = clampi(x + charW + PADX - 1, 0, W-1);
            int y0 = clampi(y_text - PADY, 0, H-1);
            int y1 = clampi(y_text + charH + PADY, 0, H-1);
            ILI9341_Draw_Filled_Rectangle_Coord(x0, y0, x1, y1, bg);

            char s[2] = { cur[i], '\0' };
            ILI9341_Draw_Text(s, x, y_text, fg, fs, bg);
        }
    }

    // aktualizacja cache
    for (int i = 0; i < 6; ++i) prev[i] = cur[i];
}


//edge detect (polling)
static uint8_t edge(uint8_t now, uint8_t *prev)
{
    uint8_t r = (now && !(*prev));
    *prev = now;
    return r;
}

void ChessClock_SetInitialTime(uint32_t ms_per_side) { g_init_time_ms = ms_per_side; }

void ChessClock_Run(void)
{
    // reset cache, by pierwsze rysowanie było pełne
    first_draw = 1; for (int i = 0; i < 6; ++i) { prev_top[i]='-'; prev_bot[i]='-'; }

    ClockState s;
    s.top_ms   = g_init_time_ms;
    s.bot_ms   = g_init_time_ms;
    s.active   = 0; // TOP start
    s.running  = 0;
    s.last_tick = HAL_GetTick();

    uint32_t prev_top_s = s.top_ms / 1000;
    uint32_t prev_bot_s = s.bot_ms / 1000;

    // orientacja i tła
    ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
    draw_halves_bg(s.active);
    // narysuj oba na start
    draw_time_single(1, s.top_ms, s.active);
    draw_time_single(0, s.bot_ms, s.active);
    first_draw = 0;

    // przyciski
    uint8_t p_up=0, p_dn=0, p_sel=0, p_back=0;
    uint32_t last_ev_ms = 0;

    while (1)
    {
        uint32_t now = HAL_GetTick();

        // odliczanie tylko dla aktywnej połowy
        if (s.running) {
            uint32_t dt = now - s.last_tick;
            if (dt) {
                if (s.active == 0) {
                    s.top_ms = (s.top_ms > dt) ? (s.top_ms - dt) : 0;
                } else {
                    s.bot_ms = (s.bot_ms > dt) ? (s.bot_ms - dt) : 0;
                }
                s.last_tick = now;

                if (s.active == 0) {
                    uint32_t cur = s.top_ms / 1000;
                    if (cur != prev_top_s) {
                        prev_top_s = cur;
                        draw_time_single(1, s.top_ms, s.active);
                    }
                } else {
                    uint32_t cur = s.bot_ms / 1000;
                    if (cur != prev_bot_s) {
                        prev_bot_s = cur;
                        draw_time_single(0, s.bot_ms, s.active);
                    }
                }
            }
        } else {
            s.last_tick = now;
        }

        // przyciski (edge + debounce)
        uint8_t ev = 0;
        uint8_t up   = button1_pressed();
        uint8_t down = button2_pressed();
        uint8_t sel  = button3_pressed();
        uint8_t back = button4_pressed();

        if (edge(up,&p_up) || edge(down,&p_dn) || edge(sel,&p_sel) || edge(back,&p_back)) {
            if (now - last_ev_ms >= DEBOUNCE_MS) { last_ev_ms = now; ev = 1; }
        }

        if (ev) {
            if (p_back) {
                ILI9341_Set_Rotation(SCREEN_VERTICAL_2); // powrót do menu
                return;
            }
            if (p_up) {
                s.running = !s.running;
            }
            if (p_sel) {
                if (s.running) {
                    s.active ^= 1;
                    draw_halves_bg(s.active);
                    first_draw = 1;
                    draw_time_single(1, s.top_ms, s.active); // TOP
                    draw_time_single(0, s.bot_ms, s.active); // BOTTOM
                    first_draw = 0;
                }
            }

            if (p_dn) {
                if (!s.running) {
                    s.top_ms = s.bot_ms = g_init_time_ms;
                    s.active = 0; // wróć na górny
                    prev_top_s = s.top_ms / 1000;
                    prev_bot_s = s.bot_ms / 1000;
                    draw_halves_bg(s.active);
                    first_draw = 1;
                    draw_time_single(1, s.top_ms, s.active);
                    draw_time_single(0, s.bot_ms, s.active);
                    first_draw = 0;
                }
            }
        }

        HAL_Delay(3);
    }
}
