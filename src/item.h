#include <allegro.h>

#ifndef ITEM_H
#define ITEM_H 



#define MAX_ITEM_DATA 200
#define MAX_ITEM_INFO 70


#include "bmp_array.h"

typedef struct
{
	char name[20];
	int w;
	int h;

	int type; //0 = normal be picked up 1 = Weapon 2 = Ammo 3 = note,book
	
	int num; //number in the item...or something
	char s_string[20];
	char desc[160];
}ITEM_INFO;



typedef struct
{
	int used;
	
	int active;
	int picked_up;

	int type;

	char map_name[40];
	char name[30];

	char string[40];//used for diffrent stuff the name of a text if item_info.type = 4
	
	float x;
	float y;

	int value;
	int sp1;

	float angle;

	char desc[160];
}ITEM_DATA;



void release_items(void);
int load_items(void);

int init_item_data(void);
int load_item_data(void);
int save_item_data(void);
void release_item_data(void);

void draw_fiend_item(BITMAP *dest,int num,int x, int y,float angle, int lit);


extern int num_of_items;
extern ITEM_DATA *item_data;
extern ITEM_INFO *item_info;
extern RLE_ARRAY *item_pic;
extern DATAFILE *big_item_pic;


#endif