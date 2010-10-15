////////////////////////////////////////////////////
// This file contains savegame loading and saving
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <time.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"


//message_vars
extern int message_x,message_y;
extern unsigned int current_char;
extern char text_string[20][400];
extern int active_text_string;
extern int text_string_num;
extern int message_active;

//inventory vars
extern int inventory_is_on;
extern int current_space;
extern int left_space;
extern int menu_is_open;
extern int menu_row;

//automove
extern int auto_move_angle;
extern int auto_move_speed;
extern int auto_move_counter;
extern int auto_move_active;

//player stuff
extern int pushing_an_object;
extern int have_pushed_an_object;
extern int the_push_voice;

//music
extern char current_music[50];
extern int music_is_looping;

//misc
extern int outside_lightlevel;

SAVEDATA savedata;

long int fiend_playtime=0;

extern int current_space;
extern int current_menu;

void save_menu(void)
{
	current_menu = 7;
	inventory_is_on = 1;
	player.active=0;
}



int save_game(char *file, char* name)
{
	FILE *f;
	time_t date;
	int i;
	
	pause_fiend_music();

	f = fopen(file, "wb");
	if(f==NULL)return 0;
	
	//set the savedata struct
	sprintf(savedata.mapfile,"%s", map_file);
	sprintf(savedata.name,"%s", name);

	time(&date);
	sprintf(savedata.date,"%s", ctime(&date));

	savedata.play_time = fiend_playtime;
	//save the savedata struct
	fwrite(&savedata, sizeof(savedata),1,f);

	//save map info we need
	fwrite(map, sizeof(MAP_DATA), 1,f);
	
	fwrite(map->light, sizeof(LIGHT_DATA), map->num_of_lights, f);		
	fwrite(map->object, sizeof(OBJECT_DATA),map->num_of_objects, f);
	fwrite(map->area, sizeof(AREA_DATA),map->num_of_areas, f);
	fwrite(map->link, sizeof(LINK_DATA),map->num_of_links, f);
	fwrite(map->soundemitor, sizeof(SOUNDEMITOR_DATA),map->num_of_soundemitors, f);
	fwrite(map->trigger, sizeof(TRIGGER_DATA),map->num_of_triggers, f);
	
	//save the enemy and npc data
	for(i=0;i<MAX_ENEMY_DATA;i++)
	{
		fwrite(&enemy_data[i],sizeof(ENEMY_DATA),1,f);
		fwrite(&enemy_ai[i],sizeof(ENEMY_AI_DATA),1,f);
	}
	for(i=0;i<MAX_NPC_NUM;i++)
	{
		fwrite(&npc_data[i],sizeof(NPC_DATA),1,f);
		fwrite(&npc_ai[i],sizeof(NPC_AI_DATA),1,f);
	}
	
	//save the player
	fwrite(&player, sizeof(PLAYER_INFO),1,f);

	//save global data
	fwrite(item_data, sizeof(ITEM_DATA),MAX_ITEM_DATA,f);
	fwrite(global_var, sizeof(VARIABLE_DATA),GLOBAL_VAR_NUM,f);
	fwrite(global_trigger, sizeof(TRIGGER_DATA),GLOBAL_TRIGGER_NUM,f);

	//save minor data (particle, missile etc..)
	fwrite(particle_data,sizeof(PARTICLE_DATA),MAX_PARTICLE_DATA,f);
	fwrite(missile_data,sizeof(MISSILE_DATA),MAX_MISSILES,f);
	fwrite(effect_data,sizeof(EFFECT_DATA),MAX_EFFECT_NUM,f);
	fwrite(sound_data,sizeof(SOUND_DATA),MAX_SOUNDS_PLAYING,f);

	//save misc data
	fwrite(beam,sizeof(BEAM),BEAM_NUM,f);
	fwrite(bloodpool,sizeof(BLOODPOOL), BLOODPOOL_NUM,f);
	fwrite(flame_data,sizeof(FLAMEDATA),MAX_FLAME_DATA,f);
	fwrite(shell_data,sizeof(SHELL_DATA),MAX_SHELL_NUM,f);

	//save saved local vars
	fwrite(&saved_var_num, sizeof(saved_var_num),1,f);
	fwrite(saved_local_var,sizeof(SAVED_LOCAL_VAR),MAX_ROOM_NUM,f);
	
	//save saved object
	fwrite(&saved_object_num, sizeof(saved_object_num),1,f);
	fwrite(saved_object,sizeof(SAVED_ROOM_OBJECT),MAX_ROOM_NUM,f);

	//save small variables
	//--message
	fwrite(&message_x, sizeof(message_x),1,f);
	fwrite(&message_y, sizeof(message_y),1,f);
	fwrite(&current_char, sizeof(current_char),1,f);
	fwrite(text_string, sizeof(text_string) ,1,f);
	fwrite(&active_text_string, sizeof(active_text_string),1,f);
	fwrite(&text_string_num,sizeof(text_string_num),1,f);
	fwrite(&message_active,sizeof(message_active),1,f);
	//--inventory
	fwrite(&inventory_is_on, sizeof(inventory_is_on),1,f);
	fwrite(&current_space,sizeof(current_space),1,f);
	fwrite(&left_space,sizeof(left_space),1,f);
	fwrite(&menu_is_open,sizeof(menu_is_open),1,f);
	fwrite(&menu_row,sizeof(menu_row),1,f);
	//--automove
	fwrite(&auto_move_angle,sizeof(auto_move_angle),1,f);
	fwrite(&auto_move_speed,sizeof(auto_move_speed),1,f);
	fwrite(&auto_move_counter,sizeof(auto_move_counter),1,f);
	fwrite(&auto_move_active,sizeof(auto_move_active),1,f);
	//--player stuff
	fwrite(&pushing_an_object, sizeof(pushing_an_object),1,f);
	fwrite(&have_pushed_an_object, sizeof(have_pushed_an_object),1,f);
	fwrite(&the_push_voice, sizeof(the_push_voice),1,f);
	//--misc
	fwrite(&map_x,sizeof(map_x),1,f);
	fwrite(&map_y,sizeof(map_y),1,f);
	fwrite(&outside_lightlevel,sizeof(outside_lightlevel),1,f);
	
	//--music
	fwrite(current_music,sizeof(current_music),1,f);
	fwrite(&music_is_looping,sizeof(music_is_looping),1,f);
		
	fclose(f);

	resume_fiend_music();

	return 1;
}





int load_game(char *file)
{
	ENEMY_DATA temp_enemy_data;
	ENEMY_AI_DATA temp_enemy_ai;
	NPC_DATA temp_npc_data;
	NPC_AI_DATA temp_npc_ai;
	VARIABLE_DATA temp_global_var;
	TRIGGER_DATA temp_global_trigger;
	ITEM_DATA temp_item_data;	
	
	MAP_DATA *temp_map;
	FILE *f;
	
	char temp_music[50];

	int i;
		
	//get map file name...
	f = fopen(file, "rb");
	if(f==NULL)return 0;
	
	fread(&savedata, sizeof(SAVEDATA),1,f);

	fclose(f);

	fiend_playtime = savedata.play_time;

	
	if(strcmp(savedata.mapfile,map_file)!=0)
	{
		load_edit_map(map,savedata.mapfile);
	}

	
	f = fopen(file, "rb");
	if(f==NULL)return 0;
	
	stop_all_sounds();
	stop_fiend_music();

		
	//load save struct
	fread(&savedata, sizeof(savedata),1,f);

	//load map struct
	temp_map = calloc(sizeof(MAP_DATA),1);
	fread(temp_map, sizeof(MAP_DATA), 1,f);
	
	map->light_level = temp_map->light_level;
		
	memcpy(map->var, temp_map->var, sizeof(VARIABLE_DATA)*LOCAL_VAR_NUM);
	
	free(temp_map);

	fread(map->light, sizeof(LIGHT_DATA), map->num_of_lights, f);		
	fread(map->object, sizeof(OBJECT_DATA),map->num_of_objects, f);
	fread(map->area, sizeof(AREA_DATA),map->num_of_areas, f);
	fread(map->link, sizeof(LINK_DATA),map->num_of_links, f);
	fread(map->soundemitor, sizeof(SOUNDEMITOR_DATA),map->num_of_soundemitors, f);
	fread(map->trigger, sizeof(TRIGGER_DATA),map->num_of_triggers, f);
	

	//load the enemy
	for(i=0;i<MAX_ENEMY_DATA;i++)
	{
		fread(&temp_enemy_data,sizeof(ENEMY_DATA),1,f);
		fread(&temp_enemy_ai,sizeof(ENEMY_AI_DATA),1,f);
		if(enemy_data[i].used && temp_enemy_data.used && enemy_data[i].type==temp_enemy_data.type && strcmp(temp_enemy_data.name,enemy_data[i].name)==0)
		{
			enemy_data[i].energy = temp_enemy_data.energy;
			enemy_data[i].active = temp_enemy_data.active;
			enemy_data[i].dead = temp_enemy_data.dead;
			enemy_data[i].was_dead = temp_enemy_data.was_dead;

			enemy_data[i].x = temp_enemy_data.x;
			enemy_data[i].y = temp_enemy_data.y;
			enemy_data[i].angle = temp_enemy_data.angle;
			
			enemy_data[i].nextframe = temp_enemy_data.nextframe;
			enemy_data[i].frame = temp_enemy_data.frame;
			enemy_data[i].action = temp_enemy_data.action;
			
			enemy_data[i].current_mission = temp_enemy_data.current_mission;
			enemy_data[i].current_movement_path = temp_enemy_data.current_movement_path;

			strcpy(enemy_data[i].map_name, temp_enemy_data.map_name); 
			
			memcpy(&enemy_ai[i],&temp_enemy_ai,sizeof(ENEMY_AI_DATA));
		}
	}
	
	//load the npc
	for(i=0;i<MAX_NPC_NUM;i++)
	{
		fread(&temp_npc_data,sizeof(NPC_DATA),1,f);
		fread(&temp_npc_ai,sizeof(NPC_AI_DATA),1,f);
		if(npc_data[i].used && temp_npc_data.used && temp_npc_data.type==npc_data[i].type && strcmp(npc_data[i].name,temp_npc_data.name)==0 )
		{
			npc_data[i].energy = temp_npc_data.energy;
			npc_data[i].active = temp_npc_data.active;
			npc_data[i].dead = temp_npc_data.dead;
			npc_data[i].was_dead = temp_npc_data.was_dead;

			npc_data[i].x = temp_npc_data.x;
			npc_data[i].y = temp_npc_data.y;
			npc_data[i].angle = temp_npc_data.angle;
			
			npc_data[i].next_frame = temp_npc_data.next_frame;
			npc_data[i].frame = temp_npc_data.frame;
			npc_data[i].action = temp_npc_data.action;

			npc_data[i].current_movement_path = temp_npc_data.current_movement_path;
			
			npc_data[i].current_dialog = temp_npc_data.current_dialog;

			strcpy(npc_data[i].map_name, temp_npc_data.map_name); 
			
			memcpy(&npc_ai[i],&temp_npc_ai,sizeof(NPC_AI_DATA));
		}
	}
	
	//load the player
	fread(&player, sizeof(PLAYER_INFO),1,f);

	//load global data
	for(i=0;i<MAX_ITEM_DATA;i++)
	{
		fread(&temp_item_data, sizeof(ITEM_DATA),1,f);
		if(item_data[i].used && temp_item_data.used && temp_item_data.type==item_data[i].type)
		{
			item_data[i].value = temp_item_data.value;		
			item_data[i].active = temp_item_data.active;		
			item_data[i].picked_up = temp_item_data.picked_up;
			
			item_data[i].x = temp_item_data.x;		
			item_data[i].y = temp_item_data.y;
			item_data[i].sp1 = temp_item_data.sp1;
		}
	}
	for(i=0;i<GLOBAL_VAR_NUM;i++)
	{
		fread(&temp_global_var, sizeof(VARIABLE_DATA),1,f);
		if(strcmp(global_var[i].name,"null")!=0 && strcmp(temp_global_var.name,"null")!=0)
			memcpy(&global_var[i],&temp_global_var,sizeof(VARIABLE_DATA));
	}
	
	for(i=0;i<GLOBAL_TRIGGER_NUM;i++)
	{
		fread(&temp_global_trigger, sizeof(TRIGGER_DATA),1,f);
		if(global_trigger[i].event[0].used && temp_global_trigger.event[0].used && global_trigger[i].event[0].type ==temp_global_trigger.event[0].type)
		{
			global_trigger[i].active = temp_global_trigger.active;
		}
	}
	

	//save minor data (particle, missile etc..)
	fread(particle_data,sizeof(PARTICLE_DATA),MAX_PARTICLE_DATA,f);
	fread(missile_data,sizeof(MISSILE_DATA),MAX_MISSILES,f);
	fread(effect_data,sizeof(EFFECT_DATA),MAX_EFFECT_NUM,f);
	fread(sound_data,sizeof(SOUND_DATA),MAX_SOUNDS_PLAYING,f);

	
	//load misc data
	fread(beam,sizeof(BEAM),BEAM_NUM,f);
	fread(bloodpool,sizeof(BLOODPOOL), BLOODPOOL_NUM,f);
	fread(flame_data,sizeof(FLAMEDATA),MAX_FLAME_DATA,f);
	fread(shell_data,sizeof(SHELL_DATA),MAX_SHELL_NUM,f);

	//load saved local vars
	fread(&saved_var_num, sizeof(saved_var_num),1,f);
	fread(saved_local_var,sizeof(SAVED_LOCAL_VAR),MAX_ROOM_NUM,f);
	
	//load saved objects
	fread(&saved_object_num, sizeof(saved_object_num),1,f);
	fread(saved_object,sizeof(SAVED_ROOM_OBJECT),MAX_ROOM_NUM,f);
	
	//load small variables
	//--message
	fread(&message_x, sizeof(message_x),1,f);
	fread(&message_y, sizeof(message_y),1,f);
	fread(&current_char, sizeof(current_char),1,f);
	fread(text_string, sizeof(text_string) ,1,f);
	fread(&active_text_string, sizeof(active_text_string),1,f);
	fread(&text_string_num,sizeof(text_string_num),1,f);
	fread(&message_active,sizeof(message_active),1,f);
	//--inventory
	fread(&inventory_is_on, sizeof(inventory_is_on),1,f);
	fread(&current_space,sizeof(current_space),1,f);
	fread(&left_space,sizeof(left_space),1,f);
	fread(&menu_is_open,sizeof(menu_is_open),1,f);
	fread(&menu_row,sizeof(menu_row),1,f);
	//--automove
	fread(&auto_move_angle,sizeof(auto_move_angle),1,f);
	fread(&auto_move_speed,sizeof(auto_move_speed),1,f);
	fread(&auto_move_counter,sizeof(auto_move_counter),1,f);
	fread(&auto_move_active,sizeof(auto_move_active),1,f);
	//--player stuff
	fread(&pushing_an_object, sizeof(pushing_an_object),1,f);
	fread(&have_pushed_an_object, sizeof(have_pushed_an_object),1,f);
	fread(&the_push_voice, sizeof(the_push_voice),1,f);
	//--misc
	fread(&map_x,sizeof(map_x),1,f);
	fread(&map_y,sizeof(map_y),1,f);
	fread(&outside_lightlevel,sizeof(outside_lightlevel),1,f);
	
	//--music
	fread(temp_music,sizeof(temp_music),1,f);
	fread(&music_is_looping,sizeof(music_is_looping),1,f);

	//allegro_message("%s %d", temp_music, music_is_looping);
	
	
	if(sound_is_on && music_is_looping)
		play_fiend_music(temp_music,1);

	strcpy(current_music,temp_music);

	//some sound init..
	
	if(sound_is_on)
	{
		for(i=0;i<MAX_SOUNDS_PLAYING;i++)
			if(sound_data[i].used)
			{
				if(sound_data[i].loop)
				{
					//sound_data[i].voice_num=FSOUND_PlaySound(FSOUND_FREE, sound_info[sound_data[i].sound_num].sound);
					FMOD_System_PlaySound(fmod_system, FMOD_CHANNEL_FREE, sound_info[sound_data[i].sound_num].sound, 0, &sound_data[i].voice_num);
					FMOD_Channel_SetLoopCount(sound_data[i].voice_num, FMOD_LOOP_NORMAL);
		
					sound_data[i].playing=1;
				}
				else
				{
					sound_data[i].used=0;
				}
			}
	}
	
	fclose(f);

	get_current_map_objects();
	speed_counter=0;


	return 1;
}


