#include <allegro.h>


#ifndef NPC_H
#define NPC_H

#define MAX_NPC_PATHS_NUM 5
#define MAX_NPC_PATH_POINTS_NUM 10


#define MAX_DIALOG_NUM 12
#define MAX_DIALOG_TEXT_NUM 30
#define MAX_DIALOG_TEXT_LENGTH 120

#define MAX_NPC_PATH_NUM 5

#define MAX_NPC_NUM 20

#include "character.h"

typedef struct
{
	int x;
	int y;
}POINT_DATA;



typedef struct
{
	int type;//0=don't walk    1=turn at end    2=walk to end and stop  3=loop
	int num_of_points;
	POINT_DATA point[MAX_NPC_PATH_POINTS_NUM];
}MOVEMENT_PATH_DATA;


typedef struct
{
	char string[MAX_DIALOG_TEXT_LENGTH];
	char face[40];
}NPC_TEXT_DATA;

typedef struct
{
	int num_of_texts;
	NPC_TEXT_DATA text[MAX_DIALOG_TEXT_NUM];
	int next_dialog; //what dialog is it next time you speak with the npc...-1 = the same
}DIALOG_DATA;



typedef struct
{
	int used;
	
	int active;

	int dead;
	int was_dead;

	int type;

	char name[40];
	char map_name[40];
	
	float x;
	float y;
	float angle;

	int energy;
	int action;
	int frame;
	int next_frame;

	int sp1;
	int sp2;
	int sp3;

	int current_movement_path;
	MOVEMENT_PATH_DATA movement_path[MAX_NPC_PATHS_NUM];
	
	int current_dialog;
	DIALOG_DATA dialog[MAX_DIALOG_NUM];
}NPC_DATA;




int init_npc_data(void);
int load_npc(void);
int save_npc(void);
void release_npc(void);

extern int num_of_npc;
extern NPC_DATA *npc_data;

#endif
