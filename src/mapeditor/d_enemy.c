#include <allegro.h>

#include <stdio.h>

#include "../fiend.h"
#include "mapedit.h"

extern MENU editmode_menu[];

static char temp_string[180];


static char mission_text[][40]={"Kill Player","Kill NPC", "Patrol", "Run away"}; 

extern int editmode_enemy_path();


int enemy_chosen=0;
int current_enemy=0;

int enemy_path_chosen=0;



//make a new npc
void make_new_enemy(int x, int y)
{
	int i,j;

	if(num_of_enemys<1)return;
	
	for(i=0;i<MAX_ENEMY_DATA;i++)
		if(enemy_data[i].used==0)break;

	if(enemy_data[i].used==1)
	{
		alert("No more enemies can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
	}

	enemy_data[i].used=1;
	
	enemy_data[i].active=1;

	enemy_data[i].dead=0;
	enemy_data[i].was_dead=0;

	enemy_data[i].disappear=0;

	enemy_data[i].type=enemy_chosen;
	
	strcpy(enemy_data[i].name,"noname");
	strcpy(enemy_data[i].map_name, map->name);

	enemy_data[i].x=x;
	enemy_data[i].y=y;
	enemy_data[i].angle=0;

	enemy_data[i].energy=(float)enemy_info[enemy_chosen].energy;
	
	
	enemy_data[i].action=0;
	
	enemy_data[i].current_mission=0;
		
	enemy_data[i].frame=0;
	enemy_data[i].nextframe;

	enemy_data[i].sp1=0;
	enemy_data[i].sp2=0;
	enemy_data[i].sp3=0;

	enemy_data[i].current_movement_path=0;

		
	for(j=0;j<MAX_ENEMY_PATHS_NUM;j++)
	{
		enemy_data[i].movement_path[j].type=0;
		enemy_data[i].movement_path[j].num_of_points=0;
	}

	

}



char *mission_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MISSION_NUM;
  return NULL;
 }
 
  
 return mission_text[index];
}




int d_mission_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

 
 
  return ans;
}



int edit_enemy(void)
{
	char temp_angle[10]="";
	char temp_energy[10]="";
	char temp_active[10]="";
	char temp_name[40]="";
	char temp_action[10]="";
	char temp_disappear[10]="";
		

	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  300,  110,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 235,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 235,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_mission_list_proc,   420,   160,     125,     20,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    mission_list},
     
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Number:"},
		{  d_text_proc,     315, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   temp_num},
		
		{  d_text_proc,     255, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "active:"},
		{  d_edit_proc,     315, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_active},
		
		{  d_text_proc,     255, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "name:"},
		{  d_edit_proc,     315, 180,  93,   20,   FG_COLOR,   BG_COLOR,     0,  0,   39,    0,   temp_name},
		
		{  d_text_proc,     255, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "action:"},
		{  d_edit_proc,     315, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_action},
		
		{  d_text_proc,     255, 200,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "energy:"},
		{  d_edit_proc,     315, 200,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_energy},
		
		{  d_text_proc,     255, 210,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "angle:"},
		{  d_edit_proc,     315, 210,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_angle},
		
		{  d_text_proc,     255, 220,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "disappear:"},
		{  d_edit_proc,     335, 220,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_disappear},
		
		
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",current_enemy);
	
	
	sprintf(temp_active,"%d",enemy_data[current_enemy].active);
	sprintf(temp_angle,"%d",(int)enemy_data[current_enemy].angle);
	sprintf(temp_energy,"%d",(int)enemy_data[current_enemy].energy);
	sprintf(temp_action,"%d",enemy_data[current_enemy].action);
	sprintf(temp_disappear,"%d",enemy_data[current_enemy].disappear);
	sprintf(temp_name,"%s",enemy_data[current_enemy].name);
	
	mission_dialog[3].d1=enemy_data[current_enemy].current_mission;
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		strcpy(enemy_data[current_enemy].name, temp_name);		
		enemy_data[current_enemy].active = atoi(temp_active);
		enemy_data[current_enemy].angle = (float)atoi(temp_angle);
		enemy_data[current_enemy].energy = (float)atoi(temp_energy);
		enemy_data[current_enemy].action = atoi(temp_action);
		enemy_data[current_enemy].disappear = atoi(temp_disappear);
		
		enemy_data[current_enemy].current_mission=mission_dialog[3].d1;
	}
	

	return D_O_K;

}


int edit_enemy_path(void)
{
	char temp_type[10]="";
	
	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  300,  110,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 235,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 235,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		{  d_button_proc,     430, 235,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Edit" },
			
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Number:"},
		{  d_text_proc,     315, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   temp_num},
		
		{  d_text_proc,     255, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Type:"},
		{  d_edit_proc,     320, 170,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_type},
		
		{  d_text_proc,     255, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "0=don't walk  1=turn at end"},
		{  d_text_proc,     255, 190,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "2=stop at end 3=end to first"},
		

		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",enemy_path_chosen);
	
	sprintf(temp_type,"%d",enemy_data[current_enemy].movement_path[enemy_path_chosen].type);
	
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1 || ans==3)
	{
		enemy_data[current_enemy].movement_path[enemy_path_chosen].type = atoi(temp_type);
		
		if(ans==3)
		{
			editmode_enemy_path();		
			return D_EXIT;
		}

		


	}

	return D_O_K;
}


char *enemy_path_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MAX_ENEMY_PATH_NUM;
  return NULL;
 }
 
 sprintf(temp_string,"Path %d",index);
 
 return temp_string;
}




int d_enemy_path_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

 enemy_path_chosen=d->d1;
 
 
 if(ans==D_EXIT)
 {
	return edit_enemy_path();	
 }
   
 return ans;
}




int edit_enemy_path_data(void)
{
	char temp_path[10]="";
	
	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  300,  110,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 235,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 235,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_enemy_path_list_proc,   370,   160,     160,     50,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    enemy_path_list},
     	
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Number:"},
		{  d_text_proc,     315, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   temp_num},
		
		{  d_text_proc,     255, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Path:"},
		{  d_edit_proc,     320, 170,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_path},
		
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",current_enemy);
	
	sprintf(temp_path,"%d",enemy_data[current_enemy].current_movement_path);
	
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		enemy_data[current_enemy].current_movement_path= atoi(temp_path);
		
	}
	
	return D_O_K;
}




int delete_enemy(void)
{
	int ans;

	ans = alert("Sure you want to delete the enemy?",NULL,NULL,"Yes","Cancel",'\r',0);
	if(ans==2)return D_O_K;
	
	enemy_data[current_enemy].used=0;
	
	return D_O_K;
}	




int enemy_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Edit",      edit_enemy,      NULL,          0,         NULL},
	{ "Path", edit_enemy_path_data,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Delete",  delete_enemy,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_enemy=num;	
	
    ans=do_menu(temp_menu,mouse_x,mouse_y);
		
	return D_REDRAW;
}


/////////////////////////////////////////////////////////////////
///////// PATH FUMCNTIONS //////////////////////////////////////
////////////////////////////////////////////////////////////////

int make_new_enemy_path(int x, int y)
{
	if(enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points >= MAX_ENEMY_PATH_POINTS_NUM) //Check so we have space for the link
	{
		alert("No more points can be placed!",NULL,NULL,"OK",NULL,0,0);
		return D_O_K;
			
	}

	enemy_data[current_enemy].movement_path[enemy_path_chosen].point[enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points].x = x;
	enemy_data[current_enemy].movement_path[enemy_path_chosen].point[enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points].y = y;
	

	enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points++;


	return D_O_K;
}


void change_enemy_path_xy(int x, int y, int num)
{
	enemy_data[current_enemy].movement_path[enemy_path_chosen].point[num].x = x;
	enemy_data[current_enemy].movement_path[enemy_path_chosen].point[num].y = y;
	
}

int enemy_path_x(int num)
{
	return enemy_data[current_enemy].movement_path[enemy_path_chosen].point[num].x;
}

int enemy_path_y(int num)
{
	return enemy_data[current_enemy].movement_path[enemy_path_chosen].point[num].y;
}

int enemy_path_num(void)
{
	return enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points;
}

int delete_enemy_path(void)
{
	int i,ans;
	
	/*text_mode(0);
	textprintf(screen, font,100,100, makecol(255,255,255), "%d", enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points);
	readkey();
	*/
	
	
	ans = alert("Sure you want to delete the point?",NULL,NULL,"Yes","Cancel",'\r',0);
	
	if(selected<0 || selected>enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points-1)
		return D_O_K;

	if(ans==2)return D_O_K;

	for(i=selected;i<enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points-1;i++)
	 {
		enemy_data[current_enemy].movement_path[enemy_path_chosen].point[i].y = enemy_data[current_enemy].movement_path[enemy_path_chosen].point[i+1].y; 		 
 	 
		enemy_data[current_enemy].movement_path[enemy_path_chosen].point[i].x = enemy_data[current_enemy].movement_path[enemy_path_chosen].point[i+1].x; 		 

	};
	 
	enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points--;

	/*text_mode(0);
	textprintf(screen, font,100,100, makecol(255,255,255), "%d", enemy_data[current_enemy].movement_path[enemy_path_chosen].num_of_points);
	readkey();
	*/

	return D_O_K;

}


int enemy_path_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Delete",  delete_enemy_path,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	
    ans=do_menu(temp_menu,mouse_x,mouse_y);

	return D_REDRAW;
}








//The list-function for the NPCs
char *enemy_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = num_of_enemys;
  return NULL;
 }
 
 strcpy(temp_string, enemy_info[index].name);
 
 return temp_string;
}




//this makes the list
int d_enemy_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 enemy_chosen=d->d1;
  
 return ans;
}





