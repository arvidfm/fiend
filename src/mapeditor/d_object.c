#include <allegro.h>
#include <stdio.h>
#include <string.h>



#include "../fiend.h"
#include "mapedit.h"

char temp_string[20];

int object_chosen=0;
extern int current_object;


int *sorted_object_list;


void sort_objects(void)
{
	int i,temp;
	int sorted=0;
	char string1[40], string2[40];
	
	sorted_object_list = calloc(sizeof(int),num_of_objects);
	for(i=0;i<num_of_objects;i++)
		sorted_object_list[i]=i;


	while(!sorted)
	{
		sorted=1;

		for(i=0;i<num_of_objects-1;i++)
		{
			sprintf(string1,"%s",object_info[sorted_object_list[i]].name);
			sprintf(string2,"%s",object_info[sorted_object_list[i+1]].name);
			
			if(strcasecmp(string1,string2)>0)
			{
				temp = sorted_object_list[i];
				sorted_object_list[i] = sorted_object_list[i+1];
				sorted_object_list[i+1] = temp;

				sorted=0;
			}
		}
	}

}


int bring_object_to_front(void)
{
	int i;
	int num = current_object;
	OBJECT_DATA temp_object;

	memcpy(&temp_object, &map->object[num],sizeof(OBJECT_DATA));

	for(i=num+1;i<map->num_of_objects;i++)
	{
		memcpy(&map->object[i-1],&map->object[i],sizeof(OBJECT_DATA));
	}

	memcpy(&map->object[map->num_of_objects-1],&temp_object,sizeof(OBJECT_DATA));

	//selected=map->num_of_objects-1;

	return D_O_K;
}

int send_object_to_back(void)
{
	int i;
	int num = current_object;
	OBJECT_DATA temp_object;

	if(num==0)return D_O_K;

	memcpy(&temp_object, &map->object[num],sizeof(OBJECT_DATA));

	for(i=num-1;i>-1;i--)
	{
		memcpy(&map->object[i+1],&map->object[i],sizeof(OBJECT_DATA));
	}

	memcpy(&map->object[0],&temp_object,sizeof(OBJECT_DATA));

	//selected=0;

	
	
	return D_O_K;
}



//make a new object
void make_new_object(int x, int y)
{
	if(map->num_of_objects >= MAX_OBJECT_NUM - 1 ) //Check so we have space for the object
	{
		alert("No more objects can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
	}
	
	map->object[map->num_of_objects].x = x;
	map->object[map->num_of_objects].y = y;

	map->object[map->num_of_objects].type = object_chosen;
	map->object[map->num_of_objects].angle = 0;
	map->object[map->num_of_objects].action = 0;
	map->object[map->num_of_objects].save_object = 0;
	map->object[map->num_of_objects].frame = 0;
	map->object[map->num_of_objects].nextframe = 0;
	map->object[map->num_of_objects].pushable = 0;
	map->object[map->num_of_objects].active = 1;
	map->object[map->num_of_objects].voice_num = -1;
	map->object[map->num_of_objects].energy = object_info[map->object[map->num_of_objects].type].energy;

	sprintf(map->object[map->num_of_objects].name,"noname");
	

	map->num_of_objects++;

}






int edit_object(void)
{
	char temp_ang[10]="";
	char temp_energy[10]="";
	char temp_action[10]="";
	char temp_active[10]="";
	char temp_name[10]="";
	char temp_save[10]="";
	char temp_push[10]="";
		

	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 140,  200,  120,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 235,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 235,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_text_proc,     255, 150,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Number:"},
		{  d_text_proc,     315, 150,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   temp_num},
		
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Name:"},
		{  d_edit_proc,     315, 160,  73,   20,   FG_COLOR,   BG_COLOR,     0,  0,   18,    0,   temp_name},
				
		{  d_text_proc,     255, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "angle:"},
		{  d_edit_proc,     315, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_ang},
		
		{  d_text_proc,     255, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "active:"},
		{  d_edit_proc,     315, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_active},
		
		{  d_text_proc,     255, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "energy:"},
		{  d_edit_proc,     315, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_energy},
		
		{  d_text_proc,     255, 200,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "action:"},
		{  d_edit_proc,     315, 200,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_action},
		
		{  d_text_proc,     255, 210,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "save:"},
		{  d_edit_proc,     315, 210,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_save},
		
		{  d_text_proc,     255, 220,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "pushable:"},
		{  d_edit_proc,     328, 220,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_push},
		

		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",current_object);
	sprintf(temp_active,"%d",map->object[current_object].active);
	sprintf(temp_energy,"%d",map->object[current_object].energy);
	sprintf(temp_action,"%d",map->object[current_object].action);
	sprintf(temp_save,"%d",map->object[current_object].save_object);
	sprintf(temp_push,"%d",map->object[current_object].pushable);
	sprintf(temp_ang,"%d",(int)map->object[current_object].angle);
	sprintf(temp_name,"%s",map->object[current_object].name);
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		map->object[current_object].angle = atoi(temp_ang);
		map->object[current_object].active = atoi(temp_active);
		map->object[current_object].energy = atoi(temp_energy);
		map->object[current_object].action = atoi(temp_action);
		map->object[current_object].pushable = atoi(temp_push);
		map->object[current_object].save_object = atoi(temp_save);
		sprintf(map->object[current_object].name,"%s",temp_name);
	}
	

	return D_O_K;
}





int delete_object(void)
{
	int i,ans;
	ans = alert("Sure you want to delete the object?",NULL,NULL,"Yes","Cancel",'\r',0);

	if(ans==2)return D_O_K;

	for(i=current_object;i<map->num_of_objects-1;i++)
	 {
		 map->object[i].x = map->object[i+1].x;
		 map->object[i].y = map->object[i+1].y;
		 
		 map->object[i].type = map->object[i+1].type;
		 map->object[i].angle = map->object[i+1].angle;
		 map->object[i].action = map->object[i+1].action;
		 map->object[i].frame = map->object[i+1].frame;
		 map->object[i].nextframe = map->object[i+1].nextframe;
		 map->object[i].active = map->object[i+1].active;
		 map->object[i].energy = map->object[i+1].energy;
		 map->object[i].pushable = map->object[i+1].pushable;
		 map->object[i].save_object = map->object[i+1].save_object;

		 strcpy(map->object[i].name, map->object[i+1].name);
		 		 		 	 
		 
 	 }
	 map->num_of_objects--;

	return D_O_K;
}



int object_menu(int num)
{
 int ans;
 char temp_text[40];
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "",  NULL,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Edit",    edit_object,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Bring to front",  bring_object_to_front,      NULL,          0,         NULL},
	{ "Send to back",  send_object_to_back,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Delete",  delete_object,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_object=num;	

	sprintf(temp_text,"%s : %d", object_info[map->object[num].type].name, num);
	
	temp_menu[0].text = temp_text;

    ans=do_menu(temp_menu,mouse_x,mouse_y);

	return D_REDRAW;
}




//The list-function for the objects
char *object_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = num_of_objects;
  return NULL;
 }
 
 strcpy(temp_string, object_info[sorted_object_list[index]].name);
 
 return temp_string;
}




//this makes the list
int d_object_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 object_chosen=sorted_object_list[d->d1];
  
 return ans;
}







