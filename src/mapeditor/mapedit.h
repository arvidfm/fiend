#include <allegro.h>


#ifndef MAPEDIT_H
#define MAPEDIT_H


#define EDITMODE_TILES 0
#define EDITMODE_OBJECTS 1
#define EDITMODE_ITEMS 2
#define EDITMODE_NPCS 3
#define EDITMODE_LIGHTS 4
#define EDITMODE_PLAYER 5
#define EDITMODE_LINKS 6
#define EDITMODE_AREAS 7
#define EDITMODE_LOOK_AT_AREAS 8
#define EDITMODE_SOUNDEMITORS 9
#define EDITMODE_ENEMY 10
#define EDITMODE_PATHNODE 11
#define EDITMODE_BRUSH 12
#define EDITMODE_SHADOWS 13
#define EDITMODE_ENEMY_PATH 14
#define EDITMODE_NPC_PATH 15

#define MAX_BRUSH_NUM 20
#define MAX_BRUSH_SIZE 10

#define LIMK_MESSAGE_LENGTH 160
 

typedef struct
{
	int tile_set; //The tileset
	int tile_num; //the tile in the set}
}BRUSH_TILE;

typedef struct
{
	char file[80];
	char name[20];
	int w;
	int h;
	BRUSH_TILE data[MAX_BRUSH_SIZE][MAX_BRUSH_SIZE];
}BRUSH_DATA;

extern BRUSH_DATA brush[MAX_BRUSH_NUM];
extern int brush_chosen;


extern char save_path[100];
extern char load_path[100];



extern int current_tileset;
extern int current_tile;
extern int current_shadow;
extern int current_layer;
extern int current_editmode;

extern int current_look_at_area;

extern int current_character;
extern int current_object;
extern int current_pathnode;

extern int x_grid;
extern int y_grid;

extern int selected;

extern DIALOG main_dialog[];

extern void sort_objects(void);
extern void sort_tiles(void);

extern DATAFILE *misc;


#endif