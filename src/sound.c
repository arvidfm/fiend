////////////////////////////////////////////////////
// This file contains sound loading
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <allegro.h>

#include <stdio.h>
#include <string.h>

#include "fiend.h"
#include "grafik4.h"

#define MAX_SOUNDS 200

SOUND_INFO *sound_info;

SOUND_DATA *sound_data;

int num_of_sounds=0;



int load_sounds(void)
{
	FILE *f;

	char sound_file_name[70];
   	char *file_path ="sound\\";
	char final_path[80];
	char *name;
	char name2[40];
	int i;
	unsigned int j,k;
	
	
	sound_info = calloc(sizeof(SOUND_INFO), MAX_SOUNDS);

	f = fopen("sound\\sounds.txt", "r");      //Load the Info file
	
	if(f==NULL)
	{sprintf(fiend_errorcode,"couldn't load sounds.txt");return 1;} //error testing...
    

	
	while(fscanf(f, "%s %d %d\n", sound_file_name,&sound_info[num_of_sounds].volume,&sound_info[num_of_sounds].num)!=EOF)  //Get the number of tiles and the file names
	{
		
		//make the game of the sound
		name = get_filename(sound_file_name);
		strcpy(name2,name);
		k = strlen(name2);
		for(j=0;j<k;j++)
		{
			if(name2[j]=='.')
			{
				name2[j]=0;
				break;
			}
		}
		strcpy(sound_info[num_of_sounds].name,name2);
		
		
		//get the sound data
		strcpy(final_path, file_path);
		strcat(final_path, sound_file_name);
		
		sound_info[num_of_sounds].sound = FSOUND_Sample_Load(FSOUND_FREE,final_path,FSOUND_2D,0);//load the sound
		
		if(sound_info[num_of_sounds].sound==NULL){allegro_message("couldn't load %s",final_path);exit(-1);}//error testing...
		
		num_of_sounds++;
	}

	fclose(f);

	sound_data = calloc(sizeof(SOUND_DATA),MAX_SOUNDS_PLAYING);
   
	for(i=0;i<MAX_SOUNDS_PLAYING;i++)
		sound_data[i].used=0;


	return 0;
}



void free_sounds(void)
{
 int i;

 for(i=0;i<num_of_sounds;i++)
	 FSOUND_Sample_Free(sound_info[i].sound);


 free(sound_info);



}

