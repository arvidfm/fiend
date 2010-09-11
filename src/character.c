////////////////////////////////////////////////////
// This file Contains Character loading and drawing funcs 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#include "grafik4.h"

#include "fiend.h"
#include "character.h"
#include "rotate_sprite.h"




//Info
CHARACTER_INFO *char_info;

//variables
int num_of_characters=0;




//Loads all the character graphics... 
int load_characters(void)
{
	FILE *f;

	
	BMP_ARRAY *temp_data;
	
	char buffer[50];
	char sprite_file_name[100][15];
    char info_file_name[100][15];
	int num_of_frames=1;
	char *file_path ="graphic/characters/";
	char final_path[40];
	int i,j,k;
	
	
	char_info = calloc(sizeof(CHARACTER_INFO), MAX_CHARACTERS);

	f = fopen("graphic/characters/characters.txt", "r");      //Load the Info file
	
	if(f==NULL)
	{sprintf(fiend_errorcode,"couldn't find characters.txt");return 1;} //error testing...
	
	
	while(fscanf(f, "%s %s\n", sprite_file_name[num_of_characters], info_file_name[num_of_characters])!=EOF)
	{
		num_of_characters++;	
	}

	fclose(f);
    

	for(i=0;i<num_of_characters;i++)
	{
		
		//----------BEGIN GET INFO-----------------------
		strcpy(final_path, file_path);
		strcat(final_path, info_file_name[i]);    //Get the info.

		f = fopen(final_path, "r");
		if(f==NULL){sprintf(fiend_errorcode,"couldn't load %s",final_path);return 1;}//error testing...

		fscanf(f,"%s %d\n",buffer,&num_of_frames);//get all the info.....
		char_info[i].num_of_frames = num_of_frames;//number of frames
		
		fscanf(f,"%s %s\n",buffer, char_info[i].name);//get the name

		//get witdh, height, number of angles and the speed
		fscanf(f,"%s %d %s %d %s %d %s %d %s %d %s %f %s %d %s %d\n",buffer, &char_info[i].w, buffer, &char_info[i].h,buffer, &char_info[i].hit_w, buffer, &char_info[i].hit_h, buffer, &char_info[i].angles, buffer, &char_info[i].speed, buffer, &char_info[i].energy, buffer, &char_info[i].weight);

		//Get the sounds
		fscanf(f,"%s %s %s %s %s %s\n",buffer, char_info[i].sound_death, buffer, char_info[i].sound_hurt, buffer, char_info[i].sound_ambient);
		
		//blood stain...
		fscanf(f,"%s %d %s %d %s %f\n",buffer, &char_info[i].blood_x, buffer, &char_info[i].blood_y,buffer,&char_info[i].run_add);
		
		//Get step info
		fscanf(f,"%s %d %s %d %s %d %s %d\n",buffer, &char_info[i].walk_step1, buffer, &char_info[i].walk_step2, buffer, &char_info[i].run_step1, buffer, &char_info[i].run_step2);

		//get the number of animations
		fscanf(f,"%s %d",buffer, &char_info[i].num_of_animations);
		
		//read the animation in fo first you get the name of the animation
		//then you read the frames til a '-1' is encounterd
		for(j=0;j<char_info[i].num_of_animations;j++)
		{
			fscanf(f,"%s",char_info[i].animation[j].name);
			k=-1;
			do
			{
				k++;
				fscanf(f,"%d ",&char_info[i].animation[j].frame[k],buffer);
			}while(char_info[i].animation[j].frame[k]!=-1);
			
		}

		fclose(f);
		//-------END GET INFO---------------

		
		
		//--------BEGIN GET DATA------------------
		strcpy(final_path, file_path);
		strcat(final_path, sprite_file_name[i]);   //Get the tile data .

		
	    temp_data = load_bmp_array(final_path,char_info[i].num_of_frames);//load the graphic
		if(temp_data==NULL){sprintf(fiend_errorcode,"couldn't load %s",final_path);return 1;}//error testing...

		
		for(j=0;j<char_info[i].num_of_frames;j++)//For each of the diffrent pics....
		{
			crop_picdata(temp_data[j].dat, &char_info[i].pic[j]);
		}
		
		unload_bmp_array(temp_data);
		//----------END GET DATA-------------------

	
	}






	return 0;
}



//release all the characters from memory
void release_characters(void)
{
	int i,j;

	for(i=0;i<num_of_characters;i++)
		for(j=0;j<char_info[i].num_of_frames;j++)
		{
			destroy_bitmap(char_info[i].pic[j].data);			
		}
	
			
	free(char_info);
}







//Draw a character to dest....
void draw_fiend_char(BITMAP *dest,CHARACTER_INFO *temp,int x, int y,int action,int frame, float angle)
{
	int num = temp->animation[action].frame[frame];
	
	pivot_sprite(dest, temp->pic[num].data,x,y,temp->pic[num].center_x,temp->pic[num].center_y,degree_to_fixed(angle));
	//pivot_fiend_sprite(dest, temp->pic[num].data,x,y,temp->pic[num].center_x,temp->pic[num].center_y,angle,FIEND_DRAW_MODE_TRANS);
	//rotate_fiend_sprite(dest, temp->pic[num].data,x,y,angle,FIEND_DRAW_MODE_TRANS);
}



//what number had the action name? 
int char_action(CHARACTER_INFO *temp, char *name)
{
	int i;

	for(i=0;i<temp->num_of_animations;i++)
	{
		if(strcmp(name,temp->animation[i].name)==0)
			return i;

	}

	return 0;
}



