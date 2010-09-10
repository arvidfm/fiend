

#ifndef PLAYER_H
#define PLAYER_H


#define PLAYER_USE_H 60
#define PLAYER_USE_W 60
#define PLAYER_USE_LENGTH 20

#define PLAYER_PICKUP_H 50
#define PLAYER_PICKUP_W 50
#define PLAYER_PICKUP_LENGTH 20

#define MAX_ITEM_SPACES 200
#define MAX_AMMO_SPACES 20
#define MAX_WEAPON_SPACES 20
#define MAX_NOTE_SPACES 200

typedef struct
{
	int item;
	int num;
	int active;
}PLAYER_ITEM;


typedef struct
{
	float x;//the cords
	float y;
	float angle;//the angle
	
	int action;//the current action
	int frame;//the current frame in an  action
	int nextframe;//How long until next frame....
	
	int active;//Is the player active (can you move him)

	int dead;
	int was_dead;
	
	float hit_speed;
	float hit_angle;

	int energy;
	int active_weapon;

	int weapon_drawn;

	int reloading;
	int running;

	int animation;
	
	float last_dx;
	float last_dy;

	int can_shoot;
	
	int num_of_items;
	PLAYER_ITEM item_space[MAX_ITEM_SPACES];
	int num_of_ammo;
	PLAYER_ITEM ammo_space[MAX_AMMO_SPACES];
	int num_of_weapons;
	PLAYER_ITEM weapon_space[MAX_WEAPON_SPACES];
	int num_of_notes;
	PLAYER_ITEM note_space[MAX_NOTE_SPACES];

}PLAYER_INFO;


extern PLAYER_INFO player;

extern int player_has_used;
extern int player_use_x;
extern int player_use_y;

extern int player_has_used_item;
extern int player_item_x;
extern int player_item_y;
extern int player_item_num;

void change_player_action(char *name);
void update_global_keys(void);
void clear_player_data(void);



#endif