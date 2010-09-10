#include <allegro.h>


#ifndef SOUND_H
#define SOUND_H 

#define MAX_SOUNDS_PLAYING 30

#define HEAR_RANGE 500

// the structs
typedef struct
{
	char name[50];
	int volume;
	int num;
	FSOUND_SAMPLE *sound; 
}SOUND_INFO;



typedef struct 
{
	int used;
	int voice_num;
	int sound_num;
	int x;
	int y;
	int vol;
	int loop;
	int lower_at_dist;
	int playing;
	int priority;
}SOUND_DATA;


extern int num_of_sounds;

extern SOUND_INFO *sound_info;
extern SOUND_DATA *sound_data;
// the functions


int play_fiend_sound(char *name, int x, int y,int lower_at_dist,int loop,int priority);
void stop_all_sounds(void);
void pause_all_sounds(void);
void resume_all_sounds(void);
void stop_sound_num(int num);
void stop_sound_name(char *name);

int play_fiend_music(char* file, int loop);
void stop_fiend_music(void);
void pause_fiend_music(void);
void resume_fiend_music(void);
void set_fiend_music_volume(int vol);

void update_sound(void);
void update_soundemitors(void);

int load_sounds(void);
void free_sounds(void);

#endif

