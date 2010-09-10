#include <allegro.h>
#include <stdio.h>
#include "../fiend.h"
#include "mapedit.h"


static char temp_string[100];

//JUst show some stuff
int misc_about_proc(void)
{
 alert("The Fiend Mapeditor","By The FiendTeam 2000",
       "Version 1.0","OK",NULL,0,0);
 return D_O_K;


}


//JUst show some stuff
int misc_credits_proc(void)
{
	char text[]={
		"CREDITS\n\n"
		"Programming and Design:\n"
		"- Thomas Grip\n\n"
		"Tesing and Ideas:\n"
		"- Jonatan Hedborg\n\n\n"
		"Thanks to:\n"
		"- Shawn Hargreaves\n"
		"- All Allegro contributers\n"
		"- Ingava.com\n"
						
	};

 DIALOG temp_dialog[] =
 {
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
	{d_shadow_box_proc, 300, 260-70,  250, 200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_textbox_proc,    310, 270-70,  230, 150,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     text},
    {d_button_proc,     310, 430-70,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "OK"},
    {NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
 };
 
	
 popup_dialog(temp_dialog,-1);
 
 return D_REDRAW;
}


///////////////////////////////////////////
/////// Mini Map Stuff /////////////////
/////////////////////////////////////////

static BITMAP *temp_bmp;

int d_draw_minimap_proc(int msg, DIALOG *d, int c)
{
 int i,j;

 	
 switch(msg)
 {
 case MSG_START:
	 temp_bmp = create_bitmap(d->w, d->h);
	 clear_to_color(temp_bmp, makecol(255,255,255));
	 break;

 case MSG_END:
	 destroy_bitmap(temp_bmp);
	 break;
 
 case MSG_DRAW:

	 rect(screen,d->x-1, d->y-1,d->x+d->w, d->y+d->h,makecol(255,0,255)); 	
	 
	 clear_to_color(temp_bmp,makecol(255,255,255));

	 for(i=0;i<map->w;i++)
		 for(j=0;j<map->h;j++)
		 {
			 if( (map->layer2+ (i) +( (j) * map->w))->tile_num != 0 || (map->layer2+ (i) +( (j) * map->w))->tile_set !=0)
				 putpixel(temp_bmp,i,j,makecol(128,128,128));
			 if( (map->layer3+ (i) +( (j) * map->w))->tile_num != 0 || (map->layer3+ (i) +( (j) * map->w))->tile_set !=0)
				 putpixel(temp_bmp,i,j,makecol(0,0,0));

		 }

	 rect(temp_bmp,map_x-1,map_y-1,map_x+17,map_y+17,makecol(0,0,255));
	

	 blit(temp_bmp,screen,0,0,d->x,d->y,temp_bmp->w, temp_bmp->h);

	 
	 break;

 }

 return D_O_K;
}


int misc_minimap_proc(void)
{
 DIALOG temp_dialog[] =
 {
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
	{d_shadow_box_proc, 400-map->w/2-30, 300-map->h/2-10,  map->w+60, map->h+50,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_draw_minimap_proc, 400-map->w/2, 300-map->h/2, map->w, map->h,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_button_proc,     400-25, 310+map->h/2,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "OK"},
    {NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
 };
 	
 popup_dialog(temp_dialog,-1);
 
 return D_REDRAW;
}




///////////////////////////////////////////
/////// Global enemy stuff /////////////////
/////////////////////////////////////////

int global_enemy_chosen=0;

//The list-function for the NPCs
char *global_enemy_list(int index, int *list_size)
{
	if(index<0)
	{
		*list_size = MAX_ENEMY_DATA;
		return NULL;
	}
 
	if(enemy_data[index].used)
		sprintf(temp_string, "%d.%s - %s",index,enemy_data[index].name,enemy_data[index].map_name);
	else
		sprintf(temp_string, "%d.--Empty--",index);
 
	return temp_string;
}


//this makes the list
int d_global_enemy_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 global_enemy_chosen=d->d1;
  
 return ans;
}



int d_ge_button_proc(int msg,DIALOG *d,int c)
{
	int ans,ans2;
	ans = d_button_proc(msg,d,c);
	
	if(ans==D_EXIT)
	{
		ans2 = alert("Sure you want to delete the enemy?",NULL,NULL,"Yes","Cancel",'\r',0);
		
		if(ans2==1)
			enemy_data[global_enemy_chosen].used=0;
				
		return D_REDRAW;
	}

	return D_O_K;
}


int edit_global_enemies_proc(void)
{
	DIALOG temp_dialog[] =
 {
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
	{d_shadow_box_proc, 300, 260-70,  250, 200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_global_enemy_list_proc,    310, 270-70,  230, 150,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0, global_enemy_list},
    {d_button_proc,     310, 430-70,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "OK"},
    {d_ge_button_proc,     370, 430-70,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "Delete"},
    {NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
 };
 
	
 popup_dialog(temp_dialog,-1);
 
 return D_REDRAW;

}


///////////////////////////////////////////
/////// Global npc stuff /////////////////
/////////////////////////////////////////

int global_npc_chosen=0;

//The list-function for the NPCs
char *global_npc_list(int index, int *list_size)
{
	if(index<0)
	{
		*list_size = MAX_NPC_NUM;
		return NULL;
	}
 
	if(npc_data[index].used)
		sprintf(temp_string, "%d.%s - %s",index,npc_data[index].name,npc_data[index].map_name);
	else
		sprintf(temp_string, "%d.--Empty--",index);
 
	return temp_string;
}


//this makes the list
int d_global_npc_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 global_npc_chosen=d->d1;
  
 return ans;
}



int d_gn_button_proc(int msg,DIALOG *d,int c)
{
	int ans,ans2;
	ans = d_button_proc(msg,d,c);
	
	if(ans==D_EXIT)
	{
		ans2 = alert("Sure you want to delete the npc?",NULL,NULL,"Yes","Cancel",'\r',0);
		
		if(ans2==1)
			npc_data[global_npc_chosen].used=0;
				
		return D_REDRAW;
	}

	return D_O_K;
}


int edit_global_npcs_proc(void)
{
	DIALOG temp_dialog[] =
 {
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
	{d_shadow_box_proc, 300, 260-70,  250, 200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_global_npc_list_proc,    310, 270-70,  230, 150,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0, global_npc_list},
    {d_button_proc,     310, 430-70,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "OK"},
    {d_gn_button_proc,     370, 430-70,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "Delete"},
    {NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
 };
 
	
 popup_dialog(temp_dialog,-1);
 
 return D_REDRAW;

}




///////////////////////////////////////////
/////// Global item stuff /////////////////
/////////////////////////////////////////

int global_item_chosen=0;

//The list-function for the NPCs
char *global_item_list(int index, int *list_size)
{
	if(index<0)
	{
		*list_size = MAX_ITEM_DATA;
		return NULL;
	}
 
	if(item_data[index].used)
		sprintf(temp_string, "%d.%s - %s",index,item_info[item_data[index].type].name,item_data[index].map_name);
	else
		sprintf(temp_string, "%d.--Empty--",index);
 
	return temp_string;
}


//this makes the list
int d_global_item_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 global_item_chosen=d->d1;
  
 return ans;
}



int d_gi_button_proc(int msg,DIALOG *d,int c)
{
	int ans,ans2;
	ans = d_button_proc(msg,d,c);
	
	if(ans==D_EXIT)
	{
		ans2 = alert("Sure you want to delete the item?",NULL,NULL,"Yes","Cancel",'\r',0);
		
		if(ans2==1)
			item_data[global_item_chosen].used=0;
				
		return D_REDRAW;
	}

	return D_O_K;
}


int edit_global_items_proc(void)
{
	DIALOG temp_dialog[] =
 {
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
	{d_shadow_box_proc, 300, 260-70,  250, 200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_global_item_list_proc,    310, 270-70,  230, 150,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0, global_item_list},
    {d_button_proc,     310, 430-70,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "OK"},
    {d_gi_button_proc,     370, 430-70,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "Delete"},
    {NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
 };
 
	
 popup_dialog(temp_dialog,-1);
 
 return D_REDRAW;

}