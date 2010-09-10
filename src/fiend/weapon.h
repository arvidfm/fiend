#include <allegro.h>


#ifndef WEAPON_H
#define WEAPON_H 

#define MAX_MISSILES 300
#define MAX_WEAPONS 40 


typedef struct
{
	
	///// weapon props ///////
	char name[30];
	char player_action[30];

	int weight;
	
	int flame_num;
	int flame_length;
	int flame_x;
	int flame_y;
	
	int light_r;
	int light_c;

	char sound[30];
	int shot_length;
	int silent;

	int max_ammo;

	int num_of_missiles;
	float missile_angle;

	int missile_x;
	int missile_y;

	char shell_name[20];
	
	int shell_x;
	int shell_y;

	int shell_angle;
	int shell_after_shot_length;
	
	////// the weapons missile props ///////

	char missile_name[20];

	float speed;
	
	int range;
	float range_dec;

	int beam;

	int min_damage;
	int max_damage;
	int damage_range;
	float damage_range_dec;

	char explosion_name[20];
	int num_of_explosions;

	char particle_name[20];
	int particle_num;

	int strength;
	
	int missile_light_r;
	int missile_light_c;

	int hit_enemy;

}WEAPON_INFO;




typedef struct
{
	int used;

	int dist;

	float speed;

	int start_x;
	int start_y;
	
	float x;
	float y;

	float angle;

	float min_damage;
	float max_damage;
	
	int type;
	int p_type;
		
	int frame;
	int next_frame;
}MISSILE_DATA;

extern WEAPON_INFO *weapon_info;

extern MISSILE_DATA *missile_data;

extern int num_of_weapons;

int load_weapons(void);
void release_weapons(void);

int get_weapon_num(char *name);

void shoot_weapon(char *name, float x, float y, float angle);

#endif