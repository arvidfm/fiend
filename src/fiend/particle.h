#include <allegro.h>


#ifndef PARTICLE_H
#define PARTICLE_H

#define MAX_PARTICLE_INFO 50
#define MAX_PARTICLE_DATA 800


typedef struct
{
	char name[30];
	int aa; //1 if antialiased
	int trans; //1 if transarent
	int trans_alpha; //the alpha of the trans
	int rotate; // if 1 then rotate, doesn't work if trans or aa
	int num_of_frames;
	int  anim_frame[30];
	BMP_ARRAY *pic;
}PARTICLE_INFO;


typedef struct
{
	int used;
	int type;
	int blood;
	int color;
	float alpha;
	float x;
	float y;
	float angle;
	float speed;
	float speed_dec;
	int time;
	int level;//0 = below all 1 = below player 2 = over player 3 = over the ligtmap
	int frame;
	int next_frame;
}PARTICLE_DATA;


extern int num_of_particles;

extern PARTICLE_INFO *particle_info;
extern PARTICLE_DATA *particle_data;


void reset_particles(void);


int load_particles(void);
void release_particles(void);

void make_new_particle(char *name, float x, float y, float angle, float speed,float speed_dec, int time, int level, int color);
void update_particles(void);
void draw_particles(int level);


#endif

 