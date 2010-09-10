////////////////////////////////////////////////////
// This file contains alldiffrent lightmap generation
// funcs.  
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include "grafik4.h"
#include "fiend.h"
#include "lightmap.h"
#include "draw.h"

#define S_SHADOW_LENGTH (4)

#define S_SHADOW_MIN (4)

#define S_SHADOW_END (31)

#define WALL_SHADOW_MIN (16)

#define WALL_SHADOW_MIN_LENGTH (20)

#define WALL_SHADOW_NUM (25)


//defines for custom lightmaps
#define LIGHTMAP_C_NUM 4
#define LIGHTMAP_R_NUM 8

#define LIGHTMAP_MAX_R 200

//shadows below characters
BITMAP *char_shadow[MAX_CHARACTERS];
BITMAP *enemy_shadow[MAX_ENEMY_INFO];

//the wall shadows
BITMAP *wall_shadow[WALL_SHADOW_NUM];

//the normal/costum lightmaps
BITMAP *normal_lightmap[LIGHTMAP_C_NUM][LIGHTMAP_R_NUM];
BITMAP *normal_lightmap[LIGHTMAP_C_NUM][LIGHTMAP_R_NUM];

//vars used for normal light map making
static int lightmap_c[LIGHTMAP_C_NUM]={80,140,200,255};
static int lightmap_r[LIGHTMAP_R_NUM]={20,40,80,120,180,220,280,340};


NORMAL_LIGHT_DATA *normal_light_data;

//not used....
BITMAP *lightmap_cache;


//Wall shadow stuff....

int num_of_wall_shadows= WALL_SHADOW_NUM;


//Create the gfx for the differnt wall shadows...
void create_wall_shadows(void)
{
	int i;
	float add;
	float col;
	
	//get the memeory
	for(i=0;i<WALL_SHADOW_NUM;i++)
		wall_shadow[i] = create_bitmap_ex(8,32,32);

	
	//Straight shadow
	for(i=0;i<4;i++)
		clear_to_color(wall_shadow[i],WALL_SHADOW_MIN);

	add = (31 - WALL_SHADOW_MIN)/(32-WALL_SHADOW_MIN_LENGTH);
	col = WALL_SHADOW_MIN;

	//Do the vertical
	for(i=WALL_SHADOW_MIN_LENGTH;i<32;i++)
	{
		col+=add;
		line(wall_shadow[0],0,i,31,i,col);
	}

	draw_sprite_v_flip(wall_shadow[1],wall_shadow[0],0,0);
	
	add = (31 - WALL_SHADOW_MIN)/(32-WALL_SHADOW_MIN_LENGTH);
	col = WALL_SHADOW_MIN;

	//Do the horisontal
	for(i=WALL_SHADOW_MIN_LENGTH;i<32;i++)
	{
		col+=add;
		line(wall_shadow[2],i,0,i,31,col);
	}

	draw_sprite_h_flip(wall_shadow[3],wall_shadow[2],0,0);
	


	//do the slice
	for(i=4;i<8;i++)
		clear_to_color(wall_shadow[i],WALL_SHADOW_MIN);
	
	add = (31 - WALL_SHADOW_MIN)/(32-WALL_SHADOW_MIN_LENGTH);
	col = WALL_SHADOW_MIN;

	for(i=WALL_SHADOW_MIN_LENGTH;i<32;i++)
	{
		col+=add;
		line(wall_shadow[4],31-i,0,31,i,col);
	}
	for(i=0;i<32;i++)
		line(wall_shadow[4],0,31-i,i,31,31);

	draw_sprite_h_flip(wall_shadow[5],wall_shadow[4],0,0);
	draw_sprite_v_flip(wall_shadow[6],wall_shadow[5],0,0);
	draw_sprite_h_flip(wall_shadow[7],wall_shadow[6],0,0);
	

	
	//do the corner
	for(i=8;i<12;i++)
		clear_to_color(wall_shadow[i],WALL_SHADOW_MIN);

	add = (31 - WALL_SHADOW_MIN)/(32-WALL_SHADOW_MIN_LENGTH);
	col = WALL_SHADOW_MIN;

	for(i=WALL_SHADOW_MIN_LENGTH;i<32;i++)
	{
		col+=add;
		line(wall_shadow[8],0,i,WALL_SHADOW_MIN_LENGTH+i-WALL_SHADOW_MIN_LENGTH,i,col);
		line(wall_shadow[8],i,0,i,WALL_SHADOW_MIN_LENGTH+i-WALL_SHADOW_MIN_LENGTH,col);
	}
	draw_sprite_h_flip(wall_shadow[9],wall_shadow[8],0,0);
	draw_sprite_v_flip(wall_shadow[10],wall_shadow[9],0,0);
	draw_sprite_h_flip(wall_shadow[11],wall_shadow[10],0,0);
	
	
	//do the full corner
	for(i=12;i<16;i++)
		clear_to_color(wall_shadow[i],WALL_SHADOW_MIN);

	add = (31 - WALL_SHADOW_MIN)/(32-WALL_SHADOW_MIN_LENGTH);
	col = WALL_SHADOW_MIN;

	for(i=WALL_SHADOW_MIN_LENGTH;i<32;i++)
	{
		col+=add;
		line(wall_shadow[12],i,i,31,i,col);
		line(wall_shadow[12],i,i,i,31,col);
	}

	draw_sprite_h_flip(wall_shadow[13],wall_shadow[12],0,0);
	draw_sprite_v_flip(wall_shadow[14],wall_shadow[13],0,0);
	draw_sprite_h_flip(wall_shadow[15],wall_shadow[14],0,0);
	
	
	//do the  full slice corner
	for(i=16;i<20;i++)
		clear_to_color(wall_shadow[i],WALL_SHADOW_MIN);

	add = (31 - WALL_SHADOW_MIN)/(32-WALL_SHADOW_MIN_LENGTH);
	col = WALL_SHADOW_MIN;

	for(i=WALL_SHADOW_MIN_LENGTH;i<32;i++)
	{
		col+=add;
		line(wall_shadow[16],31,i,i,31,col);
	}

	draw_sprite_h_flip(wall_shadow[17],wall_shadow[16],0,0);
	draw_sprite_v_flip(wall_shadow[18],wall_shadow[17],0,0);
	draw_sprite_h_flip(wall_shadow[19],wall_shadow[18],0,0);

	clear_to_color(wall_shadow[20],WALL_SHADOW_MIN);



	///////////////////////////////////
	//// Do the stair shadows!! ///////
	/////////////////////////////////////

	
	//Straight shadow
	for(i=21;i<25;i++)
		clear_to_color(wall_shadow[i],S_SHADOW_MIN);

	
	//allegro_message("%d %d %d",S_SHADOW_END, S_SHADOW_LENGTH,S_SHADOW_MIN);
	add = (31 - S_SHADOW_MIN)/(S_SHADOW_END - S_SHADOW_LENGTH);
	col = S_SHADOW_MIN;

	//Do the vertical
	for(i=S_SHADOW_LENGTH;i<S_SHADOW_END;i++)
	{
		col+=add;
		line(wall_shadow[21],0,i,31,i,col);
	}
	for(i=S_SHADOW_END;i<32;i++)
	{
		line(wall_shadow[21],0,i,31,i,31);
	}


	draw_sprite_v_flip(wall_shadow[22],wall_shadow[21],0,0);
	
	add = (31 - S_SHADOW_MIN)/(S_SHADOW_END-S_SHADOW_LENGTH);
	col = S_SHADOW_MIN;

	//Do the horisontal
	for(i=S_SHADOW_LENGTH;i<S_SHADOW_END;i++)
	{
		col+=add;
		line(wall_shadow[23],i,0,i,31,col);
	}
	for(i=S_SHADOW_END;i<32;i++)
	{
		line(wall_shadow[23],i,0,i,31,31);
	}


	draw_sprite_h_flip(wall_shadow[24],wall_shadow[23],0,0);
	
}


//Checks if the x,y is a wall (you can't see behind it...) 
int tile_is_wall_solid(int x, int y)
{
	int tile_set;
	int tile_num;

	if(x<0 || x>map->w-1 || y<0 || y>map->h-1)
		return 1;
	
	tile_set = (map->layer3+ (x) +( (y) * map->w))->tile_set;
	tile_num = (map->layer3+ (x) +( (y) * map->w))->tile_num; 			
	
	if(tile_info[tile_set].tile[tile_num].solid>1)return 1;
	
	
	return 0;
}

//Draw the wall shadows....
void draw_wall_shadows(BITMAP *dest,int xpos,int ypos)
{
	int i,j;
	int x,y,x1,y1;
	char num;
	//int temp_x, temp_y;


	x1=0-(xpos%TILE_SIZE);//check where on the tile map you begin to draw
	y1=0-(ypos%TILE_SIZE);
	 
	x=(xpos-(xpos%TILE_SIZE))/TILE_SIZE;//what tile do you begin with 
	y=(ypos-(ypos%TILE_SIZE))/TILE_SIZE;

	for(i=-1;i< (dest->w/TILE_SIZE+1) ;i++)
		for(j=-1;j< (dest->h/TILE_SIZE+1) ;j++)
		{
			if((i+x < 0)|| (j+y < 0) || (j+y > map->h-1) || (i+x > map->w-1) ) //if the tile is out side the map it is just a black square
			{
	   
			}
			else //get the set and the number of the tiles
			{
				num = *(map->shadow+ (i+x) +( (j+y) * map->w));
			
				if(num>-1)
					draw_lightsprite(dest, wall_shadow[num], x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
			}
		}

	/*if(!in_the_game)
	{
		temp_x = map_x;
		temp_y = map_y;

		map_x=map_x*32;
		map_y=map_y*32;
	}

	for(i=-1;i<16;i++)
		for(j=-1;j<16;j++)
        {
			
			x = i + map_x/32;
			y = j + map_y/32;
			
					
			if(tile_is_wall_solid(x,y))
			{
				
				if( !tile_is_wall_solid(x,y+1) && tile_is_wall_solid(x-1,y) && !tile_is_wall_solid(x-1,y+1))
				{
					draw_lightsprite(dest,wall_shadow[WALL_SHADOW_DOWN] ,(i)*32-(map_x%32), (j+1)*32-(map_y%32) );
				}
				else if( !tile_is_wall_solid(x,y+1) && !tile_is_wall_solid(x-1,y))
				{
					draw_lightsprite(dest,wall_shadow[WALL_SHADOW_HALF_DOWN] ,(i)*32-(map_x%32), (j+1)*32-(map_y%32) );
				}
				else if(!tile_is_wall_solid(x,y+1) && tile_is_wall_solid(x-1,y) && tile_is_wall_solid(x-1,y+1))
				{
					draw_lightsprite(dest,wall_shadow[WALL_SHADOW_FULL] ,(i)*32-(map_x%32), (j+1)*32-(map_y%32) );
				}
				
				if(!tile_is_wall_solid(x+1,y) && tile_is_wall_solid(x,y-1) && !tile_is_wall_solid(x+1,y-1))
				{
					draw_lightsprite(dest,wall_shadow[WALL_SHADOW_RIGHT] ,(i+1)*32-(map_x%32), (j)*32-(map_y%32) );
				}
				else if(!tile_is_wall_solid(x+1,y) && !tile_is_wall_solid(x,y-1))
				{
					draw_lightsprite(dest,wall_shadow[WALL_SHADOW_HALF_RIGHT] ,(i+1)*32-(map_x%32), (j)*32-(map_y%32) );
				}
				
				if(!tile_is_wall_solid(x+1,y) && !tile_is_wall_solid(x,y+1) && !tile_is_wall_solid(x+1,y+1))
				{
					draw_lightsprite(dest,wall_shadow[WALL_SHADOW_CORNER] ,(i+1)*32-(map_x%32), (j+1)*32-(map_y%32) );
				}

				

			}
        }

	if(!in_the_game)
	{
		map_x = temp_x;
		map_y = temp_y;
	}*/

}


//make the normal/custom light maps that are used by weapons etc....
void create_normal_lightmaps(void)
{
	int i,j;
	LIGHT_DATA temp_light;

	for(i=0;i<LIGHTMAP_C_NUM;i++)
		for(j=0;j<LIGHTMAP_R_NUM;j++)
		{	
			temp_light.x = lightmap_r[j];
			temp_light.y = lightmap_r[j];

			temp_light.bitmap_w = lightmap_r[j]*2;
			temp_light.bitmap_h = lightmap_r[j]*2;

			temp_light.strech_w = lightmap_r[j]*2;
			temp_light.strech_h = lightmap_r[j]*2;

			temp_light.r = lightmap_r[j];
			temp_light.centre_r = 1;

			temp_light.max_light = lightmap_c[i];

			create_light_map2(&normal_lightmap[i][j], &temp_light);  
		}

	normal_light_data = calloc(sizeof(NORMAL_LIGHT_DATA), NORMAL_LIGHT_NUM);
	
	for(i=0;i<NORMAL_LIGHT_NUM;i++)
		normal_light_data[i].used=0;

}


//add a normal light in the game....
void make_normal_light(int x, int y, int c, int r, int time)
{
	int i;
	
	if(c<0 || r<0)return;

	for(i=0;i<NORMAL_LIGHT_NUM;i++)
		if(!normal_light_data[i].used)
			break;
	
	normal_light_data[i].x = x;
	normal_light_data[i].y = y;
	normal_light_data[i].c = c;
	normal_light_data[i].r = r;

	normal_light_data[i].time = time;

	normal_light_data[i].used = 1;

}

//upadate the normal lights
void update_normal_light(void)
{
	int i;


	for(i=0;i<NORMAL_LIGHT_NUM;i++)
		if(normal_light_data[i].used)
		{
			normal_light_data[i].time--;
			
			if(normal_light_data[i].time<0)
				normal_light_data[i].used=0;
		}

}


//draw a normal lightmap
void draw_normal_light(BITMAP* dest, int num)
{
	int size;
	if(normal_light_data[num].used)
	{
		size = normal_lightmap[normal_light_data[num].c][normal_light_data[num].r]->w/2;

		draw_lightmap2(dest, normal_lightmap[normal_light_data[num].c][normal_light_data[num].r],normal_light_data[num].x - map_x -size, normal_light_data[num].y - map_y -size );
	}

}


//make the shadows below characters
void create_shadows(void)
{
	int i,j;
	int w,h,r,c;
	int color;

	for(i=0;i<num_of_characters;i++)
	{
		w = 0.9 * (float)char_info[i].w;
		h = 0.9 * (float)char_info[i].h;
		
		r = w/2;
		
		c = 0.5 *(float)r;

		char_shadow[i] = create_bitmap_ex(8,w,h);

		clear_to_color(char_shadow[i],31);
						
		for(j=r;j>c;j--)
		 {
			color = ((float)(j-c) / (float)(r-c))*20+11;
			if(color>31)color=31;  
			circlefill(char_shadow[i],w/2, h/2,j, color);
        
		}
	}



	for(i=0;i<num_of_enemys;i++)
	{
		w = 0.8 * (float)enemy_info[i].w;
		h = 0.8 * (float)enemy_info[i].h;
		
		r = w/2;
		
		c = 0.5 *(float)r;

		enemy_shadow[i] = create_bitmap_ex(8,w,h);

		clear_to_color(enemy_shadow[i],31);
						
		for(j=r;j>c;j--)
		 {
			color = ((float)(j-c) / (float)(r-c))*20+11;
			if(color>31)color=31;  
			circlefill(enemy_shadow[i],w/2, h/2,j, color);
        
		}

	}

}





//create a lightmap for the editor.......
void create_light_map(BITMAP **dest, LIGHT_DATA *light)
{
 int i;
 BITMAP *temp;
 int color = light->max_light;
 	
 *dest = create_bitmap_ex(8, light->bitmap_w, light->bitmap_h);
 clear_to_color(*dest, 0); 	 

 
 for(i=light->r;i>light->centre_r;i--)
 {
   color = ((1-( (float)(i-light->centre_r) / (float)(light->r-light->centre_r)   )) * light->max_light);
  
   circlefill(*dest,light->x, light->y,i, color);
        
 }

 if(light->bitmap_w!=light->strech_w || light->bitmap_h!=light->strech_h)
 {
	 temp = create_bitmap_ex(8,light->strech_w, light->strech_h);
	 stretch_blit(*dest, temp,0,0,light->bitmap_w, light->bitmap_h,0,0,light->strech_w,light->strech_h);
	 destroy_bitmap(*dest);
	 *dest = create_bitmap_ex(8,light->strech_w,light->strech_h);
	 blit(temp,*dest,0,0,0,0,light->strech_w,light->strech_h);
	 destroy_bitmap(temp);

 }



}




//create a lightmap for the game.....(same thing but max value is 31 instead of 255)
void create_light_map2(BITMAP **dest, LIGHT_DATA *light)
{
 int i;
 BITMAP *temp;
 int color = light->max_light;
 	
 *dest = create_bitmap_ex(8, light->bitmap_w, light->bitmap_h);
 clear_to_color(*dest, 0); 	 

 
 for(i=light->r;i>light->centre_r;i--)
 {
   color = ((1-( (float)(i-light->centre_r) / (float)(light->r-light->centre_r)   )) * light->max_light)/8;
   if(color>31)color=31;  
   circlefill(*dest,light->x, light->y,i, color);
        
 }

 if(light->bitmap_w!=light->strech_w || light->bitmap_h!=light->strech_h)
 {
	 temp = create_bitmap_ex(8,light->strech_w, light->strech_h);
	 stretch_blit(*dest, temp,0,0,light->bitmap_w, light->bitmap_h,0,0,light->strech_w,light->strech_h);
	 destroy_bitmap(*dest);
	 *dest = create_bitmap_ex(8,light->strech_w,light->strech_h);
	 blit(temp,*dest,0,0,0,0,light->strech_w,light->strech_h);
	 destroy_bitmap(temp);

 }



}


