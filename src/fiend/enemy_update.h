#ifndef ENEMY_UPDATE_H
#define ENEMY_UPDATE_H 

	
#define  FOUND_LENGTH 1200
#define  SPEAK_LENGTH 120

typedef struct 
{
	int current_point;
	int next_point;
	int last_point;

	float wanted_angle;

	int walking_random;
	int random_count;
	
	int running;
	int run_x;
	int run_y;

	float hit_speed;
	float hit_angle;
	
	int attacking;
	int attack_num;
	int can_attack;
	int weapon_was_shot;
	int release_shell;
	
	int have_seen_player;
	
	int found_player;
	int last_player_x;
	int last_player_y;
	
	int last_dx;
	int last_dy;

	int speaking;

	int dead_and_still;
	
	int damage_taken;
	int damage_count;

	int animation;
}ENEMY_AI_DATA;

ENEMY_AI_DATA enemy_ai[MAX_ENEMY_DATA];


void update_enemy(void);

void reset_enemy_ai(void);
void total_reset_enemy_ai(void);
void reset_enemy_data(void);
void change_enemy_action(char *name, int num);


#endif