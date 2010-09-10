////////////////////////////////////////////////////
// This file contains npoc data loading and saving
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <allegro.h>
#include <stdio.h>
#include "fiend.h"
#include "NPC.h"




char npc_filename[80] = "maps\\npc.inf";


NPC_DATA *npc_data;

//variables
int num_of_npc=0;



//initaialze det npc stuff and clear the npc struct
int init_npc_data(void)
{
	int i;

	npc_data = calloc(sizeof(NPC_DATA),MAX_NPC_NUM);

	for(i=0;i<MAX_NPC_NUM;i++)
	{
		npc_data[i].used=0;
		npc_data[i].active=0;
		
		strcpy(npc_data[i].name,"nomame");
		strcpy(npc_data[i].map_name,"");
	}
	
	return 0;
}



//load the npc struct 
int load_npc(void)
{
	FILE *f;
	int i;

	f = fopen(npc_filename,"rb");
	if(f==NULL)
	{
		for(i=0;i<MAX_NPC_NUM;i++)
		{
				npc_data[i].used=0;
				npc_data[i].active=0;
		
				strcpy(npc_data[i].name,"nomame");
				strcpy(npc_data[i].map_name,"");
		}
		
		return 1; //when you start fresh you have no npc:s so you can't give an error code here
			
	}
	fread(npc_data, sizeof(NPC_DATA), MAX_NPC_NUM, f);

	fclose(f);

	return 1;
}



//save the npc struct
int save_npc(void)
{
	FILE *f;

	f = fopen(npc_filename,"wb");
	if(f==NULL)return 0;

	fwrite(npc_data, sizeof(NPC_DATA), MAX_NPC_NUM, f);

	fclose(f);

	return 1;
}


void release_npc(void)
{
	free(npc_data);
}









