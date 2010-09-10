////////////////////////////////////////////////////
// This file contains weapon loading
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>

#include <stdio.h>
#include <string.h>

#include "..//fiend.h"
#include "..//grafik4.h"



WEAPON_INFO *weapon_info;

MISSILE_DATA *missile_data;

int num_of_weapons=0;



void shoot_weapon(char *name, float x, float y, float angle)
{
	int num = get_weapon_num(name);


	
}




int get_weapon_num(char *name)
{
	int i;

	for(i=0;i<num_of_weapons;i++)
	{
		if(strcmp(weapon_info[i].name,name)==0)
			return i;
	}
	return 0;
}

int load_weapons(void)
{
	FILE *f;

	char buffer[50];
	char info_file_name[100][70];
	int num_of_tiles=1;
	char *file_path ="data\\weapons\\";
	char final_path[40];
	int i;

	
	weapon_info = calloc(sizeof(WEAPON_INFO), MAX_WEAPONS);
	missile_data = calloc(sizeof(MISSILE_DATA), MAX_MISSILES);

	for(i=0;i<MAX_MISSILES;i++)
		missile_data[i].used=0;


	f = fopen("data\\weapons\\weapons.txt", "r");      //Load the Info file
	
	if(f==NULL)
	{sprintf(fiend_errorcode,"couldn't load weapons.txt");return 1;} //error testing...
    

	
	while(fscanf(f, "%s\n",info_file_name[num_of_weapons])!=EOF)//Get the info
	{
		num_of_weapons++;
	}

	fclose(f);

	
	
	
	
	for(i=0;i<num_of_weapons;i++)
	{
		strcpy(final_path, file_path);
		strcat(final_path, info_file_name[i]);    //Get the info.

		f = fopen(final_path, "r");
		if(f==NULL){sprintf(fiend_errorcode,"couldn't load %s",final_path);return 1;}//error testing...
		
		//// Weapon Props
		fscanf(f, "%s %s %s %s\n", buffer, weapon_info[i].name, buffer, weapon_info[i].player_action); 
		fscanf(f, "%s %d\n",buffer, &weapon_info[i].weight);
		
		fscanf(f, "%s %d %s %d %s %d %s %d\n", buffer, &weapon_info[i].flame_num, buffer, &weapon_info[i].flame_length, buffer, &weapon_info[i].flame_x, buffer, &weapon_info[i].flame_y); 
		fscanf(f, "%s %d %s %d\n", buffer, &weapon_info[i].light_r, buffer, &weapon_info[i].light_c);
				
		fscanf(f, "%s %s %s %d %s %d\n", buffer, weapon_info[i].sound, buffer, &weapon_info[i].shot_length, buffer, &weapon_info[i].silent); 
		fscanf(f, "%s %d\n", buffer, &weapon_info[i].max_ammo); 
				
		fscanf(f, "%s %d %s %f\n", buffer, &weapon_info[i].num_of_missiles, buffer, &weapon_info[i].missile_angle);
		fscanf(f, "%s %d %s %d\n", buffer, &weapon_info[i].missile_x, buffer, &weapon_info[i].missile_y);
		
		fscanf(f, "%s %s\n", buffer, weapon_info[i].shell_name);
		fscanf(f, "%s %d %s %d %s %d %s %d\n\n", buffer, &weapon_info[i].shell_x, buffer, &weapon_info[i].shell_y, buffer, &weapon_info[i].shell_angle, buffer, &weapon_info[i].shell_after_shot_length); 
		
		/////Missile Props
		
		fscanf(f, "%s %s\n", buffer, weapon_info[i].missile_name); 
		fscanf(f, "%s %f\n", buffer, &weapon_info[i].speed); 
		fscanf(f, "%s %d %s %f\n", buffer, &weapon_info[i].range, buffer, &weapon_info[i].range_dec);
		
		fscanf(f, "%s %d\n", buffer, &weapon_info[i].beam);
		
		fscanf(f, "%s %d %s %d %s %d %s %f \n", buffer, &weapon_info[i].min_damage, buffer, &weapon_info[i].max_damage, buffer, &weapon_info[i].damage_range, buffer, &weapon_info[i].damage_range_dec); 
		fscanf(f, "%s %s %s %d\n", buffer, weapon_info[i].explosion_name, buffer, &weapon_info[i].num_of_explosions); 

		fscanf(f, "%s %s %s %d\n", buffer, weapon_info[i].particle_name, buffer, &weapon_info[i].particle_num); 

		fscanf(f, "%s %d\n",buffer, &weapon_info[i].strength); 
		fscanf(f, "%s %d %s %d\n",buffer, &weapon_info[i].missile_light_r, buffer, &weapon_info[i].missile_light_c); 
		
		fscanf(f, "%s %d\n", buffer, &weapon_info[i].hit_enemy); 
		
		

		fclose(f);

	}


	return 0;
}


void release_weapons(void)
{
	free(missile_data);
	free(weapon_info);
}
