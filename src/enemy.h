#include <allegro.h>

#ifndef ENEMY_H
#define ENEMY_H 

#include "picdata.h"


#define MAX_ENEMY_PATHS_NUM 5
#define MAX_ENEMY_PATH_POINTS_NUM 10

#define MAX_ENEMY_PATH_NUM 5

#define MAX_ENEMY_DATA 300
#define MAX_ENEMY_INFO 50


#define MISSION_NUM 4


#define MISSION_PLAYER 0
#define MISSION_NPC 1
#define MISSION_PATROL 2
#define MISSION_RUN 3
 

typedef struct
{
	int x;
	int y;
}E_POINT_DATA;



typedef struct
{
	int type;//0=don't walk    1=turn at end    2=walk to end and stop  3=loop
	int num_of_points;
	E_POINT_DATA point[MAX_ENEMY_PATH_POINTS_NUM];
}E_MOVEMENT_PATH_DATA;




typedef struct
{
	char name[20];
	int frame[120];
}ENEMY_ANIMATION_DATA;

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
	char blood_particle[20];
		
	int eye_type; // 0=Normal 1=Motion based 2=Infrared
	int can_smell;
	int can_hear;
	int hear_range;

	int run_when_hurt;
	int attack_from_behind;
	int attack_in_group;
	int use_a_star;
	int care_if_hurt;
	int fov;
	
	char weapon_name[3][30];
	int weapon_range[3];
	int weapon_random[3]; 
	int weapon_length[3]; 

	int move_random;
	int move_random_length;
	int move_random_time;
	
	char death_explosion[20];
	int death_explosion_num;

	int blood_x;
	int blood_y;
	int blood_color;//written in hexadecimal rrggbb
	int show_bloodpool;

	int under_player;
	int friendly;
	float regenerate;

	char sound_death[40];
	char sound_hurt[40];
	
	char sound_ambient[5][40];
	int sound_ambient_num;
	int sound_ambient_random[5];

	char step_sound[30];
	float run_add;
	int walk_step1;
	int walk_step2;
	int run_step1;
	int run_step2;

		
	int num_of_frames;
	int num_of_animations;
	ENEMY_ANIMATION_DATA animation[15];
	PIC_DATA pic[100];
}ENEMY_INFO;



typedef struct
{
	int used;
	
	int active;

	int dead;
	int was_dead;
	
	int disappear;

	int type;

	char name[40];
	char map_name[40];
	
	float x;
	float y;
	float angle;
	
	float energy;
	int action;

	int current_mission;

	int frame;
	int nextframe;

	int sp1;
	int sp2;
	int sp3;

	int current_movement_path;
	E_MOVEMENT_PATH_DATA movement_path[MAX_ENEMY_PATHS_NUM];
}ENEMY_DATA;



void release_enemys(void);
int load_enemys(void);

int init_enemy_data(void);
int load_enemy_data(void);
int save_enemy_data(void);
void release_enemy_data(void);

void draw_fiend_enemy(BITMAP *dest,ENEMY_INFO *temp,int x, int y,int action,int frame, float angle);

int enemy_action(ENEMY_INFO *temp, char *name);


extern int num_of_enemys;
extern ENEMY_DATA *enemy_data;
extern ENEMY_INFO *enemy_info;


#endif