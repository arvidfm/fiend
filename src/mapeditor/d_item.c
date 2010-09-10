#include <allegro.h>

#include <stdio.h>

#include "../fiend.h"
#include "mapedit.h"

static char temp_string[180];


int item_chosen=0;
int current_item=0;



//make a new npc
void make_new_item(int x, int y)
{
	int i;

	for(i=0;i<MAX_ITEM_DATA;i++)
		if(item_data[i].used==0)break;

	if(item_data[i].used==1)
	{
		alert("No more items can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
	}

	item_data[i].used=1;
	
	item_data[i].active=1;

	item_data[i].x=x;
	item_data[i].y=y;

	item_data[i].angle=0;

	item_data[i].value=item_info[item_chosen].num;
	item_data[i].sp1=0;

	item_data[i].picked_up=0;
	item_data[i].type=item_chosen;
	
	strcpy(item_data[i].map_name, map->name);
	
	strcpy(item_data[i].name, item_info[item_chosen].name);
	strcpy(item_data[i].desc, item_info[item_chosen].desc);
	strcpy(item_data[i].string, "");

}








int edit_item(void)
{
	char temp_active[10]="";
	char temp_value[10]="";
	char temp_desc[160]="";
	char temp_name[30]="";
	char temp_angle[10]="";
	char temp_string[40]="";
		

	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  300,  110,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 235,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 235,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Number:"},
		{  d_text_proc,     315, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   temp_num},
		
		{  d_text_proc,     255, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Active:"},
		{  d_edit_proc,     315, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_active},
		
		{  d_text_proc,     255, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Value:"},
		{  d_edit_proc,     315, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_value},
		
		{  d_text_proc,     255, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Angle:"},
		{  d_edit_proc,     315, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_angle},
		
		{  d_text_proc,     255, 200,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Name:"},
		{  d_edit_proc,     315, 200,  160,   20,   FG_COLOR,   BG_COLOR,     0,  0,   29,    0,   temp_name},
		
		{  d_text_proc,     255, 210,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Desc:"},
		{  d_edit_proc,     315, 210,  210,   20,   FG_COLOR,   BG_COLOR,     0,  0,   159,    0,   temp_desc},
		
		{  d_text_proc,     255, 220,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "String:"},
		{  d_edit_proc,     315, 220,  160,   20,   FG_COLOR,   BG_COLOR,     0,  0,   39,    0,   temp_string},
				
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",current_item);
	
	
	sprintf(temp_active,"%d",item_data[current_item].active);
	sprintf(temp_value,"%d",item_data[current_item].value);
	sprintf(temp_angle,"%d",(int)item_data[current_item].angle);
	
	sprintf(temp_name,"%s",item_data[current_item].name);
	sprintf(temp_desc,"%s",item_data[current_item].desc);
	sprintf(temp_string,"%s",item_data[current_item].string);
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		item_data[current_item].active = atoi(temp_active);
		item_data[current_item].value = atoi(temp_value);
		item_data[current_item].angle = (float)atoi(temp_angle);
				
		sprintf(item_data[current_item].name,"%s",temp_name);
		sprintf(item_data[current_item].desc,"%s",temp_desc);
		sprintf(item_data[current_item].string,"%s",temp_string);
	
	}
	

	return D_O_K;

}








int delete_item(void)
{
	int ans;

	ans = alert("Sure you want to delete the item?",NULL,NULL,"Yes","Cancel",'\r',0);
	if(ans==2)return D_O_K;
	
	item_data[current_item].used=0;
	
	return D_O_K;
}



int item_menu(int num)
{
 int ans;
 char temp_text[40];
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "",      NULL,      NULL,          0,         NULL},
	{ "",      NULL,      NULL,          0,         NULL},
	{ "Edit",      edit_item,      NULL,          0,         NULL},
	{ "",      NULL,      NULL,          0,         NULL},
	{ "Delete",  delete_item,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_item=num;
	
	sprintf(temp_text,"%s : %d", item_info[item_data[num].type].name, num);
	temp_menu[0].text = temp_text;
	
    ans=do_menu(temp_menu,mouse_x,mouse_y);
	
	
	return D_REDRAW;
}






//The list-function for the NPCs
char *item_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = num_of_items;
  return NULL;
 }
 
 strcpy(temp_string, item_info[index].name);
 
 return temp_string;
}




//this makes the list
int d_item_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 item_chosen=d->d1;
  
 return ans;
}






