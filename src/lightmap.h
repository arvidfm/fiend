#include <allegro.h>


#ifndef LIGHTMAP_H
#define LIGHTMAP_H

#define LIGHTMAP_C_NUM 4
#define LIGHTMAP_R_NUM 8

#define NORMAL_LIGHT_NUM 40

#define WALL_SHADOW_DOWN 0
#define WALL_SHADOW_RIGHT 1
#define WALL_SHADOW_HALF_DOWN 2
#define WALL_SHADOW_HALF_RIGHT 3
#define WALL_SHADOW_CORNER 4
#define WALL_SHADOW_FULL 5


typedef struct
{
 int bitmap_w;//w and h of bitmap;
 int bitmap_h;
 
 int strech_w;//if not equal to bitmap then strech the sprite...
 int strech_h;
 
 int x;//x and y pos light in bitmap
 int y;
 
 int r;//the radie
 int centre_r; // the centre size

 int max_light;// The light amount in the centre
 
 int flash;//if 1 than light flashes on and of

 int active;//The angle of the light

 int world_x;//world x and y
 int world_y;

}LIGHT_DATA;



typedef struct 
{
	int used;

	int x;
	int y;
	
	int c;
	int r;

	int time;
}NORMAL_LIGHT_DATA;

extern int num_of_wall_shadows;

extern NORMAL_LIGHT_DATA *normal_light_data;


void create_shadows(void);
void create_normal_lightmaps(void);

void create_wall_shadows(void);
void draw_wall_shadows(BITMAP *dest,int xpos,int ypos);
int tile_is_wall_solid(int x, int y);


extern BITMAP *char_shadow[];
extern BITMAP *enemy_shadow[];

extern BITMAP *wall_shadow[];

extern BITMAP *normal_lightmap[LIGHTMAP_C_NUM][LIGHTMAP_R_NUM];

void make_normal_light(int x, int y, int c, int r, int time);
void update_normal_light(void);
void draw_normal_light(BITMAP* dest, int num);

#endif