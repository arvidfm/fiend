////////////////////////////////////////////////////
// This file contains sound playing and updating
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>


#include <stdio.h>
#include <string.h>

#include "..//fiend.h"
#include "..//grafik4.h"




extern SOUND_INFO *sound_info;




//calculate pan and volume
void calc_sound_prop(int x,int y,int*vol, int*pan, int s_vol)
{
 float dist, x_dist;
 	
 dist = distance(x,y,player.x,player.y);
 
 if(dist>HEAR_RANGE)
 {
	 *vol=0;
	 *pan=128;
	  return;
 }

 *vol = s_vol - s_vol * (dist/HEAR_RANGE);

 if(*vol<0)*vol=0;

 x_dist=x-(int)player.x;
 *pan = 128 +(127*(x_dist/HEAR_RANGE));
}





//////////////////////////////////////////////////
//////////// PLAY A SOUND ////////////////////////
//////////////////////////////////////////////////

int play_fiend_sound(char *name, int x, int y,int lower_at_dist,int loop,int priority)
{
	int i,num;
	int found_sound=0;
	int found_data=0;
	int vol, pan;
	int temp;


	if(!sound_is_on)return -1;
	
	///////// Find The sound number ////////////
	
	for(i=0;i<num_of_sounds;i++)
		if(strcmp(sound_info[i].name,name)==0)
		{
			found_sound=1;
			num=i;
			break;
		}
	if(!found_sound)return -1;

	//if the sound has more then one alternative (num>1) randomize between em
	if(sound_info[num].num>1)
	{
		temp = RANDOM(0,sound_info[num].num-1);
		num+=temp;
	}
	
	for(i=0;i<MAX_SOUNDS_PLAYING;i++)
		if(sound_data[i].used==0)
		{
			found_data=1;
			break;
		}

	if(!found_data)return -1;
	
	///////// Set the sound data ////////////
	
	sound_data[i].used=1;
	sound_data[i].sound_num=num;
	sound_data[i].priority=priority;
	sound_data[i].x=x;
	sound_data[i].y=y;
	sound_data[i].loop=loop;
	sound_data[i].lower_at_dist=lower_at_dist;
	sound_data[i].playing=0;
	sound_data[i].vol=sound_info[sound_data[i].sound_num].volume;
	
	
	///////// Play the sound and set propreties //
	
	//sound_data[i].voice_num=allocate_voice(sound_info[num].sound);
				
	if(!sound_data[i].lower_at_dist)
	{
		vol = sound_data[i].vol;
		pan = 128;
	}
	else
		calc_sound_prop(sound_data[i].x,sound_data[i].y,&vol, &pan, sound_info[sound_data[i].sound_num].volume);
				
	
	FSOUND_Sample_SetDefaults(sound_info[num].sound,-1,vol,pan,sound_data[i].priority);
	
	if(sound_data[i].loop)
		FSOUND_Sample_SetLoopMode(sound_info[num].sound,FSOUND_LOOP_NORMAL);

	
	sound_data[i].voice_num=FSOUND_PlaySound(FSOUND_FREE, sound_info[num].sound);
	if(sound_data[i].voice_num==-1)return -1;
	
	sound_data[i].playing=1;
	
	return 	i;	
}




//////////////////////////////////////////////////
////////////// UPDATE THE SOUNDS /////////////////
///////////////////////////////////////////////////
 
void update_sound(void)
{
	int i;
	int vol, pan;
	
	if(!sound_is_on)return;
	
	for(i=0;i<MAX_SOUNDS_PLAYING;i++)
	{
		if(sound_data[i].used)
		{
			
			if(FSOUND_IsPlaying(sound_data[i].voice_num)==FALSE)
			{
				FSOUND_StopSound(sound_data[i].voice_num);
				sound_data[i].used=0;
			}
			else
			{
				if(!sound_data[i].lower_at_dist)
				{
					vol = sound_data[i].vol;
					pan = 128;
				}
				else
					calc_sound_prop(sound_data[i].x,sound_data[i].y,&vol, &pan, sound_info[sound_data[i].sound_num].volume);
				
								
				FSOUND_SetVolume(sound_data[i].voice_num, vol);
				FSOUND_SetPan(sound_data[i].voice_num, pan);

			}

		}
	}
}



/////////////////////////////////////////////////////
//////////// STOP ALL SOUNDS FUNCTIONS ////////////////
/////////////////////////////////////////////////////


void stop_sound_num(int num)
{
	if(!sound_is_on)return;
	
	FSOUND_StopSound(sound_data[num].voice_num);
	sound_data[num].used=0;
}


void stop_sound_name(char *name)
{
	int found_sound=0;
	int num,i;
	
	if(!sound_is_on)return;
	
	for(i=0;i<num_of_sounds;i++)
		if(strcmp(sound_info[i].name,name)==0)
		{
			found_sound=1;
			num=i;
			break;
		}
	if(!found_sound)return;
	
	for(i=0;i<MAX_SOUNDS_PLAYING;i++)
		if(sound_data[i].used)
		{
			if(sound_data[i].sound_num==num)
				stop_sound_num(i);
		}

}




void stop_all_sounds(void)
{
	int i;

	if(!sound_is_on)return;
	
	for(i=0;i<MAX_SOUNDS_PLAYING;i++)
	{
		if(sound_data[i].used)
		{
			FSOUND_StopSound(sound_data[i].voice_num);
			sound_data[i].used=0;
		}
	}

}


void pause_all_sounds(void)
{
	int i;

	if(!sound_is_on)return;
	
	for(i=0;i<MAX_SOUNDS_PLAYING;i++)
	{
		if(sound_data[i].used)
		{
			FSOUND_SetPaused(sound_data[i].voice_num,TRUE);
			sound_data[i].used=0;
		}
	}

}

void resume_all_sounds(void)
{
	int i;

	if(!sound_is_on)return;
	
	for(i=0;i<MAX_SOUNDS_PLAYING;i++)
	{
		if(sound_data[i].used)
		{
			FSOUND_SetPaused(sound_data[i].voice_num,FALSE);
			sound_data[i].used=0;
		}
	}

}


/////////////////////////////////////////////////////////////
/////////////////// MUSIC FUNCTIONS ////////////////////////
/////////////////////////////////////////////////////////////


char current_music[50] ="none";
int music_is_looping=0;
static FMUSIC_MODULE *music_data;
static int music_channel;

int play_fiend_music(char* file, int loop)
{
	char path[100];
	char error_string[50];
	
	if(!sound_is_on)return -1;
	

	if(strcmp(file,current_music)==0)return 1;
	
	if(strcmp(current_music,"none")!=0)
	{
		FMUSIC_StopSong(music_data);
		FMUSIC_FreeSong(music_data);
	}
		
	sprintf(path,"music\\%s",file);
	
	if(loop)
	{
		music_data = FMUSIC_LoadSong(path);
		music_is_looping = 1;
	}
	else
	{
		music_data = FMUSIC_LoadSong(path);
		music_is_looping=0;
	}
	
	if(music_data==NULL)
	{
		sprintf(error_string,"Error loading stream \"%s\" ",path);
		make_engine_error(error_string);
		return 0;
	}
	
	strcpy(current_music,file);
		
	speed_counter=0;
	FMUSIC_PlaySong(music_data);
	
	set_fiend_music_volume(fiend_music_volume);
	
	return 1;
}


void set_fiend_music_volume(int vol)
{
	//if(music_channel <0 || sound_is_on==0)return;

	FMUSIC_SetMasterVolume(music_data, vol);

}


void stop_fiend_music(void)
{
	if(!sound_is_on)return;
	if(strcmp(current_music,"none")==0)return;
	
	if(strcmp(current_music,"none")!=0)
	{
		FMUSIC_StopSong(music_data);
		FMUSIC_FreeSong(music_data);
		music_channel = -1;

		strcpy(current_music,"none");
	}
}

void pause_fiend_music(void)
{
	if(!sound_is_on)return;
	if(strcmp(current_music,"none")==0)return;
	
	
	if(FMUSIC_GetPaused(music_data)==FALSE)
	{
		FMUSIC_SetPaused(music_data, TRUE);
	}
}

void resume_fiend_music(void)
{
	if(!sound_is_on)return;
	if(strcmp(current_music,"none")==0)return;
	
	
	if(FMUSIC_GetPaused(music_data)!=FALSE)
	{
		FMUSIC_SetPaused(music_data, FALSE);
	}

}


////////////////////////////////////////////////////////////
////////// UPDATE SOUND EMITORS ///////////////////////////
///////////////////////////////////////////////////////////

void update_soundemitors(void)
{
	int i;

	for(i=0;i<map->num_of_soundemitors;i++)
	{
		if(map->soundemitor[i].active && map->soundemitor[i].voice_num<0)
		{
			map->soundemitor[i].voice_num = play_fiend_sound(map->soundemitor[i].sound_name, map->soundemitor[i].x, map->soundemitor[i].y, map->soundemitor[i].emitor_type, map->soundemitor[i].loop, 200);

		}

	}

}