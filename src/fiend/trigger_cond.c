////////////////////////////////////////////////////
// This file contains trigger cond checking 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"

// a var for automovement
extern int auto_move_active;

int condition_used_something=0;
int condition_item_something=0;


int check_logic(int n1, int n2, int type)
{
	if(type==0)
	{
		if(n1==n2)return 1;
	}
	else if(type==1)
	{
		if(n1>n2)return 1;
	}
	else
	{
		if(n1<n2)return 1;
	}

	return 0;
}


int check_cond(int t_num,int c_num ,int global)
{
	int i;
	int num;
	int num2;
	int type ;
	int x;
	int true_state;
	int z;
	int logic;
	char string1[100];
	char string2[100];
	
	
	if(!global)
	{
		type = map->trigger[t_num].condition[c_num].type;
		x = get_var_value(map->trigger[t_num].condition[c_num].x);
		true_state = map->trigger[t_num].condition[c_num].correct;
		z = get_var_value(map->trigger[t_num].condition[c_num].z);
		logic = map->trigger[t_num].condition[c_num].logic;
		strcpy(string1, map->trigger[t_num].condition[c_num].string1);
		strcpy(string2, map->trigger[t_num].condition[c_num].string2);
	}
	else
	{
		type = global_trigger[t_num].condition[c_num].type;
		x = get_var_value(global_trigger[t_num].condition[c_num].x);
		true_state = global_trigger[t_num].condition[c_num].correct;
		z = get_var_value(global_trigger[t_num].condition[c_num].z);
		logic = global_trigger[t_num].condition[c_num].logic;
		strcpy(string1, global_trigger[t_num].condition[c_num].string1);
		strcpy(string2, global_trigger[t_num].condition[c_num].string2);
	}
	

	
	
	/////////////////////////////////
		
	if(type==COND_PLAYER_AREA)
	{
		num = get_area_num(string1);

		if(num<0){
			return -1;}
		
		if(check_collision(player.x - char_info[0].w/2, player.y - char_info[0].h/2,  char_info[0].w,  char_info[0].h,
			map->area[num].x - map->area[num].w/2, map->area[num].y - map->area[num].h/2, map->area[num].w, map->area[num].h))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	
	/////////////////////////////////
	
	if(type==COND_PLAYER_ENERGY)
	{
		if(check_logic(player.energy,x, logic))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	
	/////////////////////////////////
	
	if(type==COND_PLAYER_TRIGGER_OBJECT)
	{
		z = get_object_num(string1);

		if(z<0){
			return -1;}
		
		if(player_has_used && map->object[z].active &&
		check_angle_collision(player_use_x, player_use_y, PLAYER_USE_W, PLAYER_USE_H,0,
		  map->object[z].x, map->object[z].y, object_info[map->object[z].type].w, object_info[map->object[z].type].h,map->object[z].angle))
		{
			if(true_state){condition_used_something=1;
			return 1;}
			else return 0;
		}
		else
		{
			if(!true_state) return 1;
			else return 0;
		}
		
		
	}
	
	/////////////////////////////////
	
	if(type==COND_PLAYER_TRIGGER_AREA)
	{
		num = get_area_num(string1);

		if(num<0){
			return -1;}
				
		if(player_has_used && map->area[num].active &&
		check_collision(player_use_x - PLAYER_USE_W/2, player_use_y - PLAYER_USE_W/2, PLAYER_USE_W, PLAYER_USE_H,
		  map->area[num].x - map->area[num].w/2, map->area[num].y - map->area[num].h/2, map->area[num].w, map->area[num].h))
		{
			if(true_state){condition_used_something=1;
			return 1;}
			else return 0;
		}
		else
		{
			if(!true_state) return 1;
			else return 0;
		}
	}
	/////////////////////////////////
	
	if(type==COND_PLAYER_TRIGGER_ENEMY)
	{
		num = get_enemy_num(string1);

		if(num<0){
			return -1;}
		
		
		if(player_has_used && enemy_data[num].active && enemy_data[num].used &&
		check_collision(player_use_x - PLAYER_USE_W/2, player_use_y - PLAYER_USE_W/2, PLAYER_USE_W, PLAYER_USE_H,
		 enemy_data[num].x - enemy_info[enemy_data[num].type].w/2, enemy_data[num].y - enemy_info[enemy_data[num].type].h/2, enemy_info[enemy_data[num].type].w,enemy_info[enemy_data[num].type].h))
		{
			if(true_state){condition_used_something=1;
			return 1;}
			else return 0;
		}
		else
		{
			if(!true_state) return 1;
			else return 0;
		}
	}
	/////////////////////////////////
	
	if(type==COND_PLAYER_USE_ITEM_OBJECT)
	{
		z = get_object_num(string2);

		if(z<0){
			return -1;}
		
		if(player_item_num>-1 && player_has_used_item && map->object[z].active && strcmp(item_data[player_item_num].name, string1)==0 &&
		check_angle_collision(player_item_x, player_item_y, PLAYER_USE_W, PLAYER_USE_H,0,
		  map->object[z].x, map->object[z].y, object_info[map->object[z].type].w, object_info[map->object[z].type].h,map->object[z].angle))
		{
			if(true_state){condition_item_something=1;
			return 1;}
			else return 0;
		}
		else
		{
			if(!true_state) return 1;
			else return 0;
		}
	}
	/////////////////////////////////
	if(type==COND_PLAYER_USE_ITEM_NPC)
	{
		num = get_npc_num(string2);

		if(num<0){
			return -1;}
		
		if(player_item_num>-1 && player_has_used_item && npc_data[num].active && strcmp(item_data[player_item_num].name, string1)==0 &&
		check_collision(player_item_x - PLAYER_USE_W/2, player_item_y - PLAYER_USE_W/2, PLAYER_USE_W, PLAYER_USE_H,
		  npc_data[num].x - char_info[npc_data[num].type].w/2, npc_data[num].y - char_info[npc_data[num].type].h/2, char_info[npc_data[num].type].w,char_info[npc_data[num].type].h))
		{
			if(true_state){condition_item_something=1;
			return 1;}
			else return 0;
		}
		else
		{
			if(!true_state) return 1;
			else return 0;
		}
	}
	/////////////////////////////////	
	if(type==COND_PLAYER_USE_ITEM_AREA)
	{
		num = get_area_num(string2);

		if(num<0){
			return -1;}
		
		
		if(player_item_num>-1 && player_has_used_item && map->area[num].active && strcmp(item_data[player_item_num].name, string1)==0 &&
		check_collision(player_item_x - PLAYER_USE_W/2, player_item_y - PLAYER_USE_W/2, PLAYER_USE_W, PLAYER_USE_H,
		  map->area[num].x - map->area[num].w/2, map->area[num].y - map->area[num].h/2, map->area[num].w, map->area[num].h))
		{
			if(true_state){condition_item_something=1;
			return 1;}
			else return 0;
		}
		else
		{
			if(!true_state) return 1;
			else return 0;
		}
	}
	/////////////////////////////////
	if(type==COND_PLAYER_USE_ITEM_ENEMY)
	{
		num = get_enemy_num(string2);

		if(num<0){
			return -1;}
		
		
		if(player_item_num>-1 && player_has_used_item && enemy_data[num].active && strcmp(item_data[player_item_num].name, string1)==0 &&
		check_collision(player_item_x - PLAYER_USE_W/2, player_item_y - PLAYER_USE_W/2, PLAYER_USE_W, PLAYER_USE_H,
		  enemy_data[num].x - enemy_info[enemy_data[num].type].w/2, enemy_data[num].y - enemy_info[enemy_data[num].type].h/2, enemy_info[enemy_data[num].type].w,enemy_info[enemy_data[num].type].h))
		{
			if(true_state){condition_item_something=1;
			return 1;}
			else return 0;
		}
		else
		{
			if(!true_state) return 1;
			else return 0;
		}
	}
	////////////////////////////////
	if(type==COND_HAS_ITEM)
	{
		num = -1;
		
		for(i=0;i<player.num_of_items;i++)
			if(player.item_space[i].item>-1)
				if(strcmp(item_data[player.item_space[i].item].name, string1)==0)num = i;
			
		if(num!=-1)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;	
		}
	}

	///////////////////////////////////
	
	if(type==COND_ENEMY_ENERGY)
	{
		num = get_enemy_num(string1);

		if(num<0){
			return -1;}
		
		
		if(check_logic(enemy_data[num].energy, x, logic))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	///////////////////////////////////

	if(type==COND_ENEMY_MISSION)
	{
		num = get_enemy_num(string1);

		if(num<0){
			return -1;}
		
		
		if(enemy_data[num].current_mission==x)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	
	////////////////////////////////////

	if(type==COND_ENEMY_AREA)
	{
		num = get_enemy_num(string1);
		num2 = get_area_num(string2);

		if(num<0){
			return -1;}
		if(num2<0){
			return -1;}
		
		
		if(map->area[num2].active && enemy_data[num].active &&
		   check_collision(enemy_data[num].x - enemy_info[enemy_data[num].type].w/2, enemy_data[num].y - enemy_info[enemy_data[num].type].h/2, enemy_info[enemy_data[num].type].w,enemy_info[enemy_data[num].type].h,
		   map->area[num2].x - map->area[num2].w/2, map->area[num2].y - map->area[num2].h/2, map->area[num2].w, map->area[num2].h))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	
	/////////////////////////////////

	if(type==COND_NPC_ENERGY)
	{
		num = get_npc_num(string1);

		if(num<0){
			return -1;}
				
		if(check_logic(npc_data[num].energy, x, logic))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	//////////////////////////////////
	
	if(type==COND_NPC_AREA)
	{
		num = get_npc_num(string1);
		num2 = get_area_num(string2);

		if(num<0){
			return -1;}
		if(num2<0){
			return -1;}
		
		
		
		if(map->area[num2].active && npc_data[num].active &&
		   check_collision(npc_data[num].x - char_info[npc_data[num].type].w/2, npc_data[num].y - char_info[npc_data[num].type].h/2, char_info[npc_data[num].type].w,char_info[npc_data[num].type].h,
		   map->area[num2].x - map->area[num2].w/2, map->area[num2].y - map->area[num2].h/2, map->area[num2].w, map->area[num2].h))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
		//////////////////////////////////
	
	if(type==COND_NPC_DIALOG)
	{
		num = get_npc_num(string1);
		
		if(num<0){
			return -1;}
		
		if(check_logic(npc_data[num].current_dialog, x, logic))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	
	//////////////////////////////////
	if(type==COND_OBJECT_ENERGY)
	{
		z = get_object_num(string1);

		if(z<0){
			return -1;}
		
		
		if(check_logic(map->object[z].energy,x,logic))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	/////////////////////////////////

	if(type==COND_OBJECT_AREA)
	{
		z = get_object_num(string1);
				
		num2= get_area_num(string2);

		if(z<0){
			return -1;}
		if(num2<0){
			return -1;}
		
		

		if(map->area[num2].active && map->object[z].active &&
		check_angle_collision(map->object[z].x, map->object[z].y, object_info[map->object[z].type].w,object_info[map->object[z].type].h,map->object[z].angle,
		 map->area[num2].x - map->area[num2].w/2, map->area[num2].y - map->area[num2].h/2, map->area[num2].w, map->area[num2].h,0))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}


	////////////////////////////////

	if(type==COND_LIGHTLEVEL)
	{
		if(check_logic(map->light_level,x,logic))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	
	////////////////////////////////

	if(type==COND_MAP_OUTSIDE)
	{
		if(check_logic(map->outside,x,logic))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	/////////////////////////////////

	if(type==COND_GLOBAL_VAR)
	{
		num = get_global_var_num(string1);
		if(num<0){
			return -1;}
		
		if(check_logic(global_var[num].value,x,logic))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	
	//////////////////////////////////
	
	if(type==COND_LOCAL_VAR)
	{
		num = get_local_var_num(string1);
		if(num<0){
			return -1;}
		

		if(check_logic(map->var[num].value,x,logic))
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	///////////////////////////////


	if(type==COND_ITEM_PICKED)
	{
		z = get_item_num(string1);
		
		if(item_data[z].picked_up)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	///////////////////////////////

	if(type==COND_ITEM_ABLED)
	{
		z = get_item_num(string1);
		
		if(item_data[z].active)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	///////////////////////////////
	if(type==COND_NPC_ABLED)
	{
		num = get_npc_num(string1);

		if(num<0){
			return -1;}
		

		if(npc_data[num].active)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	///////////////////////////////
	if(type==COND_ENEMY_ABLED)
	{
		num = get_enemy_num(string1);

		if(num<0){
			return -1;}
		

		if(enemy_data[num].active)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	///////////////////////////////
	
	if(type==COND_OBJECT_ABLED)
	{
		z = get_object_num(string1);

		if(z<0){
			return -1;}
		

		if(map->object[z].active)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	///////////////////////////////

	if(type==COND_AUTOMOVE)
	{
		if(auto_move_active)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	///////////////////////////////
	if(type==COND_MESSAGE_ACTIVE)
	{
		if(message_active)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}

	///////////////////////////////
	if(type==COND_RANDOM)
	{
		if(RANDOM(0,x)==0)
		{
			if(true_state)return 1;
			else return 0;
		}
		else
		{
			if(!true_state)return 1;
			else return 0;
		}
	}
	
	///////////////////////////////



	if(type==COND_ALWAYS)
	{
		return 1;
	}
	
	
	return 0;
}