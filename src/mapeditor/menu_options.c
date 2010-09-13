#include <allegro.h>
#include <stdio.h>
#include "../fiend.h"
#include "mapedit.h"


extern MENU editmode_menu[];

static char temp_level[20]="";
static int ll_fuck_shit=0;

//choose the grid size
int options_grid_proc(void)
{
 char height[20]="";
 char width[20]="";
 
 
 int ans;
	
 	
 DIALOG temp_dialog[] =
 {
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
	{d_shadow_box_proc, 300, 260,  200, 90,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_text_proc,       323, 280,  50,  20,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,  "X-Grid:"},
    {d_edit_proc,       379, 280,  60,  20,  FG_COLOR,   BG_COLOR,     0,     0,     3,     0,  width},
    {d_text_proc,       323, 295,  50,  20,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,  "Y-Grid:"},
    {d_edit_proc,       379, 295,  60,  20,  FG_COLOR,   BG_COLOR,     0,     0,     3,     0,  height},
    {d_button_proc,     310, 320,  50,  20,  FG_COLOR,   BG_COLOR,     '\r',     D_EXIT,     0,     0,  "OK"},
    {d_button_proc,     380, 320,  50,  20,  FG_COLOR,   BG_COLOR,     0,     D_EXIT,     0,     0,  "Cancel"},
       
   {NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
 };
 
 sprintf(width,"%d",x_grid);
 sprintf(height,"%d",y_grid);

 ans = popup_dialog(temp_dialog,2);
  
 if(ans==5)
 {
	x_grid = atoi(width);
	y_grid = atoi(height);

 }



 return D_REDRAW;


}


int d_ll_edit_proc(int msg,DIALOG *d,int c)
{
	//text_mode(makecol(255,255,255));	
	textprintf_ex(screen,font,d->x,d->y,d->fg,makecol(255,255,255),"%d ",atoi(temp_level));
	return D_O_K;
}


static int level;

int d_ll_slider_proc(int msg,DIALOG *d,int c)
{
	int ans;
	
	ans = d_slider_proc(msg,d,c);
	
	if(d->d2!=level)
	{
		level = d->d2;
		sprintf(temp_level,"%d",d->d2);
		ans = D_REDRAW;
	}
	
	return ans;
	
}


//choose the light level
int options_light_proc(void)
{
 int ans;
 char temp_angle[20]="";
 char temp_outside[20]="";

 
 DIALOG temp_dialog[] =
 {
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
	{d_shadow_box_proc, 300, 260,  200, 160 ,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,     NULL},
    {d_text_proc,       320, 280,  140,  20,  FG_COLOR,   BG_COLOR,     0,     0,     0,    0,  "Light Level:"},
    {d_ll_slider_proc,     320, 295,  140,  20,  FG_COLOR,   BG_COLOR,     0,     0,     30,    map->light_level-1,  NULL},
    {d_text_proc,       320, 320,  120,  20,  FG_COLOR,   BG_COLOR,     0,     0,     0,    0,  "Light Level:"},
    {d_ll_edit_proc,    417, 320,  30,  20,  FG_COLOR,   BG_COLOR,     0,     D_EXIT,     3,    0,  temp_level},
    {d_button_proc,     310, 385,  50,  20,  FG_COLOR,   BG_COLOR,     0,     D_EXIT,     0,     0,  "OK"},
    {d_button_proc,     380, 385,  50,  20,  FG_COLOR,   BG_COLOR,     0,     D_EXIT,     0,     0,  "Cancel"},
    
	{d_text_proc,       320, 340,  80,  20,  FG_COLOR,   BG_COLOR,     0,     0,     0,    0,  "Player angle:"},
    {d_edit_proc,       440, 340,  40,  20,  FG_COLOR,   BG_COLOR,     0,     0,     5,  0,  temp_angle},
    
	{d_text_proc,       320, 360,  80,  20,  FG_COLOR,   BG_COLOR,     0,     0,     0,    0,  "Outside:"},
    {d_edit_proc,       400, 360,  40,  20,  FG_COLOR,   BG_COLOR,     0,     0,     5,  0,  temp_outside},
  	
   {NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
 };
 
 
 ll_fuck_shit = map->light_level-1;
 sprintf(temp_level,"%d",map->light_level-1);
 sprintf(temp_angle,"%d",map->player_angle);
 sprintf(temp_outside,"%d",map->outside);

 level = map->light_level;

 ans = popup_dialog(temp_dialog,2);

 if(ans==5)//was OK pressed?
 {
  map->light_level = temp_dialog[2].d2+1; //Set the light level...'+1' because 0 makes the game crash...
  map->player_angle = atoi(temp_angle);
  map->outside = atoi(temp_outside);
 }

 return D_REDRAW;
}










//EDIT MODES//////

//sets the diffrent edit modes nothing strange.....
//one funktion for each edit mode

int editmode_tiles(void)
{
 if(current_editmode!=EDITMODE_TILES)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_TILES;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  
 
  return D_REDRAW;
 }

  return D_O_K;
}





int editmode_lights(void)
{
 if(current_editmode!=EDITMODE_LIGHTS)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_LIGHTS;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}


int editmode_player(void)
{
 if(current_editmode!=EDITMODE_PLAYER)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_PLAYER;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}



int editmode_links(void)
{
 if(current_editmode!=EDITMODE_LINKS)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_LINKS;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}

int editmode_npcs(void)
{
 if(current_editmode!=EDITMODE_NPCS)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_NPCS;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}

int editmode_objects(void)
{
 if(current_editmode!=EDITMODE_OBJECTS)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_OBJECTS;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}

int editmode_soundemitors(void)
{
 if(current_editmode!=EDITMODE_SOUNDEMITORS)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_SOUNDEMITORS;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}


int editmode_areas(void)
{
 if(current_editmode!=EDITMODE_AREAS)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_AREAS;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}


int editmode_npc_path(void)
{
 if(current_editmode!=EDITMODE_NPC_PATH)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_NPC_PATH;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}

int editmode_enemy_path(void)
{
 if(current_editmode!=EDITMODE_ENEMY_PATH)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_ENEMY_PATH;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}



int editmode_enemy(void)
{
 if(current_editmode!=EDITMODE_ENEMY)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_ENEMY;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}

int editmode_items(void)
{
 if(current_editmode!=EDITMODE_ITEMS)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_ITEMS;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}

int editmode_pathnodes(void)
{
 if(current_editmode!=EDITMODE_PATHNODE)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_PATHNODE;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}

int editmode_brushes(void)
{
 if(current_editmode!=EDITMODE_BRUSH)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_BRUSH;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}

int editmode_shadows(void)
{
 if(current_editmode!=EDITMODE_SHADOWS)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_SHADOWS;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}

int editmode_look_at_area(void)
{
 if(current_editmode!=EDITMODE_LOOK_AT_AREAS)
 {
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags ^ D_SELECTED; 
  current_editmode = EDITMODE_LOOK_AT_AREAS;
  editmode_menu[current_editmode].flags = editmode_menu[current_editmode].flags | D_SELECTED; 
  selected=-1;
  return D_REDRAW; 
 }

  return D_O_K;
}
