#include <allegro.h>

#ifndef CHARACTER_H
#define CHARACTER_H

#include "picdata.h" 

#define MAX_CHARACTERS 40

typedef struct
{
	char name[20];
	int frame[60];
}CHAR_ANIMATION_DATA;

typedef struct
{
	char name[30];
	int w;
	int h;
	int hit_w;
	int hit_h;
	int pic_size;
	int angles;
	
	int energy;
	float speed;
	int weight;
	char sound_death[40];
	char sound_hurt[40];
	char sound_ambient[40];

	float run_add;

	int blood_x;
	int blood_y;
	
	int walk_step1;
	int walk_step2;
	int run_step1;
	int run_step2;

	int num_of_frames;
	int num_of_animations;
	CHAR_ANIMATION_DATA animation[15];
	PIC_DATA pic[100];
}CHARACTER_INFO;



#endif