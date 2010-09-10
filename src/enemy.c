////////////////////////////////////////////////////
// This file contains enemy loading and drawing funcs 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "grafik4.h"
#include "rotate_sprite.h"


#include "fiend.h"
#include "enemy.h"


//the file name that enemy data is loaded from
char enemy_filename[80] = "maps\\enemy.inf";


ENEMY_DATA *enemy_data;
ENEMY_INFO *enemy_info;

//variables
int num_of_enemys=0;



//load the enemies
int load_enemys(void)
{
	FILE *f;

	
	BMP_ARRAY *temp_data;
	
	char buffer[50];
	char sprite_file_name[100][15];
    char info_file_name[100][15];
	int num_of_frames=1;
	char *file_path ="graphic\\enemies\\";
	char final_path[40];
	int i,j,k,r,g,b;
	
	enemy_info = calloc(sizeof(ENEMY_INFO), MAX_ENEMY_INFO);

	f = fopen("graphic\\enemies\\enemies.txt", "r");      //Load the Info file
	
	if(f==NULL)
	{sprintf(fiend_errorcode,"couldn't load enemys.txt");return 1;} //error testing...
	
	
	while(fscanf(f, "%s %s\n", sprite_file_name[num_of_enemys], info_file_name[num_of_enemys])!=EOF)
	{
		num_of_enemys++;	
	}

	fclose(f);
    
	
	for(i=0;i<num_of_enemys;i++)
	{
		
		//----------BEGIN GET INFO-----------------------
		strcpy(final_path, file_path);
		strcat(final_path, info_file_name[i]);    //Get the info.

		f = fopen(final_path, "r");
		if(f==NULL){ sprintf(fiend_errorcode,"couldn't load %s",final_path);return 1;}//error testing...

		fscanf(f,"%s %d\n",buffer,&num_of_frames);//get all the info.....
		enemy_info[i].num_of_frames = num_of_frames;//number of frames
		
		fscanf(f,"%s %s\n",buffer, enemy_info[i].name);//get the name

		//get witdh, height, number of angles and the speed
		fscanf(f,"%s %d %s %d %s %d %s %d %s %d %s %f %s %d %s %d\n\n",buffer, &enemy_info[i].w, buffer, &enemy_info[i].h,buffer, &enemy_info[i].hit_w, buffer, &enemy_info[i].hit_h, buffer, &enemy_info[i].angles, buffer, &enemy_info[i].speed, buffer, &enemy_info[i].energy, buffer, &enemy_info[i].weight);

		//get stats...
		fscanf(f,"%s %d %s %d %s %d %s %d\n",buffer, &enemy_info[i].eye_type, buffer, &enemy_info[i].can_smell, buffer, &enemy_info[i].can_hear, buffer, &enemy_info[i].hear_range);
		fscanf(f,"%s %d %s %d %s %d %s %d %s %d %s %d\n\n",buffer, &enemy_info[i].run_when_hurt, buffer, &enemy_info[i].attack_from_behind, buffer, &enemy_info[i].attack_in_group, buffer, &enemy_info[i].use_a_star,buffer,&enemy_info[i].care_if_hurt,buffer,&enemy_info[i].fov);

		//weapon_stats
		fscanf(f,"%s %s %s %d %s %d %s %d\n",buffer, &enemy_info[i].weapon_name[0], buffer, &enemy_info[i].weapon_range[0], buffer, &enemy_info[i].weapon_random[0], buffer, &enemy_info[i].weapon_length[0]);
		fscanf(f,"%s %s %s %d %s %d %s %d\n",buffer, &enemy_info[i].weapon_name[1], buffer, &enemy_info[i].weapon_range[1], buffer, &enemy_info[i].weapon_random[1], buffer, &enemy_info[i].weapon_length[1]);
		fscanf(f,"%s %s %s %d %s %d %s %d\n\n",buffer, &enemy_info[i].weapon_name[2], buffer, &enemy_info[i].weapon_range[2], buffer, &enemy_info[i].weapon_random[2], buffer, &enemy_info[i].weapon_length[2]);
		
		//other stats...
		fscanf(f,"%s %d %s %d %s %d\n",buffer, &enemy_info[i].move_random, buffer, &enemy_info[i].move_random_length, buffer, &enemy_info[i].move_random_time);
		fscanf(f,"%s %s %s %d\n\n",buffer, &enemy_info[i].death_explosion, buffer, &enemy_info[i].death_explosion_num);

		//blood pool cords
		fscanf(f,"%s %d %s %d\n",buffer, &enemy_info[i].blood_x, buffer, &enemy_info[i].blood_y);
		fscanf(f,"%s %d %d %d %s %d\n\n",buffer,&r,&g,&b , buffer, &enemy_info[i].show_bloodpool);
		
		enemy_info[i].blood_color = makecol(r,g,b);
		
		//misc stuff
		fscanf(f,"%s %d %s %f\n\n",buffer, &enemy_info[i].under_player, buffer, &enemy_info[i].regenerate);

		//Get the sounds
		fscanf(f,"%s %s %s %s\n",buffer, enemy_info[i].sound_death, buffer, enemy_info[i].sound_hurt);
		fscanf(f,"%s %d\n",buffer, &enemy_info[i].sound_ambient_num);
		fscanf(f,"%s %s %s %s %s %s %s %s %s %s\n",buffer, &enemy_info[i].sound_ambient[0], buffer, &enemy_info[i].sound_ambient[1], buffer, &enemy_info[i].sound_ambient[2], buffer, &enemy_info[i].sound_ambient[3], buffer, &enemy_info[i].sound_ambient[4]);
		fscanf(f,"%s %d %s %d %s %d %s %d %s %d\n\n",buffer, &enemy_info[i].sound_ambient_random[0], buffer, &enemy_info[i].sound_ambient_random[1], buffer, &enemy_info[i].sound_ambient_random[2], buffer, &enemy_info[i].sound_ambient_random[3], buffer, &enemy_info[i].sound_ambient_random[4]);
		
		//steps
		fscanf(f,"%s %s %s %f\n",buffer, enemy_info[i].step_sound,buffer, &enemy_info[i].run_add);
		fscanf(f,"%s %d %s %d %s %d %s %d\n\n",buffer, &enemy_info[i].walk_step1, buffer, &enemy_info[i].walk_step2,buffer, &enemy_info[i].run_step1, buffer, &enemy_info[i].run_step2);
		
		
		//get the number of animations
		fscanf(f,"%s %d",buffer, &enemy_info[i].num_of_animations);

				
		//read the animation in fo first you get the name of the animation
		//then you read the frames til a '-1' is encounterd
		for(j=0;j<enemy_info[i].num_of_animations;j++)
		{
			fscanf(f,"%s",enemy_info[i].animation[j].name);
			k=-1;
			do
			{
				k++;
				fscanf(f,"%d ",&enemy_info[i].animation[j].frame[k],buffer);
			}while(enemy_info[i].animation[j].frame[k]!=-1);
			
		}

		fclose(f);
		//-------END GET INFO---------------

		
		
		//--------BEGIN GET DATA------------------
		strcpy(final_path, file_path);
		strcat(final_path, sprite_file_name[i]);   //Get the tile data .

														 
	    temp_data = load_bmp_array(final_path,enemy_info[i].num_of_frames);//load the graphic
		if(temp_data==NULL){sprintf(fiend_errorcode,"couldn't load %s",final_path);return 1;}//error testing...

				
		for(j=0;j<enemy_info[i].num_of_frames;j++)//For each of the diffrent pics....
		{
			crop_picdata(temp_data[j].dat, &enemy_info[i].pic[j]);
		}
		
		unload_bmp_array(temp_data);
		//----------END GET DATA-------------------

	
	}






	return 0;
}


//release all the characters from memory
void release_enemys(void)
{
	int i,j;

	for(i=0;i<num_of_enemys;i++)
		for(j=0;j<enemy_info[i].num_of_frames;j++)
		{
			destroy_bitmap(enemy_info[i].pic[j].data);			
		}
	
			
	free(enemy_info);
}








//initaialze det enemy stuff and clear the enemy struct
int init_enemy_data(void)
{
	int i;

	enemy_data = calloc(sizeof(ENEMY_DATA),MAX_ENEMY_DATA);

	for(i=0;i<MAX_ENEMY_DATA;i++)
	{
		enemy_data[i].used=0;
		enemy_data[i].active=0;
		
		strcpy(enemy_data[i].name,"nomame");
		strcpy(enemy_data[i].map_name,"");
	}
	
	return 0;
}



//load the npc struct 
int load_enemy_data(void)
{
	FILE *f;
	int i;

	f = fopen(enemy_filename,"rb");
	if(f==NULL)
	{
		for(i=0;i<MAX_ENEMY_DATA;i++)
		{
				enemy_data[i].used=0;
				enemy_data[i].active=0;
		
				strcpy(enemy_data[i].name,"nomame");
				strcpy(enemy_data[i].map_name,"");
		}
	
		return 1; //when you start fresh you have no npc:s so you can't give an error code here
	}
	
	fread(enemy_data, sizeof(ENEMY_DATA), MAX_ENEMY_DATA, f);

	fclose(f);

	return 1;
}



//save the npc struct
int save_enemy_data(void)
{
	FILE *f;

	f = fopen(enemy_filename,"wb");
	if(f==NULL)return 0;

	fwrite(enemy_data, sizeof(ENEMY_DATA), MAX_ENEMY_DATA, f);

	fclose(f);

	return 1;
}


void release_enemy_data(void)
{
	free(enemy_data);
}




//Draw a character to dest....
void draw_fiend_enemy(BITMAP *dest,ENEMY_INFO *temp,int x, int y,int action,int frame, float angle)
{
	int num = temp->animation[action].frame[frame];
		
	//pivot_fiend_sprite(dest, temp->pic[num].data,x,y,temp->pic[num].center_x,temp->pic[num].center_y,angle,FIEND_DRAW_MODE_TRANS);
	pivot_sprite(dest, temp->pic[num].data,x,y,temp->pic[num].center_x,temp->pic[num].center_y,degree_to_fixed(angle));
	//rotate_sprite(dest, temp->pic[num].data,x-temp->pic[num].data->w/2,y-temp->pic[num].data->h/2,f_angle);
	

}



//what number had the action name? 
int enemy_action(ENEMY_INFO *temp, char *name)
{
	int i;

	for(i=0;i<temp->num_of_animations;i++)
	{
		if(strcmp(name,temp->animation[i].name)==0)
			return i;

	}

	return 0;
}






