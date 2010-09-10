
#ifndef SAVEGAME_H
#define SAVEGAME_H

typedef struct 
{
	char mapfile[80];
	char name[80];
	char date[50];
	int play_time;
}SAVEDATA;

extern SAVEDATA savedata;

int save_game(char *file,char *name);
int load_game(char *file);
void save_menu(void);


#endif 
