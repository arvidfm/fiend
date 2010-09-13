#include <allegro.h>
#include <stdio.h>
#include <string.h>

#include "../fiend.h"
#include "mapedit.h"


int current_link=0;
int current_link_event=0;

char temp_map_file[80]="";


void make_new_link(int x, int y)
{
	int i;

	if(map->num_of_links >= MAX_LINK_NUM - 1 ) //Check so we have space for the link
	{
		alert("No more links can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
			
	}

	map->link[map->num_of_links].x = x;
	map->link[map->num_of_links].y = y;

	map->link[map->num_of_links].w = 40;
	map->link[map->num_of_links].h = 40;

	map->link[map->num_of_links].event_chosen = 0;

	map->link[map->num_of_links].type = 0;

	strcpy(map->link[map->num_of_links].name,"Noname");

	for(i=0;i<5;i++)
	{
		map->link[map->num_of_links].event[i].type = 0;
		strcpy(map->link[map->num_of_links].event[i].message,"Nothing");
		strcpy(map->link[map->num_of_links].event[i].map_file,"");
		strcpy(map->link[map->num_of_links].event[i].link_name,"Nothing");


	}
	map->num_of_links++;
	
}

int d_browse_map_file(int msg, DIALOG *d, int c)
{
	int ans;
	int ans2;
	char temp_path[80];

	ans = d_button_proc(msg,d,c);

	if(ans==D_EXIT)
	{
		strcpy(temp_path, temp_map_file);
		ans2 = file_select_ex("select a map file", temp_path, "map", 80, OLD_FILESEL_WIDTH, OLD_FILESEL_HEIGHT);
		if(ans2!=0)
			strcpy(temp_map_file, temp_path);
		
		return D_REDRAW;
	}

	return ans;
}


void edit_link_event(void)
{
	int ans;
	char temp_type[5]="";
	char temp_message[LIMK_MESSAGE_LENGTH]="";
	char temp_link_name[20]="";

	
	DIALOG temp_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{ d_clear_proc,       0,    0,    0,    0,    FG_COLOR,  BG_COLOR,    0,    0,       0,    0,    NULL },
		{  d_shadow_box_proc, 250, 150,  300,  100,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		{  d_button_proc,     310, 225,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 225,   53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_text_proc,     255, 160,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "type (0=normal 1=stop):"},
		{  d_edit_proc,     450, 160,  33,   15,   FG_COLOR,   BG_COLOR,     0,  0,   5,  0,   temp_type},
		
		{  d_text_proc,     255, 175,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "map_file:"},
		{  d_edit_proc,     330, 175,  123,   15,   FG_COLOR,   BG_COLOR,     0,  0,   79,  0,   temp_map_file},
		
		{  d_browse_map_file, 463, 172,  80,  12,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,  0,   "Browse..."},
		
		{  d_text_proc,     255, 190,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "linkname:"},
		{  d_edit_proc,     330, 190,  173,   15,   FG_COLOR,   BG_COLOR,     0,  0,   19,  0,   temp_link_name},
		
		{  d_text_proc,     255, 205,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "message:"},
		{  d_edit_proc,     330, 205,  183,   15,   FG_COLOR,   BG_COLOR,     0,  0,   LIMK_MESSAGE_LENGTH-1,  0,   temp_message},
		

		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_type,"%d",map->link[current_link].event[current_link_event].type);
	strcpy(temp_link_name, map->link[current_link].event[current_link_event].link_name);
	strcpy(temp_message, map->link[current_link].event[current_link_event].message);
	strcpy(temp_map_file, map->link[current_link].event[current_link_event].map_file);

	ans = popup_dialog(temp_dialog,-1);

	if(ans==2)
	{
		strcpy(map->link[current_link].event[current_link_event].link_name, temp_link_name);
		strcpy(map->link[current_link].event[current_link_event].message, temp_message);
		strcpy(map->link[current_link].event[current_link_event].map_file, temp_map_file);
		map->link[current_link].event[current_link_event].type = atoi(temp_type);

	}

	
}




int d_event_list_proc(int msg, DIALOG *d, int c)
{
	int ans;
	ans = d_list_proc(msg,d,c);

	current_link_event = d->d1;

	if(ans==D_EXIT)
	{
		edit_link_event();
		return D_REDRAW;
	}


	return ans;
}



char *link_list_proc(int index, int *list_size)
{
	if(index<0)
	{
		*list_size=5;
		return NULL;
	}
	else
	{
		return map->link[current_link].event[index].link_name;
	}



}





int edit_link(void)
{
	int ans;
	char the_num[4] =""; 
	char temp_w[5]="";
    char temp_h[5]="";
	char temp_ec[5]="";
    char temp_t[5]="";
	char temp_name[25]="";

	DIALOG temp_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{ d_clear_proc,       0,    0,    0,    0,    FG_COLOR,  BG_COLOR,    0,    0,       0,    0,    NULL },
		{  d_shadow_box_proc, 250, 150,  300,  200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 325,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 325,   53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Num:"},
		{  d_text_proc,     295, 160,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   the_num},
		
		{  d_text_proc,     350, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "name:"},
		{  d_edit_proc,     390, 160,  80,   20,   FG_COLOR,   BG_COLOR,     0,  0,   17,  0,   temp_name},
		
		{  d_text_proc,     255, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "width:"},
		{  d_edit_proc,     320, 180,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_w},
		
		{  d_text_proc,     255, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "width:"},
		{  d_edit_proc,     320, 180,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_w},
		
		{  d_text_proc,     375, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "height:"},
		{  d_edit_proc,     435, 180,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_h},
		
		{  d_text_proc,     255, 210,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "current:"},
		{  d_edit_proc,     320, 210,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_ec},
		
		{  d_text_proc,     375, 210,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "type (0=v 1=h):"},
		{  d_edit_proc,     495, 210,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_t},
		
		{  d_event_list_proc,335, 230,  130,   80,   FG_COLOR,   BG_COLOR,     0, D_EXIT ,   0,    0,  link_list_proc },
		
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	
	sprintf(the_num,"%d",current_link);

	sprintf(temp_w,"%d",map->link[current_link].w);
	sprintf(temp_h,"%d",map->link[current_link].h);
	sprintf(temp_ec,"%d",map->link[current_link].event_chosen);
	sprintf(temp_t,"%d",map->link[current_link].type);
	strcpy(temp_name, map->link[current_link].name);

	ans = popup_dialog(temp_dialog,-1);
	

	if(ans==2)
	{
		map->link[current_link].w = atoi(temp_w);
		map->link[current_link].h = atoi(temp_h);
		map->link[current_link].event_chosen = atoi(temp_ec);
		map->link[current_link].type = atoi(temp_t);

		strcpy(map->link[current_link].name, temp_name);
		

	}


	return D_O_K;
}



int delete_link(void)
{
	int i,j,ans;
	ans = alert("Sure you want to delete the link?",NULL,NULL,"Yes","Cancel",'\r',0);

	if(ans==2)return D_O_K;

	for(i=current_link;i<map->num_of_links-1;i++)
	 {
		 map->link[i].x = map->link[i+1].x;
		 map->link[i].y = map->link[i+1].y;
		 
		 map->link[i].w = map->link[i+1].w;
		 map->link[i].h = map->link[i+1].h;

		 map->link[i].event_chosen = map->link[i+1].event_chosen;
		 map->link[i].type = map->link[i+1].type;

		 strcpy(map->link[i].name,map->link[i+1].name);

		 for(j=0;j<5;j++)
		 {
			strcpy(map->link[i].event[j].message,map->link[i+1].event[j].message);
			strcpy(map->link[i].event[j].map_file,map->link[i+1].event[j].map_file);
			strcpy(map->link[i].event[j].link_name,map->link[i+1].event[j].link_name);
			map->link[i].event[j].type = map->link[i+1].event[j].type;

			
		 }
		 
		 
 	 }
	 map->num_of_links--;

	return D_O_K;
}



int link_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Edit",    edit_link,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Delete",  delete_link,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_link=num;

	ans=do_menu(temp_menu,mouse_x,mouse_y);

	return D_REDRAW;
}

