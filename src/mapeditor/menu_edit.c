#include <stdio.h>
#include <stdlib.h>
#include <allegro.h>
#include "../fiend.h"
#include "../grafik4.h"
#include "mapedit.h"




int current_local_var=0;
int current_global_var=0;

static char temp_string[120];

//Change the maps name...
int edit_change_mapname_proc(void)
{
 int ans,i;
 char temp_name[40];
 DIALOG temp_dialog[] =
 {
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
	{d_shadow_box_proc, 300, 260,  200, 80 ,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_text_proc,       303, 280,  50,  20,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,  "Name:"},
    {d_edit_proc,       350, 280,  140,  20,  FG_COLOR,   BG_COLOR,     0,     0,     39,     0,  temp_name},
    {d_button_proc,     310, 305,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "OK"},
    {d_button_proc,     380, 305,  50,  20,  FG_COLOR,   BG_COLOR,     0,     D_EXIT,     0,     0,  "Cancel"},
       
   {NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
 };
 
 strcpy(temp_name, map->name);//display the current mapname
 ans = popup_dialog(temp_dialog,2);

 if(ans==3)//If OK was pressed change the name...
 {
	 for(i=0;i<MAX_NPC_NUM;i++)
	 	if(npc_data[i].used && strcmp(npc_data[i].map_name, map->name)==0)
			strcpy(npc_data[i].map_name, temp_name);
	
	for(i=0;i<MAX_ENEMY_DATA;i++)
	 	if(enemy_data[i].used && strcmp(enemy_data[i].map_name, map->name)==0)
			strcpy(enemy_data[i].map_name, temp_name);
	
	for(i=0;i<MAX_ITEM_DATA;i++)
	 	if(item_data[i].used && strcmp(item_data[i].map_name, map->name)==0)
			strcpy(item_data[i].map_name, temp_name);
 
	 
	 strcpy(map->name, temp_name);
 }

 return D_REDRAW;
}


int edit_clear_map_cord(void)
{
 map_x=0;
 map_y=0;

 return D_REDRAW;
}


int edit_change_mapnsize_proc(void)
{
 TILE_DATA temp_layer[MAX_LAYER_W][MAX_LAYER_H];

 char height[20]="";
 char width[20]="";
 
 int temp_w;
 int temp_h;

 int ans;
	
 int i,j;
	
 DIALOG temp_dialog[] =
 {
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
	{d_shadow_box_proc, 300, 260,  200, 90 ,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_text_proc,       323, 280,  50,  20,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,  "Width:"},
    {d_edit_proc,       379, 280,  60,  20,  FG_COLOR,   BG_COLOR,     0,     0,     3,     0,  width},
    {d_text_proc,       323, 295,  50,  20,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,  "Height:"},
    {d_edit_proc,       379, 295,  60,  20,  FG_COLOR,   BG_COLOR,     0,     0,     3,     0,  height},
    {d_button_proc,     310, 320,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "OK"},
    {d_button_proc,     380, 320,  50,  20,  FG_COLOR,   BG_COLOR,     0,     D_EXIT,     0,     0,  "Cancel"},
       
   {NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
 };
 
 sprintf(width,"%d",map->w);
 sprintf(height,"%d",map->h);

 ans = popup_dialog(temp_dialog,2);
 
 temp_w = map->w;
 temp_h = map->h;

 if(ans==5)
 {
  
	temp_w = atoi(temp_dialog[2].dp);
	temp_h = atoi(temp_dialog[4].dp);
    
	if(temp_w<map->w || temp_h<map->h)
	{
		ans = alert("The new size is smaller than the old.","Some parts of the map will be erased","Do you wish to continue?","Yes","Cancel",'\r',0);
		if(ans!=1)return D_REDRAW;
	}
     
	if(temp_w>=MAX_LAYER_W || temp_h>=MAX_LAYER_H)
	{
		alert("The new map is too big!!","Sucker..",NULL,"OK",NULL,'\r',0);
		return D_REDRAW;
	}

	if(temp_w!=map->w || temp_h!=map->h)
	{
		
		//clear the temp layer with 0 2...
		for(i=0;i<MAX_LAYER_W;i++)
		 for(j=0;j<MAX_LAYER_H;j++)
		 {
			 temp_layer[i][j].tile_num = 2;
		     temp_layer[i][j].tile_set = 0;
		 }

	    /*map_x =0;
		map_y =0;*/
		
		//transfer the first layer to the temp layer and then back in the right way
		for(i=0;i<map->w;i++)
		 for(j=0;j<map->h;j++)
		{
			 temp_layer[i][j].tile_num = (map->layer1+ (i) +( (j) * map->w))->tile_num;
			 temp_layer[i][j].tile_set = (map->layer1+ (i) +( (j) * map->w))->tile_set;

		}


		for(i=0;i<temp_w;i++)
		 for(j=0;j<temp_h;j++)
		 {
			 (map->layer1+ (i) +( (j) * temp_w))->tile_num = temp_layer[i][j].tile_num;
			 (map->layer1+ (i) +( (j) * temp_w))->tile_set = temp_layer[i][j].tile_set;
		 }
		
		
		//clear the temp layer with 0 0 
		for(i=0;i<MAX_LAYER_W;i++)
		 for(j=0;j<MAX_LAYER_H;j++)
		 {
			 temp_layer[i][j].tile_num = 0;
		     temp_layer[i][j].tile_set = 0;
		 }
 

		//transfer the second layer to the temp layer and then back in the right way
		for(i=0;i<map->w;i++)
		 for(j=0;j<map->h;j++)
		{
			 temp_layer[i][j].tile_num = (map->layer2+ (i) +( (j) * map->w))->tile_num;
			 temp_layer[i][j].tile_set = (map->layer2+ (i) +( (j) * map->w))->tile_set;

		}


		for(i=0;i<temp_w;i++)
		 for(j=0;j<temp_h;j++)
		 {
			 (map->layer2+ (i) +( (j) * temp_w))->tile_num = temp_layer[i][j].tile_num;
			 (map->layer2+ (i) +( (j) * temp_w))->tile_set = temp_layer[i][j].tile_set;
		 }



		//transfer the third layer to the temp layer and then back in the right way
		for(i=0;i<map->w;i++)
		 for(j=0;j<map->h;j++)
		{
			 temp_layer[i][j].tile_num = (map->layer3+ (i) +( (j) * map->w))->tile_num;
			 temp_layer[i][j].tile_set = (map->layer3+ (i) +( (j) * map->w))->tile_set;

		}


		for(i=0;i<temp_w;i++)
		 for(j=0;j<temp_h;j++)
		 {
			 (map->layer3+ (i) +( (j) * temp_w))->tile_num = temp_layer[i][j].tile_num;
			 (map->layer3+ (i) +( (j) * temp_w))->tile_set = temp_layer[i][j].tile_set;
		 }
				 

		//Change the size
		map->w = temp_w;
		map->h = temp_h;
		



	}




 }



 return D_REDRAW;
}




///////////////////////////////////////////////////
////////////////Local Var functions//////////////////////
///////////////////////////////////////////////////




void edit_local_var_value(void)
{
	int ans,i;
	
	char temp_name[40];
	char temp_value[10];


	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 300, 200,  200,  200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     305, 375,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		
		{  d_text_proc, 315,   205,   50,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Name:"},
		{  d_edit_proc, 365,   205,   120,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  39,    0,    temp_name},
			
		{  d_text_proc, 315,   245,   50,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Value:"},
		{  d_edit_proc, 365,   245,   80,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  7,    0,    temp_value},
					
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_name,"%s", map->var[current_local_var].name);
	sprintf(temp_value,"%d", map->var[current_local_var].value);

	
	ans = popup_dialog(trigger_dialog,-1);


	if(ans==1)
	{
		for(i=0;i<LOCAL_VAR_NUM;i++)
			if(i!=current_local_var)
				if(strcmp(map->var[i].name,temp_name)==0)
				{
					alert("The local variable name",temp_name,"is already taken!","Ok",NULL,'\r',0);
					return; 
				}
		
		strcpy(map->var[current_local_var].name, temp_name);
		map->var[current_local_var].value = atoi(temp_value);

	}	

	

}


	

char *local_vars_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = LOCAL_VAR_NUM;
  return NULL;
 }
 
 sprintf(temp_string,"%s - %d",map->var[index].name,map->var[index].value);

 return temp_string;
}




int d_local_vars_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);


 current_local_var =d->d1;

 if(ans==D_EXIT)
 {
	edit_local_var_value();
	 return D_REDRAW;
 }
 
 
 return ans;
}





int edit_local_vars(void)
{

	int ans;
	
	
	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 300, 200,  260,  200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     305, 375,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		
		{  d_local_vars_list_proc, 305,   205,   250,   165  ,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    local_vars_list},
     
					
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	
	
	ans = popup_dialog(trigger_dialog,-1);


	
	return D_O_K;
}



///////////////////////////////////////////////////
////////////////Global Var functions//////////////////////
///////////////////////////////////////////////////





void edit_global_var_value(void)
{
	int ans,i;
	
	char temp_name[40];
	char temp_value[10];


	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 300, 200,  200,  200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     305, 375,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		
		{  d_text_proc, 315,   205,   50,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Name:"},
		{  d_edit_proc, 365,   205,   120,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  39,    0,    temp_name},
			
		{  d_text_proc, 315,   245,   50,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Value:"},
		{  d_edit_proc, 365,   245,   80,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  7,    0,    temp_value},
					
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_name,"%s", global_var[current_global_var].name);
	sprintf(temp_value,"%d", global_var[current_global_var].value);

	
	ans = popup_dialog(trigger_dialog,-1);


	if(ans==1)
	{
		for(i=0;i<GLOBAL_VAR_NUM;i++)
			if(i!=current_global_var)
				if(strcmp(global_var[i].name,temp_name)==0)
				{
					alert("The global variable name",temp_name,"is already taken!","Ok",NULL,'\r',0);
					return; 
				}
		
		strcpy(global_var[current_global_var].name, temp_name);
		global_var[current_global_var].value = atoi(temp_value);

	}	

	

}


	

char *global_vars_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = GLOBAL_VAR_NUM;
  return NULL;
 }
 
 sprintf(temp_string,"%s - %d",global_var[index].name,global_var[index].value);

 return temp_string;
}




int d_global_vars_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);


 current_global_var =d->d1;

 if(ans==D_EXIT)
 {
	edit_global_var_value();
	 return D_REDRAW;
 }
 
 
 return ans;
}





int edit_global_vars(void)
{

	int ans;
	
	
	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 300, 200,  260,  200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     305, 375,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		
		{  d_global_vars_list_proc, 305,   205,   250,   165  ,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    global_vars_list},
     
					
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	
	
	ans = popup_dialog(trigger_dialog,-1);


	
	return D_O_K;
}











