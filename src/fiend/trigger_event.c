////////////////////////////////////////////////////
// This file contains trigger event checking
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"

// a var for automovement
extern int auto_move_active;
extern int outside_lightlevel;

static int num;
static int num2;
static int type;
static int x;
static int y;
static int z;
static char string1[100];
static char string2[100];
static char text[200];

extern int with_sound;


void init_check_events(int t_num, int e_num, int global)
{
	
	if(!global)
	{
		x = get_var_value(map->trigger[t_num].event[e_num].x);
		y = get_var_value(map->trigger[t_num].event[e_num].y);
		z = get_var_value(map->trigger[t_num].event[e_num].z);

		type = map->trigger[t_num].event[e_num].type;
	
		strcpy(string1, map->trigger[t_num].event[e_num].string1);
		strcpy(string2, map->trigger[t_num].event[e_num].string2);
		strcpy(text, map->trigger[t_num].event[e_num].text);
	}
	else
	{
		x = get_var_value(global_trigger[t_num].event[e_num].x);
		y = get_var_value(global_trigger[t_num].event[e_num].y);
		z = get_var_value(global_trigger[t_num].event[e_num].z);

		type = global_trigger[t_num].event[e_num].type;
	
		strcpy(string1, global_trigger[t_num].event[e_num].string1);
		strcpy(string2, global_trigger[t_num].event[e_num].string2);
		strcpy(text, global_trigger[t_num].event[e_num].text);
	}
	

	
}


/////////////////////////////////////
/////// CHECK PLAYER EVENTS ////////
//////////////////////////////////////

int check_player_events(void)
{
	int i,j,temp;
	int item_type;
	
	if(type==EVENT_PLAYER_ENERGY_BECOME)
	{
		player.energy = x;
		return 1;
	}

	if(type==EVENT_PLAYER_ENERGY_ADD)
	{
		player.energy+= x;
		return 1;
	}
	
	if(type==EVENT_PLAYER_POS)
	{
		player.x = x;
		player.y = y;
		return 1;
	}

	if(type==EVENT_PLAYER_MAP)
	{
		stop_all_sounds();
		save_local_vars();

		//Load the new map...
		load_edit_map(map, text);
		load_local_vars();

		player.x = map->player_x;
		player.y = map->player_y;
	
		check_triggers(0);
		get_current_map_objects();
		
		reset_npc_data();
		reset_enemy_data();
		reset_shells();
		reset_bloodpools();
		reset_missiles();
		reset_flames();
		reset_particles();
		reset_effects();
		reset_enemy_ai();
		reset_npc_ai();
		reset_beams();

		update_before_map();

			
		speed_counter=0;

		return 1;
	}
	///////////////////////////////
	if(type==EVENT_PLAYER_ITEM)
	{
		i = get_item_num(string1);
		
		if(i<0)return -1;

		item_type = item_info[item_data[i].type].type;

		////// you pick up a normal item ////////
		if(item_type == 0)
		{
			player.item_space[player.num_of_items].item = i;
			player.item_space[player.num_of_items].active = 1;
			player.num_of_items++;
		}
		////// you pick up a weapon ////////
		else if(item_type == 1)
		{
			player.weapon_space[player.num_of_weapons].item = i;
			item_data[i].active = 1;
			player.num_of_weapons++;
		}
		////// you pick up a ammo ////////
		else if(item_type == 2)
		{
			temp=-1;
			for(j=0;j<player.num_of_ammo;j++)
			if(strcmp(item_info[item_data[player.ammo_space[j].item].type].s_string,item_info[item_data[i].type].s_string)==0)
			{
				temp=j;
				break;
			}
						
			//I the typ of ammo don't exist in the inventory make anew space
			if(temp==-1)
			{
				player.ammo_space[player.num_of_ammo].item = i;
				player.ammo_space[player.num_of_ammo].num = item_data[i].value;
				player.ammo_space[player.num_of_ammo].active = 1;
				player.num_of_ammo++;
			}
			else //make the type of ammo ammount larger.
			{
				player.ammo_space[temp].num+=item_data[i].value;
			}
		}
		////// you pick up a note ////////
		else if(item_type == 3)
		{
			player.note_space[player.num_of_notes].item = i;
			player.note_space[player.num_of_notes].active = 1;
			player.num_of_notes++;
		}
		item_data[i].picked_up=1;


		return 1;
	}
	////////////////////////////////

	if(type==EVENT_PLAYER_ACTIVATE_BECOME)
	{
		player.active = x;
		return 1;
	}

	if(type==EVENT_PLAYER_ANGLE)
	{
		player.angle=x;

		return 1;
	}

	if(type==EVENT_PLAYER_ACTION)
	{
		change_player_action(string1);
		player.animation = x;
		return 1;
	}


	return 0;
}



/////////////////////////////////////
/////// CHECK ENEMY EVENTS ////////
//////////////////////////////////////

int check_enemy_events(void)
{
	if(type==EVENT_ENEMY_ENERGY_BECOME)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
		
		enemy_data[num].energy = x;
		return 1;
	}

	if(type==EVENT_ENEMY_ENERGY_ADD)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
				
		enemy_data[num].energy+= x;
		return 1;
	}
	
	if(type==EVENT_ENEMY_POS)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
		
		enemy_data[num].x = x;
		enemy_data[num].y = y;
		return 1;
	}

	if(type==EVENT_ENEMY_MAP)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
		
		strcpy(enemy_data[num].map_name, string1);
		
		return 1;
	}

	if(type==EVENT_ENEMY_KNOW_PLAYER)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
		
		enemy_ai[num].found_player=FOUND_LENGTH;
		enemy_ai[num].last_player_x = player.x;
		enemy_ai[num].last_player_y = player.y;

		return 1;
	}

	if(type==EVENT_ENEMY_ACTIVATE_BECOME)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
		
		enemy_data[num].active = x;
		return 1;
	}

	if(type==EVENT_ENEMY_DEAD_BECOME)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
		
		enemy_data[num].dead=x;

		if(enemy_data[num].dead>0)
			enemy_data[num].was_dead=0;
		
		if(enemy_data[num].dead==0)
			enemy_data[num].was_dead=1;
		
		
		return 1;
	}

	if(type==EVENT_ENEMY_ACTION)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;

		change_enemy_action(string2,num);
		enemy_ai[num].animation=x;
		return 1;
	}

	
	if(type==EVENT_ENEMY_MISSION)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
		
		enemy_data[num].current_mission=x;

		
		return 1;
	}

	if(type==EVENT_ENEMY_RUN)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
		
		enemy_ai[num].run_x = x;
		enemy_ai[num].run_y = y;
		enemy_ai[num].running = 1;

		enemy_ai[num].attacking=0;
		enemy_ai[num].walking_random=0;

		return 1;
	}

	if(type==EVENT_ENEMY_PATH)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;
		
		enemy_data[num].current_movement_path=x;
		return 1;
	}

	return 0;
}



/////////////////////////////////////
/////// CHECK NPC EVENTS ////////
//////////////////////////////////////

int check_npc_events(void)
{
	if(type==EVENT_NPC_ENERGY_BECOME)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
		
		npc_data[num].energy = x;
		return 1;
	}

	if(type==EVENT_NPC_ENERGY_ADD)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
		
		npc_data[num].energy+= x;
		return 1;
	}
	
	if(type==EVENT_NPC_POS)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
		
		npc_data[num].x = x;
		npc_data[num].y = y;
		return 1;
	}

	if(type==EVENT_NPC_MAP)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
		
		strcpy(npc_data[num].map_name, string1);
		
		return 1;
	}

	if(type==EVENT_NPC_SOUND)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
		
		////////// DO LATER ///////////
		return 1;
	}

	if(type==EVENT_NPC_ACTIVATE_BECOME)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
		
		npc_data[num].active = x;
		return 1;
	}

	if(type==EVENT_NPC_ACTIVATE_TOGGLE)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
		
		if(npc_data[num].active==0)
			npc_data[num].active=1;
		else
			npc_data[num].active=0;

		return 1;
	}

	if(type==EVENT_NPC_ACTION)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
				
		change_npc_action(string2,num);
		npc_ai[num].animation=x;
		
		return 1;
	}


	if(type==EVENT_NPC_PATH)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
		
		npc_data[num].current_movement_path=x;
		return 1;
	}

	if(type==EVENT_NPC_AI)
	{
		num = get_enemy_num(string1);

		if(num<0)return -1;

		////////// DO LATER ///////////
		return 1;
	}

	if(type==EVENT_NPC_DIALOG)
	{
		num = get_npc_num(string1);

		if(num<0)return -1;
		
		npc_data[num].current_dialog=x;
		return 1;
	}

	return 0;
}


////////////////////////////////
//////// OBJECT EVENTS ////////
////////////////////////////////

int check_object_events(void)
{
	if(type==EVENT_OBJECT_ENERGY_BECOME)
	{
		z = get_object_num(string1);

		if(z<0)return -1;

		map->object[z].energy = x;
		return 1;
	}

	if(type==EVENT_OBJECT_ENERGY_ADD)
	{
		z = get_object_num(string1);

		if(z<0)return -1;

		map->object[z].energy+= x;
		return 1;
	}
	
	if(type==EVENT_OBJECT_POS)
	{
		z = get_object_num(string1);

		if(z<0)return -1;

		map->object[z].x = x;
		map->object[z].y = y;
		return 1;
	}

	if(type==EVENT_OBJECT_GO_TO_POS)
	{
		num = get_object_num(string1);

		if(num<0)return -1;


		make_effect(EFFECT_OBJECT_MOVE,num,x,y,z,-1);
		
		return 1;
	}


	if(type==EVENT_OBJECT_ACTIVATE_BECOME)
	{
		z = get_object_num(string1);

		if(z<0)return -1;

		map->object[z].active = x;
		return 1;
	}

	if(type==EVENT_OBJECT_ACTIVATE_TOGGLE)
	{
		z = get_object_num(string1);

		if(z<0)return -1;

		if(map->object[z].active==0)
			map->object[z].active=1;
		else
			map->object[z].active=0;

		return 1;
	}

	if(type==EVENT_OBJECT_ACTION)
	{
		z = get_object_num(string1);

		if(z<0)return -1;

		change_object_action(z,x,with_sound);				
		return 1;
	}
	


	return 0;
}


///////////////////////////////
/////// CHECK ITEM EVENTS //////
/////////////////////////////////

int check_item_events(void)
{
	int i,j;

	if(type==EVENT_ITEM_POS)
	{
		z = get_item_num(string1);

		if(z<0)return -1;

		item_data[z].x = x;
		item_data[z].y = y;
		return 1;
	}


	if(type==EVENT_ITEM_ACTIVATE_BECOME)
	{
		z = get_item_num(string1);

		if(z<0)return -1;

		item_data[z].active = x;
		if(x == 0)
		{
			for(i=0;i<player.num_of_items;i++)
			{
				if(player.item_space[i].item == z)
				{
					for(j=i;j<player.num_of_items-1;j++)
					{
						player.item_space[j].item = player.item_space[j+1].item;
						player.item_space[j].num = player.item_space[j+1].num;
						player.item_space[j].active = player.item_space[j+1].active;
					}
					player.num_of_items--;
					break;
				}
			}
		}

		return 1;
	}

	if(type==EVENT_ITEM_ACTIVATE_TOGGLE)
	{
		z = get_item_num(string1);

		if(z<0)return -1;

		if(item_data[z].active==0)
			item_data[z].active=1;
		else
			item_data[z].active=0;

		if(item_data[z].active == 0)
		{
			for(i=0;i<player.num_of_items;i++)
			{
				if(player.item_space[i].item == z)
				{
					for(j=i;j<player.num_of_items-1;j++)
					{
						player.item_space[j].item = player.item_space[j+1].item;
						player.item_space[j].num = player.item_space[j+1].num;
						player.item_space[j].active = player.item_space[j+1].active;
					}
					player.num_of_items--;
					break;
				}
			}
		}


		return 1;
	}
	
	return 0;
}


///////////////////////////////
////// CHECK AREA EVENTS /////
///////////////////////////////

int check_area_events(void)
{
	if(type==EVENT_AREA_POS)
	{
		num = get_area_num(string1);

		if(num<0)return -1;

		
		map->area[num].x = x;
		map->area[num].y = y;
		return 1;
	}


	if(type==EVENT_AREA_ACTIVATE_BECOME)
	{
		num = get_area_num(string1);

		if(num<0)return -1;
		
		map->area[num].active = x;
		return 1;
	}

	if(type==EVENT_AREA_ACTIVATE_TOGGLE)
	{
		num = get_area_num(string1);

		if(num<0)return -1;

		
		if(map->area[num].active==0)
			map->area[num].active=1;
		else
			map->area[num].active=0;

		return 1;
	}



	return 0;
}


/////////////////////////////////
////////// CHECK LINK EVENT /////
/////////////////////////////////

int check_link_events(void)
{
	if(type==EVENT_LINK_EVENT)
	{
		num = get_link_num(string1);

		if(num<0)return -1;
		
		map->link[num].event_chosen = x;
		return 1;
	}



	return 0;
}


/////////////////////////////////
////////// CHECK LIGHT EVENT /////
/////////////////////////////////

int check_light_events(void)
{
		
	if(type==EVENT_LIGHT_POS)
	{
		map->light[z].world_x = x;
		map->light[z].world_y = y;

		return 1;
	}

	if(type==EVENT_LIGHT_GO_TO_POS)
	{
		num = atoi(string1);
		
		make_effect(EFFECT_LIGHT_MOVE,num,x,y,z,-1);
		
		return 1;
	}


	if(type==EVENT_LIGHT_ACTIVATE_BECOME)
	{
		map->light[z].active = x;
		return 1;
	}

	if(type==EVENT_LIGHT_ACTIVATE_TOGGLE)
	{
		if(map->light[z].active==0)
			map->light[z].active=1;
		else
			map->light[z].active=0;

		return 1;
	}
	
	if(type==EVENT_LIGHT_FLASH_BECOMES)
	{
		map->light[z].flash = x;
		return 1;
	}

	if(type==EVENT_LIGHT_FLASH_TOGGLES)
	{
		if(map->light[z].flash==0)
			map->light[z].flash=1;
		else
			map->light[z].flash=0;

		return 1;
	}

	return 0;
}


/////////////////////////////////
////////// CHECK LIGHTLEVEL EVENT /////
/////////////////////////////////

int check_lightlevel_events(void)
{
	if(type==EVENT_LIGHTLEVEL_BECOME)
	{
		map->light_level=x;
		return 1;
	}

	if(type==EVENT_LIGHTLEVEL_ADD)
	{
		map->light_level+=x;
		return 1;
	}
	
	if(type==EVENT_LIGHTLEVEL_FADE)
	{
		make_effect(EFFECT_LIGHTLEVEL_FADE,0,x,y,map->light_level*100,-1);
		return 1;
	}
	
	
	return 0;
}


/////////////////////////////////
//// CHECK MESSAGE EVENT /////
/////////////////////////////////

int check_message_events(void)
{
	if(type==EVENT_SHOW_MESSAGE)
	{
		if(strcmp(string1,"null")==0 || strcmp(string1,"")==0 || strcmp(string1,"none")==0 || strcmp(string1,"nothing")==0)
			make_new_message(text,NULL);
		else
			make_new_message(text,string1);

		return 1;
	}
	
	if(type==EVENT_SHOW_IMAGE)
	{
		////// DO LATER //////////
		return 1;
	}
	
	return 0;
}



/////////////////////////////////
//// CHECK EFFECT EVENT /////////
/////////////////////////////////

int check_effect_events(void)
{
	int r,g,b;
	
	if(type==EVENT_MAKE_EXPLOSION)
	{
		num = get_area_num(string2);

		if(num<0)return -1;
		
		make_new_particle(string1,map->area[num].x, map->area[num].y,0,0,0,y,x,0);
		return 1;
	}
	
	if(type==EVENT_MAKE_EARTHQUAKE)
	{
		make_effect(EFFECT_QUAKE,0,0,0,z,x);
		return 1;
	}
	
	if(type==EVENT_MAKE_BLOODPOOL)
	{
		num = get_area_num(string1);

		if(num<0)return -1;
		
		sscanf(string2,"%d %d %d",&r,&g,&b);
		make_bloodpool(map->area[num].x ,map->area[num].y ,z,makecol(r,g,b));
		return 1;
	}
	
	if(type==EVENT_MAKE_BEAM)
	{
		num = get_area_num(string1);
		num2 = get_area_num(string2);

		if(num<0)return -1;
		if(num2<0)return -1;


		make_new_beam(text, map->area[num].x,map->area[num].y,map->area[num2].x,map->area[num2].y,x);

		return 1;
	}
	
	if(type==EVENT_MAKE_PICKUP)
	{
		make_pickup_message(text,NULL,x);		
		return 1;
	}

	if(type==EVENT_MAKE_THUNDER)
	{
		make_effect(EFFECT_THUNDER,0,0,0,0,-1);
		return 1;
	}

	if(type==EVENT_MAKE_RAIN)
	{
		make_effect(EFFECT_RAIN,0,x,0,0,-1);
		return 1;
	}

	if(type==EVENT_MAKE_XXX)
	{
		
		/////// DO LATER //////////
		return 1;
	}

	return 0;
}



/////////////////////////////////
//// CHECK SOUNDEMITOR EVENT ////
/////////////////////////////////

int check_soundemitor_events(void)
{
	if(type==EVENT_SOUNDEMITOR_POS)
	{
		map->soundemitor[z].x = x;
		map->soundemitor[z].y = y;
		return 1;
	}


	if(type==EVENT_SOUNDEMITOR_ACTIVATE_BECOME)
	{
		map->soundemitor[z].active = x;
		return 1;
	}

	if(type==EVENT_SOUNDEMITOR_ACTIVATE_TOGGLE)
	{
		if(map->soundemitor[z].active==0)
			map->soundemitor[z].active=1;
		else
			map->soundemitor[z].active=0;

		return 1;
	}
	
	return 0;
}



/////////////////////////////////
//// CHECK SOUND EVENT //////////
/////////////////////////////////

int check_sound_events(void)
{
	if(type==EVENT_PLAY_SOUND)
	{
		if(strcmp(string2,"null")==0 || strcmp(string2,"none")==0)
			num=-1;
		else
			num = get_area_num(string2);

		if(num==-1)
			play_fiend_sound(string1,0,0,0,x,210);
		else
			play_fiend_sound(string1,map->area[num].x,map->area[num].y,1,x,210);

		return 1;
	}
	
	if(type==EVENT_PLAY_MUSIC)
	{
		play_fiend_music(string1,x);
		return 1;
	}
	
	if(type==EVENT_STOP_SOUND)
	{
		
		stop_sound_name(string1);
		return 1;
	}
	
	if(type==EVENT_STOP_MUSIC)
	{
		stop_fiend_music();
		return 1;
	}



	return 0;
}


/////////////////////////////////
//// CHECK VAR EVENT //////////
/////////////////////////////////

int check_var_events(void)
{
	if(type==EVENT_GLOBAL_VAR_BECOME)
	{
		num = get_global_var_num(string1);
		if(num<0)return -1;

		global_var[num].value = x;
		return 1;
	}

	if(type==EVENT_GLOBAL_VAR_ADD)
	{
		num = get_global_var_num(string1);
		if(num<0)return -1;

		global_var[num].value += x;
		return 1;
	}

	if(type==EVENT_LOCAL_VAR_BECOME)
	{
		num = get_local_var_num(string1);
		if(num<0)return -1;

		map->var[num].value = x;
		return 1;
	}

	if(type==EVENT_LOCAL_VAR_ADD)
	{
		num = get_local_var_num(string1);
		if(num<0)return -1;

		map->var[num].value += x;
		return 1;
	}


	return 0;
}

/////////////////////////////////
//// CHECK MISC EVENT //////////
/////////////////////////////////

int check_misc_events(void)
{
	if(type==EVENT_FADEOUT)
	{
		//make_effect(EFFECT_FADEOUT,0,x,0,0,-1);
		fiend_fadeout(x);
		return 1;
	}

	if(type==EVENT_FADEIN)
	{
		//make_effect(EFFECT_FADEIN,0,x,0,0,-1);
		fiend_fadein(x);
		return 1;
	}

	if(type==EVENT_SAVE)
	{
		save_menu();
		return 1;
	}
	
	if(type==EVENT_OUTSIDE_LIGHT)
	{
		outside_lightlevel = x;
		return 1;
	}

	if(type==EVENT_COMPLETE_GAME)
	{
		game_ended=1;
		game_complete=1;
		return 1;
	}
	
	if(type==EVENT_NOTHING)
	{
		return 1;
	}


	return 0;
}


/////////////////////////////////
//// CHECK AUTOMOVE EVENT //////////
/////////////////////////////////

int check_automove_events(void)
{
	if(type==EVENT_AUTOMOVE_POS)
	{
		auto_move_player(x,y,char_info[0].speed);
		return 1;
	}
	
	if(type==EVENT_AUTOMOVE_PAUSE)
	{
		stop_auto_move();
		return 1;
	}

	if(type==EVENT_AUTOMOVE_RESUME)
	{
		resume_auto_move();
		return 1;
	}

	if(type==EVENT_AUTOMOVE_TOGGLE)
	{
		if(auto_move_active)stop_auto_move;
		else resume_auto_move;
		return 1;
	}

	if(type==EVENT_AUTOMOVE_STOP)
	{
		stop_auto_move();
		return 1;
	}

	return 0;
}

/////////////////////////////////
/////// MAIN _FUNCTION //////////
/////////////////////////////////

int check_event(int t_num,int e_num, int global )
{
	int v;

	init_check_events(t_num, e_num,global);

	v = check_player_events();if(v!=0)return v;
	v = check_enemy_events();if(v!=0)return v;
	v = check_npc_events();if(v!=0)return v;
	v = check_object_events();if(v!=0)return v;
		
	v = check_item_events();if(v!=0)return v;
	v = check_area_events();if(v!=0)return v;
	v = check_link_events();if(v!=0)return v;
	v = check_light_events();if(v!=0)return v;
	v = check_lightlevel_events();if(v!=0)return v;
		
	v = check_message_events();if(v!=0)return v;
	v = check_effect_events();if(v!=0)return v;
	v = check_soundemitor_events();if(v!=0)return v;
	v = check_sound_events();if(v!=0)return v;
	
	v = check_var_events();if(v!=0)return v;
	v = check_misc_events();if(v!=0)return v;
	v = check_automove_events();if(v!=0)return v;

	return 1;
}