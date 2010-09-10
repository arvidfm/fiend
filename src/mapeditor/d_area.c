#include <allegro.h>
#include <stdio.h>

#include "../fiend.h"
#include "mapedit.h"



static int current_area=0;


//make a new object
void make_new_area(int x, int y)
{
	if(map->num_of_areas >= MAX_AREA_NUM - 1 ) //Check so we have space for the object
	{
		alert("No more areas can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
			
	}
	
	
	strcpy(map->area[map->num_of_areas].name, "noname");
	map->area[map->num_of_areas].active=1;
	map->area[map->num_of_areas].x=x;
	map->area[map->num_of_areas].y=y;
	map->area[map->num_of_areas].w=32;
	map->area[map->num_of_areas].h=32;

	

	map->num_of_areas++;

}


int delete_area(void)
{
	int i,ans;
	ans = alert("Sure you want to delete the area?",NULL,NULL,"Yes","Cancel",'\r',0);

	if(ans==2)return D_O_K;

	for(i=current_area;i<map->num_of_areas-1;i++)
	 {
		strcpy(map->area[i].name, map->area[i+1].name);
		map->area[i].active=map->area[i+1].active;
		map->area[i].x=map->area[i+1].x;
		map->area[i].y=map->area[i+1].y;
		map->area[i].w=map->area[i+1].w;
		map->area[i].h=map->area[i+1].h;
	 		 	 
		 
 	 }
	 map->num_of_areas--;

	return D_O_K;
}


int edit_area(void)
{
	char temp_w[10]="";
	char temp_h[10]="";
	char temp_name[40]="";
	char temp_active[10]="";
	
	

	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  200,  100,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
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
		


		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",current_area);
	sprintf(temp_active,"%d",map->area[current_area].active);
	sprintf(temp_w,"%d",map->area[current_area].w);
	sprintf(temp_h,"%d",map->area[current_area].h);
	sprintf(temp_name,"%s",map->area[current_area].name);
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		map->area[current_area].w = atoi(temp_w);
		map->area[current_area].active = atoi(temp_active);
		map->area[current_area].h = atoi(temp_h);
		strcpy(map->area[current_area].name,temp_name);
	}
	

	return D_O_K;
}





int area_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Edit",    edit_area,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Delete",  delete_area,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_area=num;	

    ans=do_menu(temp_menu,mouse_x,mouse_y);

	return D_REDRAW;
}

