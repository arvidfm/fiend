

#ifndef EFFECT_H
#define EFFECT_H

#define MAX_EFFECT_NUM 200

#define EFFECT_QUAKE 0
#define EFFECT_RAIN 1
#define EFFECT_LIGHTLEVEL_FADE 2
#define EFFECT_OBJECT_MOVE 3
#define EFFECT_LIGHT_MOVE 4
#define EFFECT_FADEOUT 5
#define EFFECT_FADEIN 6
#define EFFECT_THUNDER 7



typedef struct
{
	int used;
	float x;
	float y;
	int z;
	int num;
	int time;
	int type;
	float sp1;
	float sp2;
}EFFECT_DATA;

EFFECT_DATA effect_data[MAX_EFFECT_NUM];


extern int screen_is_black;

void make_effect(int type,int num,int x, int y,int z,int time);
void reset_effects(void);
void update_effects(void);
void draw_effects(void);

void fiend_fadeout(int speed);
void fiend_fadein(int speed);


#endif 


