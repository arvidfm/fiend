#include <allegro.h>

#ifndef ROTATE_SPRITE_H
#define ROTATE_SPRITE_H

#include "draw_define.h"

void rotate_fiend_sprite(BITMAP *dest, BITMAP * src, int dest_x, int dest_y, float angle, int draw_mode);

void pivot_fiend_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int cx, int cy, float angle, int draw_mode);


#endif 


