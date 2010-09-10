#ifndef AI_H
#define AI_H

void init_path_nodes(void);

float get_best_npc_angle(float start_x,float  start_y,float  goal_x,float  goal_y,int num);
float get_best_enemy_angle(float start_x,float  start_y,float  goal_x,float  goal_y,int num,int check_player);

void update_tile_object_height(void);

int object_is_in_fov(float eye_x, float eye_y,float eye_angle, float x, float y, int w, int h, float fov, int corners);
int path_is_clear(float eye_x, float eye_y,float eye_angle, float x, float y, int solid, int check_player);


#endif

