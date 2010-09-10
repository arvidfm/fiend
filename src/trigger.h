#include <allegro.h>


#ifndef TRIGGER_H
#define TRIGGER_H


#define MAX_CONDITION_NUM 5
#define MAX_EVENT_NUM 5
#define LOCAL_VAR_NUM 20
#define GLOBAL_VAR_NUM 250
#define MAX_TRIGGER_NUM 50
#define GLOBAL_TRIGGER_NUM 100


#define COND_NUM 31

#define COND_PLAYER_AREA 1
#define COND_PLAYER_ENERGY 2
#define COND_PLAYER_TRIGGER_OBJECT 3
#define COND_PLAYER_TRIGGER_AREA 4
#define COND_PLAYER_TRIGGER_ENEMY 5
#define COND_PLAYER_USE_ITEM_OBJECT 6
#define COND_PLAYER_USE_ITEM_NPC 7
#define COND_PLAYER_USE_ITEM_AREA 8
#define COND_PLAYER_USE_ITEM_ENEMY 9
#define COND_HAS_ITEM 10
#define COND_ENEMY_ENERGY 11
#define COND_ENEMY_MISSION 12
#define COND_ENEMY_AREA 13
#define COND_NPC_ENERGY 14
#define COND_NPC_AREA 15
#define COND_OBJECT_ENERGY 16
#define COND_OBJECT_AREA 17
#define COND_LIGHTLEVEL 18
#define COND_GLOBAL_VAR 19
#define COND_LOCAL_VAR 20

#define COND_ITEM_PICKED 21
#define COND_ITEM_ABLED 22
#define COND_NPC_ABLED 23
#define COND_ENEMY_ABLED 24
#define COND_OBJECT_ABLED 25
#define COND_AUTOMOVE 26
#define COND_MESSAGE_ACTIVE 27
#define COND_RANDOM 28

#define COND_ALWAYS 29

#define COND_NPC_DIALOG 30
#define COND_MAP_OUTSIDE 31




#define EVENT_NUM 86

#define EVENT_PLAYER_ENERGY_BECOME 1
#define EVENT_PLAYER_ENERGY_ADD 2
#define EVENT_PLAYER_POS 3
#define EVENT_PLAYER_MAP 4
#define EVENT_PLAYER_ITEM 5
#define EVENT_PLAYER_ACTIVATE_BECOME 6
#define EVENT_PLAYER_ANGLE 7
#define EVENT_PLAYER_ACTION 8


#define EVENT_ENEMY_ENERGY_BECOME 9
#define EVENT_ENEMY_ENERGY_ADD 10
#define EVENT_ENEMY_POS 11
#define EVENT_ENEMY_MAP 12
#define EVENT_ENEMY_KNOW_PLAYER 13
#define EVENT_ENEMY_ACTIVATE_BECOME 14
#define EVENT_ENEMY_DEAD_BECOME 15
#define EVENT_ENEMY_ACTION 16
#define EVENT_ENEMY_MISSION 17
#define EVENT_ENEMY_RUN 18
#define EVENT_ENEMY_PATH 19

#define EVENT_NPC_ENERGY_BECOME 20
#define EVENT_NPC_ENERGY_ADD 21
#define EVENT_NPC_POS 22
#define EVENT_NPC_MAP 23
#define EVENT_NPC_SOUND 24
#define EVENT_NPC_ACTIVATE_BECOME 25
#define EVENT_NPC_ACTIVATE_TOGGLE 26
#define EVENT_NPC_ACTION 27
#define EVENT_NPC_DIALOG 28
#define EVENT_NPC_PATH 29
#define EVENT_NPC_AI 30

#define EVENT_OBJECT_ENERGY_BECOME 31
#define EVENT_OBJECT_ENERGY_ADD 32
#define EVENT_OBJECT_POS 33
#define EVENT_OBJECT_GO_TO_POS 34
#define EVENT_OBJECT_ACTIVATE_BECOME 35
#define EVENT_OBJECT_ACTIVATE_TOGGLE 36
#define EVENT_OBJECT_ACTION 37

#define EVENT_ITEM_POS 38
#define EVENT_ITEM_ACTIVATE_BECOME 39
#define EVENT_ITEM_ACTIVATE_TOGGLE 40

#define EVENT_AREA_POS 41
#define EVENT_AREA_ACTIVATE_BECOME 42
#define EVENT_AREA_ACTIVATE_TOGGLE 43

#define EVENT_LINK_EVENT 44

#define EVENT_LIGHT_POS 45
#define EVENT_LIGHT_GO_TO_POS 46
#define EVENT_LIGHT_ACTIVATE_BECOME 47
#define EVENT_LIGHT_ACTIVATE_TOGGLE 48
#define EVENT_LIGHT_FLASH_BECOMES 49
#define EVENT_LIGHT_FLASH_TOGGLES 50


#define EVENT_LIGHTLEVEL_BECOME 51
#define EVENT_LIGHTLEVEL_ADD 52
#define EVENT_LIGHTLEVEL_FADE 53

#define EVENT_SHOW_MESSAGE 54
#define EVENT_SHOW_IMAGE 55

#define EVENT_MAKE_EXPLOSION 56
#define EVENT_MAKE_EARTHQUAKE 57
#define EVENT_MAKE_BLOODPOOL 58
#define EVENT_MAKE_BEAM 59
#define EVENT_MAKE_PICKUP 60
#define EVENT_MAKE_THUNDER 61
#define EVENT_MAKE_RAIN 62
#define EVENT_MAKE_XXX 63

#define EVENT_SOUNDEMITOR_POS 64 
#define EVENT_SOUNDEMITOR_ACTIVATE_BECOME 65
#define EVENT_SOUNDEMITOR_ACTIVATE_TOGGLE 66

#define EVENT_PLAY_SOUND 67
#define EVENT_PLAY_MUSIC 68
#define EVENT_STOP_SOUND 69
#define EVENT_STOP_MUSIC 70

#define EVENT_GLOBAL_VAR_BECOME 71
#define EVENT_GLOBAL_VAR_ADD 72
#define EVENT_LOCAL_VAR_BECOME 73
#define EVENT_LOCAL_VAR_ADD 74

#define EVENT_AUTOMOVE_POS 75
#define EVENT_AUTOMOVE_PAUSE 76
#define EVENT_AUTOMOVE_RESUME 77
#define EVENT_AUTOMOVE_TOGGLE 78
#define EVENT_AUTOMOVE_STOP 79

#define EVENT_FADEOUT 80
#define EVENT_FADEIN 81

#define EVENT_SAVE 82

#define EVENT_OUTSIDE_LIGHT 83

#define EVENT_QUIT_TO_MENU 84
#define EVENT_COMPLETE_GAME 85


#define EVENT_NOTHING 86





typedef struct
{
	int value;
	char name[40];
}VARIABLE_DATA;


typedef struct
{
	char used;
	int type;
	char x[40];
	int correct;
	char z[40];
	char string1[40];
	char string2[40];
	char logic; //0=exact(==)//1=greater(>)//2=lesser(<)
}CONDITION_DATA;


typedef struct
{
	char used;
	int type;
	char x[40];
	char y[40];
	char z[40];
	char string1[40];
	char string2[40];
	char text[160];
	char logic; //0=exact(==)//1=greater(>)//2=lesser(<)
}EVENT_DATA;


typedef struct 
{
	char name[30];
	int active;
	int type; //0=Check only at start up // 1=Check only once //2=Always check
	CONDITION_DATA condition[MAX_CONDITION_NUM];
	EVENT_DATA event[MAX_EVENT_NUM];
}TRIGGER_DATA;


extern VARIABLE_DATA *global_var;
extern TRIGGER_DATA *global_trigger;

#define MAX_ROOM_NUM 100

typedef struct 
{
	int used;
	int value;
}SAVED_VAR;

typedef struct 
{
	SAVED_VAR var[LOCAL_VAR_NUM];
	char name[40];
}SAVED_LOCAL_VAR;

typedef struct 
{
	int x;
	int y;
	float angle;
	int action;
	int frame;
	int nextframe;
	int active;
	int energy;
	char name[40];
	int type;
}SAVED_OBJECT;

typedef struct 
{
	SAVED_OBJECT object[200];
	char name[40];
}SAVED_ROOM_OBJECT;

extern int saved_var_num;
extern SAVED_LOCAL_VAR saved_local_var[MAX_ROOM_NUM];

extern int saved_object_num;
extern SAVED_ROOM_OBJECT saved_object[MAX_ROOM_NUM];


int get_area_num(char *name);
int get_object_num(char *name);
int get_enemy_num(char *name);
int get_link_num(char *name);
int get_npc_num(char *name);
int get_global_var_num(char *name);
int get_local_var_num(char *name);
int get_item_num(char *name);

int get_var_value(char *string);

void check_triggers(int type);

int init_global_vars(void);
int load_global_vars(void);
int save_global_vars(void);
void release_global_vars(void);

int init_global_triggers(void);
int load_global_triggers(void);
int save_global_triggers(void);
void release_global_triggers(void);

void reset_saved_vars(void);
void save_local_vars(void);
int load_local_vars(void);

#endif






