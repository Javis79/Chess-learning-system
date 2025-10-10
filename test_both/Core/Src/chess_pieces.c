/*
 * chess_pieces.c
 *
 *  Created on: Oct 6, 2025
 *      Author: javis
 *
 *      Rysowanie ścieżek ruchów figur szachowych na macierzy 8x8 WS2812
 */
#include "chess_pieces.h"

 //rząd 0 to domyślnie „góra” szachownicy
static bool s_originTop = true;
//funckja do ustawiania orientacji szachownicy
void ledchess_set_origin_top(bool originTop){ s_originTop = originTop; }


//sprawdzanie czy współrzędne podane mieszczą się w szachownicy
static inline int inb(int r, int c){ return (r>=0 && r<8 && c>=0 && c<8); }


//zmiana notacji szachowej ("e4") na rzędy i kolumny
int ledchess_sq_to_rc(const char sq[2], int* r, int* c){
    if (!sq) return -1;
    char f = sq[0], rk = sq[1];
    if (f<'a'||f>'h'||rk<'1'||rk>'8') return -1;
    int file = f - 'a';
    int rank = rk - '1';
    if (s_originTop){
        *r = 7 - rank;
        *c = file;
    } else {
        *r = rank;
        *c = file;
    }
    return 0;
}

//Ułożenie szachownicy to serpentyna 8x8 (rząd 0 L->P, rząd 1 P->L, itd.)
uint16_t ledchess_xy_to_index(int r, int c){
    if (!inb(r,c)) return 0;
    if ((r & 1) == 0){
        return (uint16_t)(r*8 + c);
    } else {
        return (uint16_t)(r*8 + (7 - c));
    }
}




//rysowanie ścieżek ruchów

static void set_sq(int r,int c, uint8_t R,uint8_t G,uint8_t B){
    if (!inb(r,c)) return;
    uint16_t idx = ledchess_xy_to_index(r,c);
    ws2812_set(idx, R,G,B);
}

//rysuje linię od (r,c) w kierunku (dr,dc) aż do (r1,c1) włącznie
static void draw_line_dir(int r,int c, int dr,int dc,
                          int r1,int c1, uint8_t R,uint8_t G,uint8_t B){
    int cr = r, cc = c;
    while (inb(cr,cc)){
        cr += dr; cc += dc;
        set_sq(cr,cc,R,G,B);
        if (cr==r1 && cc==c1) break;
    }
}

//sprawdzanie czy koń nie chce "wyskoczyć" poza obszar gry
static int is_knight_target(int r0,int c0,int r1,int c1){
    int dr = r1 - r0, dc = c1 - c0;
    if (dr<0) dr=-dr; if (dc<0) dc=-dc;
    return (dr==2 && dc==1) || (dr==1 && dc==2);
}

//sprawdzanie czy ruch wykonywany jest w ramach tej samej przekątnej / rzędu / kolumny
static int same_diag(int r0,int c0,int r1,int c1){ return (r0-c0)==(r1-c1) || (r0+c0)==(r1+c1); }
static int same_rank(int r0,int c0,int r1,int c1){ (void)c0; (void)c1; return r0==r1; }
static int same_file(int r0,int c0,int r1,int c1){ (void)r0; (void)r1; return c0==c1; }

void ledchess_draw_move_rc(ledchess_piece_t piece, int r0, int c0, int r1, int c1,
                           uint8_t cr, uint8_t cg, uint8_t cb,
                           ledchess_side_t pawn_dir)
{
    if (!inb(r0,c0) || !inb(r1,c1)) return;

    set_sq(r0,c0, cr, cg/2, cb/2);
    set_sq(r1,c1, cr, cg, cb);

    switch (piece){
    case PIECE_PAWN: {
        int dir = (int)pawn_dir;     // +1 lub -1
        int dr = r1 - r0;
        int dc = c1 - c0;
        if (dc==0 && (dr==dir || dr==2*dir)){
            int step = (dr>0)? +1 : -1;
            for (int rr=r0+step; rr!=r1+step; rr+=step){
                set_sq(rr,c0, cr, cg, cb);
            }
        }
    } break;

    case PIECE_KNIGHT:
        if (is_knight_target(r0,c0,r1,c1)){
            set_sq(r1,c1, cr, cg, cb);
        }
        break;

    case PIECE_BISHOP:
        if (same_diag(r0,c0,r1,c1)){
            int dr = (r1>r0)? +1 : -1;
            int dc = (c1>c0)? +1 : -1;
            draw_line_dir(r0,c0, dr,dc, r1,c1, cr,cg,cb);
        }
        break;

    case PIECE_ROOK:
        if (same_rank(r0,c0,r1,c1)){
            int dc = (c1>c0)? +1 : -1;
            draw_line_dir(r0,c0, 0,dc, r1,c1, cr,cg,cb);
        } else if (same_file(r0,c0,r1,c1)){
            int dr = (r1>r0)? +1 : -1;
            draw_line_dir(r0,c0, dr,0, r1,c1, cr,cg,cb);
        }
        break;

    case PIECE_QUEEN:
        if (same_diag(r0,c0,r1,c1)){
            int dr = (r1>r0)? +1 : -1;
            int dc = (c1>c0)? +1 : -1;
            draw_line_dir(r0,c0, dr,dc, r1,c1, cr,cg,cb);
        } else if (same_rank(r0,c0,r1,c1)){
            int dc = (c1>c0)? +1 : -1;
            draw_line_dir(r0,c0, 0,dc, r1,c1, cr,cg,cb);
        } else if (same_file(r0,c0,r1,c1)){
            int dr = (r1>r0)? +1 : -1;
            draw_line_dir(r0,c0, dr,0, r1,c1, cr,cg,cb);
        }
        break;

    case PIECE_KING: {
        int dr = r1 - r0, dc = c1 - c0;
        if (dr<0) dr=-dr; if (dc<0) dc=-dc;
        if (dr<=1 && dc<=1 && (dr+dc)>0){
            set_sq(r1,c1, cr, cg, cb);
        }
    } break;
    }
}

void ledchess_draw_move(ledchess_piece_t piece, const char from[2], const char to[2],
                        uint8_t cr, uint8_t cg, uint8_t cb,
                        ledchess_side_t pawn_dir)
{
    int r0,c0,r1,c1;
    if (ledchess_sq_to_rc(from,&r0,&c0)!=0) return;
    if (ledchess_sq_to_rc(to,&r1,&c1)!=0) return;
    ledchess_draw_move_rc(piece, r0,c0, r1,c1, cr,cg,cb, pawn_dir);
}

void ledchess_draw_pawn_push(const char from[2], int steps, uint8_t r, uint8_t g, uint8_t b,
                             ledchess_side_t pawn_dir)
{
    int r0,c0; if (ledchess_sq_to_rc(from,&r0,&c0)!=0) return;
    int r1 = r0 + ((int)pawn_dir) * steps;
    int c1 = c0;
    if (!inb(r1,c1)) return;
    ledchess_draw_move_rc(PIECE_PAWN, r0,c0, r1,c1, r,g,b, pawn_dir);
}

//wysyłanie buforu z opóźnieniem
void leds_show_and_pause(uint32_t ms)
{
    while (ws2812_busy()) {}
    ws2812_show_async();
    while (ws2812_busy()) {}
    HAL_Delay(ms);
}



