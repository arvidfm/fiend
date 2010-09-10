#include <allegro.h>

#include <stdio.h>

#include "../fiend.h"
#include "mapedit.h"

extern MENU editmode_menu[];

static char temp_string[180];

int npc_chosen=0;
int current_npc=0;

int npc_dialog_chosen=0;
int npc_dialog_text_chosen=0;
int npc_path_chosen=0;

extern int editmode_npc_path();

//make a new npc
void make_new_npc(int x, int y)
{
	int i,j,k;

	for(i=0;i<MAX_NPC_NUM;i++)
		if(npc_data[i].used==0)break;

	if(npc_data[i].used==1)
	{
		alert("No more npc:s can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
	}

	npc_data[i].used=1;
	
	npc_data[i].active=1;

	npc_data[i].dead=0;
	npc_data[i].was_dead=0;

	npc_data[i].type=npc_chosen;
	
	strcpy(npc_data[i].name,"noname");
	strcpy(npc_data[i].map_name, map->name);

	npc_data[i].x=x;
	npc_data[i].y=y;
	npc_data[i].angle=0;

	npc_data[i].energy=char_info[npc_chosen].energy;
	npc_data[i].action=0;
	npc_data[i].frame=0;
	npc_data[i].next_frame;

	npc_data[i].sp1=0;
	npc_data[i].sp2=0;
	npc_data[i].sp3=0;

	npc_data[i].current_movement_path=-1;

	npc_data[i].current_dialog=0;

	
	for(j=0;j<MAX_NPC_PATHS_NUM;j++)
	{
		npc_data[i].movement_path[j].type=0;
		npc_data[i].movement_path[j].num_of_points=0;
	}

	for(j=0;j<MAX_DIALOG_NUM;j++)
	{
		npc_data[i].dialog[j].num_of_texts=0;
		
		for(k=0;k<MAX_DIALOG_TEXT_NUM;k++)
		{
			strcpy(npc_data[i].dialog[j].text[k].string, "nothing");
			npc_data[i].dialog[j].text[k].face[0] = 0;
		}
	}



}


static int current_face =0;

char *dialog_face_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = num_of_message_faces+1;
  return NULL;
 }
 
 if(index!=0)
	strcpy(temp_string,message_face[index-1].name);
 else
	strcpy(temp_string,"None");
 
 return temp_string;
}


int d_dialog_face_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

 return ans;
}



void write_new_text(void)
{	
	
	int ans,i;
	char temp_text[180]="";

	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  300,  110,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 235,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 235,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
			
		{d_edit_proc,     260, 170,  280,   10,   FG_COLOR,   BG_COLOR,     0,  0,   MAX_DIALOG_TEXT_LENGTH-1,    0,   temp_text},
		
		{d_text_proc,     260, 185,  280,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Face:"},
		
		{d_dialog_face_list_proc,  305,   185,  100,     45,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    dialog_face_list},
     	
				
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	

	
	sprintf(temp_text,"%s",npc_data[current_npc].dialog[npc_dialog_chosen].text[npc_dialog_text_chosen].string);
	

	for(i=0;i<num_of_message_faces;i++)
		if(strcmp(message_face[i].name, npc_data[current_npc].dialog[npc_dialog_chosen].text[npc_dialog_text_chosen].face)==0)
		{
			mission_dialog[5].d1 = i+1;
			break;
		}

	if(npc_data[current_npc].dialog[npc_dialog_chosen].text[npc_dialog_text_chosen].face[0]==0)
	{
		mission_dialog[5].d1 = 0;
	}
	
	
	ans = popup_dialog(mission_dialog,-1);

	
	if(ans==1)
	{
		strcpy(npc_data[current_npc].dialog[npc_dialog_chosen].text[npc_dialog_text_chosen].string,temp_text);
	
		if(mission_dialog[5].d1==0)
			npc_data[current_npc].dialog[npc_dialog_chosen].text[npc_dialog_text_chosen].face[0]=0;
		else
			strcpy(npc_data[current_npc].dialog[npc_dialog_chosen].text[npc_dialog_text_chosen].face, message_face[mission_dialog[5].d1-1].name);

	}
	



}



char *dialog_text_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MAX_DIALOG_TEXT_NUM;
  return NULL;
 }
 
 strcpy(temp_string,npc_data[current_npc].dialog[npc_dialog_chosen].text[index].string);
 
 return temp_string;
}




int d_dialog_text_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

 npc_dialog_text_chosen=d->d1;
 
 
 if(ans==D_EXIT)
 {
	write_new_text();	
	return D_REDRAW;
 }
   
 return ans;
}



int edit_dialog_text(void)
{
	char temp_next[10]="";
	char temp_textnum[10]="";
	
	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  300,  110,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 235,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 235,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_dialog_text_list_proc,   370,   160,     160,     50,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    dialog_text_list},
     	
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Number:"},
		{  d_text_proc,     315, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   temp_num},
		
		
		{  d_text_proc,     255, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Next:"},
		{  d_edit_proc,     320, 170,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_next},
		
		{  d_text_proc,     255, 180,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Textnum:"},
		{  d_edit_proc,     320, 180,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_textnum},
		
		
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",npc_dialog_chosen);
	
	sprintf(temp_next,"%d",npc_data[current_npc].dialog[npc_dialog_chosen].next_dialog);
	sprintf(temp_textnum,"%d",npc_data[current_npc].dialog[npc_dialog_chosen].num_of_texts);
	
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		npc_data[current_npc].dialog[npc_dialog_chosen].next_dialog= atoi(temp_next);
		npc_data[current_npc].dialog[npc_dialog_chosen].num_of_texts= atoi(temp_textnum);
		
	}
	

	return D_O_K;

}


char *dialog_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MAX_DIALOG_NUM;
  return NULL;
 }
 
 sprintf(temp_string,"Dialog %d",index);
 
 return temp_string;
}




int d_dialog_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

 npc_dialog_chosen=d->d1;
 
 
 if(ans==D_EXIT)
 {
	edit_dialog_text();	
	return D_REDRAW;
 }
   
 return ans;
}



int edit_npc(void)
{
	char temp_angle[10]="";
	char temp_energy[10]="";
	char temp_dialog[10]="";
	char temp_active[10]="";
	char temp_name[40]="";
	char temp_action[10]="";
		

	char temp_num[10]="";

	int ans;
	
	
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  300,  110,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 235,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 235,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_dialog_list_proc,   420,   160,     110,     50,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    dialog_list},
     	
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
		
		{  d_text_proc,     255, 220,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "dialog:"},
		{  d_edit_proc,     315, 220,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_dialog},
		

		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",current_npc);
	
	
	sprintf(temp_active,"%d",npc_data[current_npc].active);
	sprintf(temp_angle,"%d",(int)npc_data[current_npc].angle);
	sprintf(temp_energy,"%d",npc_data[current_npc].energy);
	sprintf(temp_dialog,"%d",npc_data[current_npc].current_dialog);
	sprintf(temp_action,"%d",npc_data[current_npc].action);
	sprintf(temp_name,"%s",npc_data[current_npc].name);

	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		strcpy(npc_data[current_npc].name, temp_name);		
		npc_data[current_npc].active = atoi(temp_active);
		npc_data[current_npc].angle = atoi(temp_angle);
		npc_data[current_npc].energy = atoi(temp_energy);
		npc_data[current_npc].action = atoi(temp_action);
		npc_data[current_npc].current_dialog = atoi(temp_dialog);
		
	}
	

	return D_O_K;

}






int edit_npc_path(void)
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

	sprintf(temp_num,"%d",npc_path_chosen);
	
	sprintf(temp_type,"%d",npc_data[current_npc].movement_path[npc_path_chosen].type);
	
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1 || ans==3)
	{
		npc_data[current_npc].movement_path[npc_path_chosen].type = atoi(temp_type);
		
		if(ans==3)
		{
			editmode_npc_path();		
			return D_EXIT;
		}

		


	}

	return D_O_K;
}


char *npc_path_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MAX_NPC_PATH_NUM;
  return NULL;
 }
 
 sprintf(temp_string,"Path %d",index);
 
 return temp_string;
}




int d_npc_path_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

 npc_path_chosen=d->d1;
 
 
 if(ans==D_EXIT)
 {
	return edit_npc_path();	
 }
   
 return ans;
}




int edit_npc_path_data(void)
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
		
		{  d_npc_path_list_proc,   370,   160,     160,     50,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    npc_path_list},
     	
		{  d_text_proc,     255, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Number:"},
		{  d_text_proc,     315, 160,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   temp_num},
		
		{  d_text_proc,     255, 170,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Path:"},
		{  d_edit_proc,     320, 170,  33,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_path},
		
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_num,"%d",current_npc);
	
	sprintf(temp_path,"%d",npc_data[current_npc].current_movement_path);
	
	
	ans = popup_dialog(mission_dialog,-1);

	if(ans==1)
	{
		npc_data[current_npc].current_movement_path= atoi(temp_path);
		
	}
	
	return D_O_K;
}





int delete_npc(void)
{
	int ans;

	ans = alert("Sure you want to delete the npc?",NULL,NULL,"Yes","Cancel",'\r',0);
	if(ans==2)return D_O_K;
	
	npc_data[current_npc].used=0;
	
	return D_O_K;
}	




int npc_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Edit",      edit_npc,      NULL,          0,         NULL},
	{ "Path", edit_npc_path_data,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Delete",  delete_npc,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_npc=num;	
	
    ans=do_menu(temp_menu,mouse_x,mouse_y);

		
	return D_REDRAW;
}






/////////////////////////////////////////////////////////////////
///////// PATH FUMCNTIONS //////////////////////////////////////
////////////////////////////////////////////////////////////////

int make_new_npc_path(int x, int y)
{
	if(npc_data[current_npc].movement_path[npc_path_chosen].num_of_points >= MAX_NPC_PATH_POINTS_NUM) //Check so we have space for the link
	{
		alert("No more points can be placed!",NULL,NULL,"OK",NULL,0,0);
		return D_O_K;
			
	}

	npc_data[current_npc].movement_path[npc_path_chosen].point[npc_data[current_npc].movement_path[npc_path_chosen].num_of_points].x = x;
	npc_data[current_npc].movement_path[npc_path_chosen].point[npc_data[current_npc].movement_path[npc_path_chosen].num_of_points].y = y;
	

	npc_data[current_npc].movement_path[npc_path_chosen].num_of_points++;


	return D_O_K;
}


void change_npc_path_xy(int x, int y, int num)
{
	npc_data[current_npc].movement_path[npc_path_chosen].point[num].x = x;
	npc_data[current_npc].movement_path[npc_path_chosen].point[num].y = y;
	
}

int npc_path_x(int num)
{
	return npc_data[current_npc].movement_path[npc_path_chosen].point[num].x;
}

int npc_path_y(int num)
{
	return npc_data[current_npc].movement_path[npc_path_chosen].point[num].y;
}

int npc_path_num(void)
{
	return npc_data[current_npc].movement_path[npc_path_chosen].num_of_points;
}

int delete_npc_path(void)
{
	int i,ans;
	ans = alert("Sure you want to delete the point?",NULL,NULL,"Yes","Cancel",'\r',0);
	
	if(selected<0 || selected>npc_data[current_npc].movement_path[npc_path_chosen].num_of_points-1)
		return D_O_K;

	if(ans==2)return D_O_K;

	for(i=selected;i<npc_data[current_npc].movement_path[npc_path_chosen].num_of_points-1;i++)
	 {
		npc_data[current_npc].movement_path[npc_path_chosen].point[i].y = npc_data[current_npc].movement_path[npc_path_chosen].point[i+1].y; 		 
 	 
		npc_data[current_npc].movement_path[npc_path_chosen].point[i].x = npc_data[current_npc].movement_path[npc_path_chosen].point[i+1].x; 		 

	};
	 
	npc_data[current_npc].movement_path[npc_path_chosen].num_of_points--;

	return D_O_K;

}


int npc_path_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Delete",  delete_npc_path,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	
    ans=do_menu(temp_menu,mouse_x,mouse_y);

	return D_REDRAW;
}




//The list-function for the NPCs
char *npc_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = num_of_characters;
  return NULL;
 }
 
 strcpy(temp_string, char_info[index].name);
 
 return temp_string;
}




//this makes the list
int d_npc_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 npc_chosen=d->d1;
  
 return ans;
}





