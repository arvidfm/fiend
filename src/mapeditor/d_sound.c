#include <allegro.h>
#include <stdio.h>
#include <string.h>

#include "../fiend.h"
#include "mapedit.h"

static char temp_string[50];
static int emitsound_chosen=0;
int current_soundemitor;


//make a new soundemitor
void make_new_soundemitor(int x, int y)
{
	if(map->num_of_soundemitors>= MAX_SOUNDEMITOR_NUM - 1 ) //Check so we have space for the object
	{
		alert("No more soundemitors can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
			
	}
	
	map->soundemitor[map->num_of_soundemitors].x = x;
	map->soundemitor[map->num_of_soundemitors].y = y;

	strcpy( map->soundemitor[map->num_of_soundemitors].sound_name, "none");
	map->soundemitor[map->num_of_soundemitors].sound_type = 0;
	map->soundemitor[map->num_of_soundemitors].emitor_type =0;
	map->soundemitor[map->num_of_soundemitors].loop = 0;
	map->soundemitor[map->num_of_soundemitors].active = 1;
	map->soundemitor[map->num_of_soundemitors].voice_num = -1;
	

	map->num_of_soundemitors++;

}






char *emitsound_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = num_of_sounds;
  return NULL;
 }
 
 strcpy(temp_string, sound_info[index].name);
 
 return temp_string;
}




int d_emitsound_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 emitsound_chosen=d->d1;
  
 return ans;
}


int d_soundbutton_proc(int msg,DIALOG *d,int c)
{
	int ans;
	ans = d_button_proc(msg,d,c);
	
	if(ans==D_EXIT)
	{
		FSOUND_StopSound(FSOUND_ALL);
		FSOUND_PlaySound(FSOUND_FREE, sound_info[emitsound_chosen].sound);
		return D_REDRAW;
		
	}
	

	return ans;
}



int edit_soundemitor(void)
{
	int i;
	char temp_loop[10]="";
	char temp_e_type[10]="";
	char temp_s_type[10]="";
	char temp_active[10]="";
	
	

	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  300,  100,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 225,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 225,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		{  d_soundbutton_proc,430, 225,  53,  20,   FG_COLOR,   BG_COLOR,     't',  D_EXIT,   0,    0,        "Test" },
		
		{  d_emitsound_list_proc,   370,   160,     160,     50,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    emitsound_list},
     	
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Number:"},
		{  d_text_proc,     315, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   temp_num},
		
		{  d_text_proc,     255, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "active:"},
		{  d_edit_proc,     315, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_active},
		
		{  d_text_proc,     255, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "e_type:"},
		{  d_edit_proc,     315, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_e_type},
		
		{  d_text_proc,     255, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "s_type:"},
		{  d_edit_proc,     315, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_s_type},
		
		{  d_text_proc,     255, 200,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "loop:"},
		{  d_edit_proc,     315, 200,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_loop},
		
		

		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",current_soundemitor);
	
	for(i=0;i<num_of_sounds;i++)
	{
		if(strcmp(sound_info[i].name, map->soundemitor[current_soundemitor].sound_name)==0){
			mission_dialog[4].d1=i;
			break;}
		else
			mission_dialog[4].d1=0;
	}
	
	sprintf(temp_active,"%d",map->soundemitor[current_soundemitor].active);
	sprintf(temp_e_type,"%d",map->soundemitor[current_soundemitor].emitor_type);
	sprintf(temp_s_type,"%d",map->soundemitor[current_soundemitor].sound_type);
	sprintf(temp_loop,"%d",map->soundemitor[current_soundemitor].loop);
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		strcpy(map->soundemitor[current_soundemitor].sound_name, sound_info[emitsound_chosen].name);		
		map->soundemitor[current_soundemitor].emitor_type = atoi(temp_e_type);
		map->soundemitor[current_soundemitor].active = atoi(temp_active);
		map->soundemitor[current_soundemitor].sound_type = atoi(temp_s_type);
		map->soundemitor[current_soundemitor].loop = atoi(temp_loop);
	}
	

	return D_O_K;
}



int delete_soundemitor(void)
{
	int i,ans;
	ans = alert("Sure you want to delete the soundemitor?",NULL,NULL,"Yes","Cancel",'\r',0);

	if(ans==2)return D_O_K;

	for(i=current_soundemitor;i<map->num_of_soundemitors-1;i++)
	 {
		 map->soundemitor[i].x = map->soundemitor[i+1].x;
		 map->soundemitor[i].y = map->soundemitor[i+1].y;
		 
		 map->soundemitor[i].sound_type = map->soundemitor[i+1].sound_type;
		 map->soundemitor[i].emitor_type = map->soundemitor[i+1].emitor_type;
		 map->soundemitor[i].loop = map->soundemitor[i+1].loop;
		 strcpy(map->soundemitor[i].sound_name, map->soundemitor[i+1].sound_name);
		 		 	 
		 
 	 }
	 map->num_of_soundemitors--;

	return D_O_K;
}



int soundemitor_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Edit",    edit_soundemitor,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Delete",  delete_soundemitor,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_soundemitor=num;	

    ans=do_menu(temp_menu,mouse_x,mouse_y);

	return D_REDRAW;
}



