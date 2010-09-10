#include <allegro.h>
#include <stdio.h>

#include "../fiend.h"
#include "mapedit.h"


static int current_look_at_area=0;

//make a new object
void make_new_look_at_area(int x, int y)
{
	if(map->num_of_look_at_areas >= MAX_LOOK_AT_AREA_NUM - 1 ) //Check so we have space for the object
	{
		alert("No more look at areas can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
			
	}
	
	
	strcpy(map->look_at_area[map->num_of_look_at_areas].name, "noname");
	strcpy(map->look_at_area[map->num_of_look_at_areas].message, "nothing");
	map->look_at_area[map->num_of_look_at_areas].active=1;
	map->look_at_area[map->num_of_look_at_areas].x=x;
	map->look_at_area[map->num_of_look_at_areas].y=y;
	map->look_at_area[map->num_of_look_at_areas].w=50;
	map->look_at_area[map->num_of_look_at_areas].h=50;

	map->num_of_look_at_areas++;
}


int delete_look_at_area(void)
{
	int i,ans;
	ans = alert("Sure you want to delete the look at area?",NULL,NULL,"Yes","Cancel",'\r',0);

	if(ans==2)return D_O_K;

	for(i=current_look_at_area;i<map->num_of_look_at_areas-1;i++)
	 {
		strcpy(map->look_at_area[i].name, map->look_at_area[i+1].name);
		strcpy(map->look_at_area[i].message, map->look_at_area[i+1].message);
		map->look_at_area[i].active=map->look_at_area[i+1].active;
		map->look_at_area[i].x=map->look_at_area[i+1].x;
		map->look_at_area[i].y=map->look_at_area[i+1].y;
		map->look_at_area[i].w=map->look_at_area[i+1].w;
		map->look_at_area[i].h=map->look_at_area[i+1].h;
	 		 	 
		 
 	 }
	 map->num_of_look_at_areas--;

	return D_O_K;
}



int edit_look_at_area(void)
{
	char temp_w[10]="";
	char temp_h[10]="";
	char temp_name[40]="";
	char temp_active[10]="";
	char temp_message[120]="";
	
	

	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  400,  100,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 225,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 225,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Number:"},
		{  d_text_proc,     315, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   temp_num},
		
		{  d_text_proc,     255, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "active"},
		{  d_edit_proc,     315, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_active},
		
		{  d_text_proc,     255, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "name:"},
		{  d_edit_proc,     315, 180,  100,   20,   FG_COLOR,   BG_COLOR,     0,  0,   39,    0,   temp_name},
		
		{  d_text_proc,     255, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "w:"},
		{  d_edit_proc,     315, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_w},
		
		{  d_text_proc,     255, 200,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "h:"},
		{  d_edit_proc,     315, 200,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_h},
		
		{  d_text_proc,     255, 210,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "message:"},
		{  d_edit_proc,     325, 210,  283,   20,   FG_COLOR,   BG_COLOR,     0,  0,   119,    0,   temp_message},
		

		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",current_look_at_area);
	sprintf(temp_active,"%d",map->look_at_area[current_look_at_area].active);
	sprintf(temp_w,"%d",map->look_at_area[current_look_at_area].w);
	sprintf(temp_h,"%d",map->look_at_area[current_look_at_area].h);
	sprintf(temp_name,"%s",map->look_at_area[current_look_at_area].name);
	sprintf(temp_message,"%s",map->look_at_area[current_look_at_area].message);
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		map->look_at_area[current_look_at_area].w = atoi(temp_w);
		map->look_at_area[current_look_at_area].active = atoi(temp_active);
		map->look_at_area[current_look_at_area].h = atoi(temp_h);
		strcpy(map->look_at_area[current_look_at_area].name,temp_name);
		strcpy(map->look_at_area[current_look_at_area].message,temp_message);
	}
	

	return D_O_K;
}





int look_at_area_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Edit",    edit_look_at_area,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Delete",  delete_look_at_area,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_look_at_area=num;	

    ans=do_menu(temp_menu,mouse_x,mouse_y);

	return D_REDRAW;
}

