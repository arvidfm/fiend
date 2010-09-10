#include <allegro.h>


#ifndef LOS_H
#define LOS_H


void clear_los_buffer(void);
int object_is_in_player_los(float x, float y, int w, int h, float angle, int solid);
void update_los_buffer(int xpos, int ypos);
void draw_los_buffer(BITMAP *dest, int xpos, int ypos);
void make_los_borders(void);



#endif