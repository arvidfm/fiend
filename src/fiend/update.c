////////////////////////////////////////////////////
// This file contains misc update funcs 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"

#define MIN_UPDATE_TURNS 300
#define MAX_UPDATE_TURNS 400

int current_map_npc[MAX_NPC_NUM];
int current_map_enemy[MAX_ENEMY_DATA];
int current_map_item[MAX_ITEM_DATA];

int current_map_npc_num;
int current_map_enemy_num;
int current_map_item_num;

int outside_lightlevel=31;



////////////////////////////////////////////////
///////////// MAP ///////////////////////////
/////////////////////////////////////////////

void update_enemies_before_map(void);

void update_before_map(void)
{
	int i, turn;
	int num_of_turns;

	int sound_was_on=0;

	float temp_x, temp_y;
	int temp;

	num_of_turns = RANDOM(MIN_UPDATE_TURNS,MAX_UPDATE_TURNS);
	
	for(i=0;i<map->num_of_objects;i++)
	{
		
		for(turn=0;turn<num_of_turns;turn++)
		{
			if(map->object[i].active && object_info[map->object[i].type].door==0)
			{
				map->object[i].nextframe++;
				if(map->object[i].nextframe>=ANIM_SPEED)
				{
					map->object[i].nextframe =0;
					map->object[i].frame++;
					if(object_info[map->object[i].type].animation[map->object[i].action].frame[map->object[i].frame] ==255)
						map->object[i].frame = object_info[map->object[i].type].animation[map->object[i].action].frame[map->object[i].frame + 1];
				}
			}
		}


	}


	temp_x = player.x;
	temp_y = player.y;

	player.x = -100;
	player.y = -100;

	temp = RANDOM(750, 800);

	sound_was_on=sound_is_on;
	
	sound_is_on=0;

	for(i=0;i<temp;i++)
	{
		update_enemy();
		update_door_objects();
		update_npc();
	}

	update_enemies_before_map();

	if(sound_was_on)
		sound_is_on=1;

	player.x = temp_x;
	player.y = temp_y;

	for(i=0;i<MAX_ENEMY_DATA;i++)
		if(enemy_data[i].active && enemy_data[i].dead && enemy_data[i].disappear)
		{
			enemy_data[i].active = 0;
		}
	 
		 
 	if(map->outside)
		map->light_level = outside_lightlevel;
}




extern int check_enemy_collision(float x,float y, int num, int borders);

void update_enemies_before_map(void)
{
	int i,j,k;
	int end_loop;
	int type;
	int new_x,new_y;
	int count;


	for(j=0;j<current_map_enemy_num;j++)
	{
		i = current_map_enemy[j];
		if(enemy_data[i].used && enemy_data[i].active && !enemy_data[i].dead)
		{
			//place the enmy on the map
			end_loop=0;
			for(count=0;count<400;count++)
			{
				type = enemy_data[i].type;
				
				new_x = enemy_data[i].x + RANDOM(-200,200); 
				new_y = enemy_data[i].y + RANDOM(-200,200); 
				
				end_loop = 1;

				if(check_enemy_collision(new_x, new_y, i,1))
					end_loop = 0;

				for(k=0;k<map->num_of_links;k++)
				{
					if(check_collision(new_x-60,new_y-60,120,120,map->link[k].x - map->link[k].w/2,map->link[k].y - map->link[k].h/2,map->link[k].w,map->link[k].h)) 
						end_loop = 0;
				}

				if(end_loop)break;

				//textout(screen,font_avalon->dat,"Updating enemy before map1",80,350,makecol(200,200,200));
			}
			
			
			if(end_loop)
			{
				enemy_data[i].x = new_x;
				enemy_data[i].y = new_y;
			}

			//textout(screen,font_avalon->dat,"END!! Updating enemy before map2",80,350,makecol(200,200,200));

		}
	}

}


//////////////////////////////////////////
//////////OBJECTS //////////////////////
////////////////////////////////////////////
void update_door_objects(void)
{
	int i,type;

	for(i=0;i<map->num_of_objects;i++)
	{
		type = map->object[i].type;
		if(map->object[i].active && object_info[type].door!=0)
		{
			if(map->object[i].energy==0 && player_has_used && check_angle_collision(player_use_x, player_use_y, PLAYER_USE_W, PLAYER_USE_H,0,
			map->object[i].x, map->object[i].y, object_info[map->object[i].type].w, object_info[map->object[i].type].h,map->object[i].angle))
			{
				if(map->object[i].action==1)
				{
					map->object[i].action=0;
					play_fiend_sound(object_info[type].animation[0].sound,map->object[i].x,map->object[i].y,1,0,180);
				}
			}

			
			//play sound when door opens/closes
			
			//update the opening of the door
			if(map->object[i].action==0 && map->object[i].frame<100)
			{
				map->object[i].frame++;
			}
			if(map->object[i].action==1 && map->object[i].frame>0)
			{
				map->object[i].frame--;
			}
	
		}

	}
}




void change_object_action(int o_num,int a_num, int with_sound)
{
		
	if(map->object[o_num].action!=a_num)
	{
		//If th object is playing a sound, stop it!
		if(object_info[map->object[o_num].type].animation[map->object[o_num].action].loop_sound && map->object[o_num].voice_num>-1)
			stop_sound_num(map->object[o_num].voice_num);
		map->object[o_num].voice_num =-1;

		map->object[o_num].action = a_num;
		map->object[o_num].frame =0;
		map->object[o_num].nextframe =0;
		
		//If the object plays a sound, play it!
		if( (with_sound && strcmp(object_info[map->object[o_num].type].animation[map->object[o_num].action].sound,"none")!=0 )
			|| object_info[map->object[o_num].type].animation[map->object[o_num].action].loop_sound)
			map->object[o_num].voice_num = play_fiend_sound(object_info[map->object[o_num].type].animation[a_num].sound, map->object[o_num].x, map->object[o_num].y,1,
							object_info[map->object[o_num].type].animation[a_num].loop_sound, 160);
	}
		

}




extern float item_light;
extern float item_light_add;

void update_objects(void)
{
	int i;

	for(i=0;i<map->num_of_objects;i++)
	{
		if(map->object[i].active && object_info[map->object[i].type].door==0)
		{
			map->object[i].nextframe++;
			if(map->object[i].nextframe>=ANIM_SPEED)
			{
				map->object[i].nextframe =0;
				map->object[i].frame++;
				if(object_info[map->object[i].type].animation[map->object[i].action].frame[map->object[i].frame] ==255)
					map->object[i].frame = object_info[map->object[i].type].animation[map->object[i].action].frame[map->object[i].frame + 1];
			}

		}

	}

	//some item stuff....
	item_light += item_light_add;
	
	if(item_light>200)
	{
		item_light = 200; 
		item_light_add = -item_light_add;
	}

	if(item_light<0)
	{
		item_light = 0; 
		item_light_add = -item_light_add;
	}


}




//////////////////////////////////////////////
///// GET CURRENT MAP OBJECTS ////////////////
//////////////////////////////////////////////


void get_current_map_objects(void)
{
	int i;

	current_map_enemy_num =0;
	for(i=0;i<MAX_ENEMY_DATA;i++)
	{
		if(enemy_data[i].used && strcmp(map->name, enemy_data[i].map_name)==0)
		{
			current_map_enemy[current_map_enemy_num] = i;
			current_map_enemy_num++;
		}
	}
	
	current_map_npc_num=0;
	for(i=0;i<MAX_NPC_NUM;i++)
	{
		if(npc_data[i].used && strcmp(map->name, npc_data[i].map_name)==0)
		{
			current_map_npc[current_map_npc_num] = i;
			current_map_npc_num++;
		}
	}

	current_map_item_num=0;
	for(i=0;i<MAX_ITEM_DATA;i++)
	{
		if(item_data[i].used && strcmp(map->name, item_data[i].map_name)==0)
		{
			current_map_item[current_map_item_num] = i;
			current_map_item_num++;
		}
	}


}



