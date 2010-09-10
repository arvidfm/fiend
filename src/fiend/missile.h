#include <allegro.h>


#ifndef MISSILE_H
#define MISSILE_H 


void update_missiles(void);
void make_new_missile(int x, int y, float angle, int type);
void reset_missiles(void);
void draw_missiles(void);

#endif