////////////////////////////////////////////////////
// This file contains trigger loading/saving and
// other smalleer trigger used funcs.
// Also has variable and object loading /saving
// and loading message faces.
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <stdio.h>
#include <string.h>

#include "fiend.h"

int saved_var_num=0;

SAVED_LOCAL_VAR saved_local_var[MAX_ROOM_NUM];

int saved_object_num=0;

SAVED_ROOM_OBJECT saved_object[MAX_ROOM_NUM];



char global_var_filename[80] = "maps/vars.inf";


VARIABLE_DATA *global_var;


char global_trigger_filename[80] = "maps/trigger.inf";

TRIGGER_DATA *global_trigger;




//////////////////////////////////////////
//////////// MESSAGE FACE LOADING ////////
/////////////////////////////////////////

FACE_DATA message_face[FACE_NUM];

int num_of_message_faces=0;

static int face_load_error =0;

static void get_one_face(char *file,int attr,int param)
{
	char *name;
	char name2[40];
	int i,k;
		
	message_face[num_of_message_faces].pic = load_bmp(file,NULL);
	
	if(message_face[num_of_message_faces].pic==NULL)face_load_error=1;
	
	
	name = get_filename(file);
	strcpy(name2,name);
	k = strlen(name2);
	for(i=0;i<k;i++)
	{
			if(name2[i]=='.')
			{
				name2[i]=0;
				break;
			}
	}
	strcpy(message_face[num_of_message_faces].name ,name2);
	//allegro_message(name2);
		
	num_of_message_faces++;
}


int load_message_faces(void)
{
	int i;

	i= for_each_file("graphic/faces/*.bmp",FA_ARCH,get_one_face,0);

	if(face_load_error || i<1)
	{
		strcpy(fiend_errorcode,"couldn't load graphic/faces/*.bmp");
		return 1;
	}

	return 0;
}


void release_message_faces(void)
{
	int i;

	for(i=0;i<FACE_NUM;i++)
	{
		destroy_bitmap(message_face[i].pic);	
	}


}


/////////////////////////////////////////////////
///////LOCAL VARIABLES STUFF /////////////////////////
/////////////////////////////////////////////////


void reset_saved_vars(void)
{
	int i;
	
	saved_var_num=0;

	for(i=0;i<MAX_ROOM_NUM;i++)
	{
		sprintf(saved_local_var[i].name,"null");
	}

	saved_object_num=0;

	for(i=0;i<MAX_ROOM_NUM;i++)
	{
		sprintf(saved_object[i].name,"null");
	}
}



void save_local_vars(void)
{
	int i;
	int num= -1;

	
	//////// Save the local variables ///////////////
	
	for(i=0;i<saved_var_num;i++)
		if(strcmp(saved_local_var[i].name,map->name)==0)
		{
			num = i;
			break;
		}
	
	if(num<0)
	{
		sprintf(saved_local_var[saved_var_num].name,"%s",map->name);
	
		for(i=0;i<LOCAL_VAR_NUM;i++)
		{
			if(strcmp(map->var[i].name,"null")!=0)
			{
				saved_local_var[saved_var_num].var[i].value = map->var[i].value;
				saved_local_var[saved_var_num].var[i].used=1;	
			}
			else
			{
				saved_local_var[saved_var_num].var[i].used=0;	
			}

		}
		
		saved_var_num++;
	}
	else
	{
		for(i=0;i<LOCAL_VAR_NUM;i++)
		{
			if(strcmp(map->var[i].name,"null")!=0)
			{
				saved_local_var[num].var[i].value = map->var[i].value;
				saved_local_var[num].var[i].used=1;	
			}
			else
			{
				saved_local_var[num].var[i].used=0;	
			}
		}
	}

	//////// Save the objects ///////////////
	
	num =-1;

	for(i=0;i<saved_object_num;i++)
		if(strcmp(saved_object[i].name, map->name)==0)
		{
			num = i;
			break;
		}

	if(num<0)
	{
		sprintf(saved_object[saved_object_num].name,"%s",map->name);
	
		for(i=0;i<MAX_OBJECT_NUM;i++)
		{
			saved_object[saved_object_num].object[i].x = map->object[i].x;
			saved_object[saved_object_num].object[i].y = map->object[i].y;
			saved_object[saved_object_num].object[i].angle = map->object[i].angle;
			saved_object[saved_object_num].object[i].action = map->object[i].action;
			saved_object[saved_object_num].object[i].frame = map->object[i].frame;
			saved_object[saved_object_num].object[i].nextframe = map->object[i].nextframe;
			saved_object[saved_object_num].object[i].active = map->object[i].active;
			saved_object[saved_object_num].object[i].energy = map->object[i].energy;
			saved_object[saved_object_num].object[i].type = map->object[i].type;
			strcpy(saved_object[saved_object_num].object[i].name, map->object[i].name);

		}
		
		saved_object_num++;

		//allegro_message("%s",saved_object[0].name);
	}
	else
	{
	
		for(i=0;i<MAX_OBJECT_NUM;i++)
		{
			if(map->object[i].save_object)
			{
				saved_object[num].object[i].x = map->object[i].x;
				saved_object[num].object[i].y = map->object[i].y;
				saved_object[num].object[i].angle = map->object[i].angle;
				saved_object[num].object[i].action = map->object[i].action;
				saved_object[num].object[i].frame = map->object[i].frame;
				saved_object[num].object[i].nextframe = map->object[i].nextframe;
				saved_object[num].object[i].active = map->object[i].active;
				saved_object[num].object[i].energy = map->object[i].energy;
				saved_object[num].object[i].type = map->object[i].type;
				strcpy(saved_object[num].object[i].name, map->object[i].name);
			}
		}
	}

	

}



int load_local_vars(void)
{
	int i;
	int num= -1;

	
	//////// Load the vars ///////////
	for(i=0;i<saved_var_num;i++)
		if(strcmp(saved_local_var[i].name,map->name)==0)
		{
			num = i;
			break;
		}

	if(num<0)
	{
		return 0;
	}
	else
	{
		for(i=0;i<LOCAL_VAR_NUM;i++)
			if(saved_local_var[num].var[i].used)
				map->var[i].value = saved_local_var[num].var[i].value;
		
	}

	//////// Load the objects ///////////
	num = -1;
	
	for(i=0;i<saved_object_num;i++)
		if(strcmp(saved_object[i].name,map->name)==0)
		{
			num = i;
			break;
		}
	//allegro_message("%d",num);

	if(num<0)
	{
		return 0;
	}
	else
	{
		for(i=0;i<MAX_OBJECT_NUM;i++)
		{
			if(map->object[i].save_object && map->object[i].type == saved_object[num].object[i].type && strcmp(map->object[i].name, saved_object[num].object[i].name)==0)
			{
				map->object[i].x = saved_object[num].object[i].x;
				map->object[i].y= saved_object[num].object[i].y;
				map->object[i].angle= saved_object[num].object[i].angle;
				map->object[i].action= saved_object[num].object[i].action;
				map->object[i].frame= saved_object[num].object[i].frame;
				map->object[i].nextframe= saved_object[num].object[i].nextframe;
				map->object[i].active= saved_object[num].object[i].active;
				map->object[i].energy= saved_object[num].object[i].energy;
			}
		}
		
	}
	return 1;
}






/////////////////////////////////////////////////
///////GLOABAL VARIABLES STUFF /////////////////////////
/////////////////////////////////////////////////


//initaialze det var stuff and clear the var struct
int init_global_vars(void)
{
	int i;

	global_var = calloc(sizeof(VARIABLE_DATA),GLOBAL_VAR_NUM);

	for(i=0;i<GLOBAL_VAR_NUM;i++)
	{
		global_var[i].value=0;
		
		sprintf(global_var[i].name,"null",i);
	}
	
	return 0;
}



//load the var struct 
int load_global_vars(void)
{
	FILE *f;
	int i;

	f = fopen(global_var_filename,"rb");
	if(f==NULL)
	{
		for(i=0;i<GLOBAL_VAR_NUM;i++)
		{
			global_var[i].value=0;
			
			sprintf(global_var[i].name,"null",i);
		}
	
		return 1; //when you start fresh you have no vars so you can't give an error code here
			
	}
	fread(global_var, sizeof(VARIABLE_DATA), GLOBAL_VAR_NUM, f);

	fclose(f);

	return 1;
}



//save the var struct
int save_global_vars(void)
{
	FILE *f;

	f = fopen(global_var_filename,"wb");
	if(f==NULL)return 0;

	fwrite(global_var, sizeof(VARIABLE_DATA), GLOBAL_VAR_NUM, f);

	fclose(f);

	return 1;
}


void release_global_vars(void)
{
	free(global_var);
}


///////////////////////////////////////////////
////////// GLOBAL TRIGGERS /////////////////////
///////////////////////////////////////////////


//initaialze det var stuff and clear the var struct
int init_global_triggers(void)
{
	int i,j;

	global_trigger = calloc(sizeof(TRIGGER_DATA),GLOBAL_TRIGGER_NUM);

	for(i=0;i<GLOBAL_TRIGGER_NUM;i++)
	{
		global_trigger[i].type=1;
		global_trigger[i].active=0;
		sprintf(global_trigger[i].name,"Nothing %d",i);
			
			
		for(j=0;j<MAX_CONDITION_NUM;j++)
		{
			global_trigger[i].condition[j].used=0;
		}
		
		for(j=0;j<MAX_EVENT_NUM;j++)
		{
			global_trigger[i].event[j].used=0;
		}

	}
	
	return 0;
}


//load the var struct 
int load_global_triggers(void)
{
	FILE *f;
	int i,j;

	f = fopen(global_trigger_filename,"rb");
	if(f==NULL)
	{
		for(i=0;i<GLOBAL_TRIGGER_NUM;i++)
		{
			global_trigger[i].type=1;
			global_trigger[i].active=0;
			sprintf(global_trigger[i].name,"Nothing %d",i);
			
			for(j=0;j<MAX_CONDITION_NUM;j++)
			{
				global_trigger[i].condition[j].used=0;
			}
		
			for(j=0;j<MAX_EVENT_NUM;j++)
			{
				global_trigger[i].event[j].used=0;
			}

		}
	
		return 1; //when you start fresh you have no vars so you can't give an error code here
			
	}
	fread(global_trigger, sizeof(TRIGGER_DATA), GLOBAL_TRIGGER_NUM, f);

	fclose(f);

	return 1;
}



//save the var struct
int save_global_triggers(void)
{
	FILE *f;

	f = fopen(global_trigger_filename,"wb");
	if(f==NULL)return 0;

	fwrite(global_trigger, sizeof(TRIGGER_DATA), GLOBAL_TRIGGER_NUM, f);

	fclose(f);

	return 1;
}


void release_global_triggers(void)
{
	free(global_trigger);
}





