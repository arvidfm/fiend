#include <allegro.h>

#ifndef OBJECT_H
#define OBJECT_H

#include "picdata.h"

typedef struct
{
	char name[20];
	int solid;
	char sound[50];
	int loop_sound;
	int frame[30];
}OBJECT_ANIMATION_DATA;



typedef struct
{
	char name[20];
	int w;
	int h;
	int pic_size;
	int angles; // 0 = no angles 1 = 4 angles 2 = 8 angles
	int can_be_pushed;
	int can_be_killed;
	int energy; //| On solid=1 : <0 = object is below tiles >=0 = object is above tiles | On doors : 0 = unlocked 1 = locked
	int solid; //0=under the player and not solid 1=under the player and solid 2=over the player and solid 3=over the player and blocks view
	int door; //0= no door 1 = turning door 180 2 = turning door 90 3 = sliding door
	int door_x;
	int door_y;
	int additive;
	int trans;
	char folder[20];
	int num_of_frames;
	int num_of_animations;
	OBJECT_ANIMATION_DATA animation[5];
	
	PIC_DATA pic[30][8];
	RLE_SPRITE *rle_pic[30][8];
}OBJECT_INFO;






typedef struct
{
 int type;	
 char name[20];
 int x;
 int y;
 float angle;
 int action;
 int frame;
 int nextframe;
 int active;
 int energy;
 int voice_num;
 int save_object;
 int pushable;
}OBJECT_DATA;




extern OBJECT_INFO *object_info;
extern int num_of_objects;

void update_door_objects(void);

void draw_fiend_object(BITMAP *dest,OBJECT_INFO *temp,int x, int y,int action,int frame, float angle);

void update_objects(void);
void change_object_action(int o_num,int a_num, int with_sound);

int load_objects(void);
void release_objects(void);


#endif