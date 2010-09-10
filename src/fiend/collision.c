////////////////////////////////////////////////////
// This file Contains collsion detection for the data
// Object. 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"




///////////////////////////////////////////////////
////// OBJECT COLLISION DETECTITION //////////////////
///////////////////////////////////////////////////





int check_object_tile_collision(float x, float y,int num)
{
	int type = map->object[num].type;
	int max_length = sqrt(object_info[type].w*object_info[type].w + object_info[type].h*object_info[type].h);
	
	int tile_x, tile_y;
	int tile_set,tile_num;

	int i,j;

	int layer;

	map_tile((int)x, (int)y, &tile_x, &tile_y,TILE_SIZE);

	
	for(layer = 0;layer<3;layer++)
		for(i=-(max_length/2)/TILE_SIZE-1;i<(max_length/2)/TILE_SIZE+1;i++)
			for(j=-(max_length/2)/TILE_SIZE-1;j<(max_length/2)/TILE_SIZE+1;j++)
			{
				if(i+tile_x<map->w && i+tile_x>=0 && j+tile_y<map->h && j+tile_y>=0)
				{
					if(layer==0){
					tile_set = (map->layer1+ (i+tile_x) +( (j+tile_y) * map->w))->tile_set;
					tile_num = (map->layer1+ (i+tile_x) +( (j+tile_y) * map->w))->tile_num; 			
					}
					if(layer==1){
					tile_set = (map->layer2+ (i+tile_x) +( (j+tile_y) * map->w))->tile_set;
					tile_num = (map->layer2+ (i+tile_x) +( (j+tile_y) * map->w))->tile_num; 			
					}
					if(layer==2){
					tile_set = (map->layer3+ (i+tile_x) +( (j+tile_y) * map->w))->tile_set;
					tile_num = (map->layer3+ (i+tile_x) +( (j+tile_y) * map->w))->tile_num; 			
					}

					if(tile_info[tile_set].tile[tile_num].solid) 
						if(check_angle_collision(x, y, object_info[map->object[num].type].w,object_info[map->object[num].type].h,  map->object[num].angle,   
							(float)(tile_x+i)*TILE_SIZE+TILE_SIZE/2,(float) (tile_y+j)*TILE_SIZE+TILE_SIZE/2,TILE_SIZE,TILE_SIZE,0) )
								return 1;
				}

			}


		




	return 0;
}



int check_object_object_collision(float x, float y,int num)
{
	int i,j;

	for(i=0;i<map->num_of_objects;i++)
	{
		if(i!=num && map->object[i].active)
			if(object_info[map->object[i].type].solid>0 && object_info[map->object[i].type].animation[map->object[i].action].solid>0)
				if( check_angle_collision(x, y, object_info[map->object[num].type].w,object_info[map->object[num].type].h,  map->object[num].angle,
					map->object[i].x, map->object[i].y, object_info[map->object[i].type].w,object_info[map->object[i].type].h,  map->object[i].angle ) )
						return 1;

	}

	for(j=0;j<current_map_enemy_num;j++)
	{
		i = current_map_enemy[j];
		if(!enemy_data[i].dead &&enemy_data[i].active && enemy_data[i].used && !enemy_info[enemy_data[i].type].under_player)
			if(check_angle_collision(x, y, object_info[map->object[num].type].w,object_info[map->object[num].type].h,  map->object[num].angle,
				 enemy_data[i].x, enemy_data[i].y,enemy_info[enemy_data[i].type].w,enemy_info[enemy_data[i].type].h,0)) 
					return 1;
	}

	for(j=0;j<current_map_npc_num;j++)
	{
		i = current_map_npc[j];
		if(npc_data[i].active && npc_data[i].used&& !npc_data[i].dead)
			if(check_angle_collision(x, y, object_info[map->object[num].type].w,object_info[map->object[num].type].h,  map->object[num].angle, 
				npc_data[i].x, npc_data[i].y,char_info[npc_data[i].type].w,char_info[npc_data[i].type].h,0)) 
					return 1;
	}

	
	if(check_object_tile_collision(x,y,num)) return 1;



	return 0;
}




