#ifndef NPC_UPDATE_H
#define NPC_UPDATE_H

typedef struct 
{
	int current_point;
	int next_point;
	int last_point;
	float wanted_angle;
	int panic;
	int panic_x;
	int panic_y;
	float hit_speed;
	float hit_angle;
	int dead_and_still;

	int animation;
}NPC_AI_DATA;

NPC_AI_DATA npc_ai[];


void update_npc(void);
void reset_npc_ai(void);
void total_reset_npc_ai(void);
void reset_npc_data(void);

void change_npc_action(char *name, int num);


#endif