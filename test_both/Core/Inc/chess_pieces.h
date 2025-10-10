/*
 * chess_pieces.h
 *
 *  Created on: Oct 6, 2025
 *      Author: javis
 */

#ifndef INC_CHESS_PIECES_H_
#define INC_CHESS_PIECES_H_
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "ws2812.h"

typedef enum {
    LEDCHESS_WHITE = +1,  //kierunek figury: malejący rząd to -1, rosnący to +1
    LEDCHESS_BLACK = -1
} ledchess_side_t;

typedef enum {
    PIECE_PAWN, PIECE_KNIGHT, PIECE_BISHOP, PIECE_ROOK, PIECE_QUEEN, PIECE_KING
} ledchess_piece_t;


int  ledchess_sq_to_rc(const char sq[2], int* r, int* c);

void ledchess_set_origin_top(bool originTop);

uint16_t ledchess_xy_to_index(int r, int c);


void ledchess_draw_move_rc(ledchess_piece_t piece, int r0, int c0, int r1, int c1,
                           uint8_t cr, uint8_t cg, uint8_t cb,
                           ledchess_side_t pawn_dir);


void ledchess_draw_move(ledchess_piece_t piece, const char from[2], const char to[2],
                        uint8_t cr, uint8_t cg, uint8_t cb,
                        ledchess_side_t pawn_dir);

void ledchess_draw_pawn_push(const char from[2], int steps, uint8_t r, uint8_t g, uint8_t b,
                             ledchess_side_t pawn_dir);

void leds_show_and_pause(uint32_t ms);

#endif /* INC_CHESS_PIECES_H_ */
