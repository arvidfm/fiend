#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include "../fiend.h"
#include "mapedit.h"
#include "../draw.h"

//cheating...
int los_buffer[18][18];

char temp_string[40];

char editmode_string[][20] = {"Tiles","Objects","Items","NPC:s","Lights","Player Pos","Links","Areas","Look At Area","SoundEmitors","Enemys","Path Nodes","Brushes","Shadows"};


extern int current_npc;
extern int npc_path_chosen;
extern int current_enemy;
extern int enemy_path_chosen;


//print file and map name of the map that is being edited
int d_print_mapname_proc(int msg,DIALOG *d,int c)
{
	switch(msg)
	{
     case MSG_DRAW:
		textprintf(screen,font,d->x, d->y, makecol(0,0,0),"Filename: %s   Mapname: %s",get_filename(save_path),map->name,selected); 
		break;
	}
	return D_O_K;
}

int d_print_mapsize_proc(int msg,DIALOG *d,int c)
{
	switch(msg)
	{
     case MSG_DRAW:
		textprintf(screen,font,d->x, d->y, makecol(0,0,0),"Width: %d   Height: %d",map->w,map->h); 
		break;
	}
	return D_O_K;
}


int d_print_tileinfo_proc(int msg,DIALOG *d,int c)
{
	switch(msg)
	{
     case MSG_DRAW:
		text_mode(makecol(255,255,255));
		textprintf(screen,font,d->x, d->y, makecol(0,0,0),"Tileinfo:%d-%d-%d",tile_info[current_tileset].tile[current_tile].masked,  tile_info[current_tileset].tile[current_tile]. trans,tile_info[current_tileset].tile[current_tile].solid); 
		break;
	}
	return D_O_K;
}


int d_print_editmode_proc(int msg,DIALOG *d,int c)
{
	switch(msg)
	{
     case MSG_DRAW:
		if(current_editmode==EDITMODE_NPC_PATH)
			textprintf(screen,font,d->x, d->y, makecol(0,0,0),"Edit Mode: Path %d for NPC nr %d",npc_path_chosen,current_npc); 
		else if(current_editmode==EDITMODE_ENEMY_PATH)
			textprintf(screen,font,d->x, d->y, makecol(0,0,0),"Edit Mode: Path %d for enemy nr %d",enemy_path_chosen,current_enemy); 
		else
			textprintf(screen,font,d->x, d->y, makecol(0,0,0),"Edit Mode: %s", editmode_string[current_editmode]); 
		break;
	}
	return D_O_K;
}



int d_print_mousepos_proc(int msg,DIALOG *d,int c)
{
	switch(msg)
	{
	case MSG_DRAW:case MSG_IDLE:
		text_mode(makecol(255,255,255));
		textprintf(screen,font,d->x, d->y, makecol(0,0,0),"Mouse_x: %d Mouse_y: %d      ",mouse_x - 10 + (map_x*32),mouse_y - 40 + (map_y*32)); 
		textprintf(screen,font,d->x, d->y+10, makecol(0,0,0),"Path Nodes: %d   Tileset: %d  ",map->num_of_path_nodes,current_tileset); 
		break;
	}
	return D_O_K;
}




//If an arrow key gets pressed scroll the map
int key_up_proc(void)
{
  map_y--;
  show_mouse(NULL);
  SEND_MESSAGE(&main_dialog[1],MSG_DRAW,0);
  show_mouse(screen);
  return D_O_K;
}

int key_down_proc(void)
{
  map_y++;
  show_mouse(NULL);
  SEND_MESSAGE(&main_dialog[1],MSG_DRAW,0);
  show_mouse(screen);
  return D_O_K;
}

int key_right_proc(void)
{
  map_x++;
  show_mouse(NULL);
  SEND_MESSAGE(&main_dialog[1],MSG_DRAW,0);
  show_mouse(screen);
  return D_O_K;
}

int key_left_proc(void)
{
  map_x--;
  show_mouse(NULL);
  SEND_MESSAGE(&main_dialog[1],MSG_DRAW,0);
  show_mouse(screen);
  return D_O_K;
}





///////////////////////////////////////////////////
/////////// Tileset selection //////////////////////////
/////////////////////////////////////////////////

int sorted_tile_list[MAX_TILES];


void sort_tiles(void)
{
	int i,temp;
	int sorted=0;
	char string1[60], string2[60];
	
	for(i=0;i<num_of_tilesets;i++)
		sorted_tile_list[i]=i;


	while(!sorted)
	{
		sorted=1;

		for(i=0;i<num_of_tilesets-1;i++)
		{
			sprintf(string1,"%s",tile_info[sorted_tile_list[i]].name);
			sprintf(string2,"%s",tile_info[sorted_tile_list[i+1]].name);
			
			if(_stricmp(string1,string2)>0)
			{
				temp = sorted_tile_list[i];
				sorted_tile_list[i] = sorted_tile_list[i+1];
				sorted_tile_list[i+1] = temp;

				sorted=0;
			}
		}
	}

}



//The list-function for the tilesets
char *tileset_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = num_of_tilesets;
  return NULL;
 }
 
 sprintf(temp_string,"%s : %d",tile_info[sorted_tile_list[index]].name, sorted_tile_list[index]);
 
 return temp_string;
}


//this makes the list
int d_tileset_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 
 if(d->d1>-1 && current_tileset!=sorted_tile_list[d->d1])//&&  current_tileset!=sorted_tile_list[d->d1])
 {
		current_tileset=sorted_tile_list[d->d1];
		current_tile = 0;
		show_mouse(NULL);
		SEND_MESSAGE(&main_dialog[11],MSG_DRAW,0);
		//SEND_MESSAGE(&main_dialog[18],MSG_DRAW,0);
		show_mouse(screen);
 }

 return ans;
}


/////////////////////////////////////////////////
//////// The Tile list ////////////////////////
////////////////////////////////////////////////

extern BITMAP *the_buffer;

static BITMAP *tl_buffer;

//Shows all the tiles in the tileset and lets you select
int d_tile_list_proc(int msg, DIALOG *d, int c)
{
 int i;

 int new_row=0;
 int row=0;
 int kol=-1;
	
 int temp;

 switch(msg)
 {
 case MSG_START:
	 tl_buffer = create_bitmap(d->w, d->h);
	 clear_to_color(tl_buffer, makecol(255,0,255));
	 break;

 case MSG_END:
	 destroy_bitmap(tl_buffer);
	 break;
 
 case MSG_DRAW:

	 clear_to_color(the_buffer,makecol(255,255,255));
	 
	 //if(current_tileset<-1 || current_tileset>= num_of_tilesets)
	 //	current_tileset = 0;
	
	 for(i=0;i<tile_info[current_tileset].num_of_tiles;i++)
	 {
		 new_row++;
		 kol++;
		 if(new_row>d->w/TILE_SIZE)//Check in what row and collumn we shall draw 
		 {
			 row++;
			 new_row=1;
			 kol=0;
		 }

		 set_trans_blender(255,0,255,0);
		 if(i==current_tile)//if the tile is chosen draw it trans....
			 draw_lit_rle_sprite(the_buffer, tile_data[current_tileset][i].dat,TILE_SIZE*kol ,TILE_SIZE*row,130);
		 else
			 draw_rle_sprite(the_buffer, tile_data[current_tileset][i].dat,TILE_SIZE*kol,TILE_SIZE*row);
		 
		 rect(the_buffer,TILE_SIZE*kol, TILE_SIZE*row, TILE_SIZE*kol+TILE_SIZE, TILE_SIZE*row+TILE_SIZE,makecol(0,0,0));


	 }
	 rect(the_buffer,0,0, d->w-1, d->h-1, makecol(0,0,0));//Draw a rect around all tiles		
	 
	 blit(the_buffer, screen,0,0,d->x,d->y,d->w, d->h);
	 break;

 case MSG_CLICK:
	 
	 temp = ((mouse_x-d->x)/TILE_SIZE) + ( (d->w/TILE_SIZE) * ((mouse_y-d->y)/TILE_SIZE) );//Which was clicked...
	 
	 if(temp<tile_info[current_tileset].num_of_tiles)//If it is a tile...
	 {
		 current_tile = temp;//....make the tile the chosen
		 show_mouse(NULL);
		 SEND_MESSAGE(&main_dialog[11],MSG_DRAW,0);
		 SEND_MESSAGE(&main_dialog[18],MSG_DRAW,0);
	     show_mouse(screen);
	 }

	 
	 break;
 }

 return D_O_K;
}



/////////////////////////////////////////////////
//////// The Shadow list ////////////////////////
////////////////////////////////////////////////

BITMAP *sl_buffer;

int d_shadow_list_proc(int msg, DIALOG *d, int c)
{
 int i;

 int new_row=0;
 int row=0;
 int kol=-1;

 int temp;

 switch(msg)
 {
 case MSG_START:
	 sl_buffer = create_bitmap(d->w, d->h);
	 clear_to_color(sl_buffer, makecol(255,0,255));
	 break;

 case MSG_END:
	 destroy_bitmap(sl_buffer);
	 break;
 
 case MSG_DRAW:

	 clear_to_color(sl_buffer,makecol(255,255,255));

	 for(i=0;i<num_of_wall_shadows+1;i++)
	 {
		 kol++;
		 if(i*32>d->w-32 && row!=1)
		 {
			row=1;
			kol=0;		
		 }
		 
		 if(i==current_shadow+1)
			 rectfill(sl_buffer,kol*32,32*row,kol*32+32,32*row+ 32,makecol(255,0,255));

		 if(i>0)
			draw_lightsprite(sl_buffer, wall_shadow[i-1],kol*32,32*row);
		 rect(sl_buffer,kol*32,32*row,kol*32+32,32*row+32,makecol(0,0,0));
	 }

 	 rect(sl_buffer,0,0, d->w-1, d->h-1, makecol(0,0,0));//Draw a rect around all tiles

	 blit(sl_buffer, screen,0,0,d->x,d->y,d->w, d->h);
	 
	 break;

 case MSG_CLICK:
	 
	 temp = ((mouse_x-d->x)/TILE_SIZE) + ( (d->w/TILE_SIZE) * ((mouse_y-d->y)/TILE_SIZE) );//Which was clicked...
	 
	 if(temp<num_of_wall_shadows+1)//If it is a shadow...
	 {
		 current_shadow = temp-1;//....make the tile the chosen
		 show_mouse(NULL);
		 SEND_MESSAGE(&main_dialog[33],MSG_DRAW,0);
		 SEND_MESSAGE(&main_dialog[34],MSG_DRAW,0);
	     show_mouse(screen);
	 }
	 
	 break;
 }

 return D_O_K;
}





//When pushing a radio button the layer changes
int d_layer_radio_proc(int msg,DIALOG *d,int c)
{
 int ans;
 ans = d_radio_proc(msg,d,c);
 if(msg == MSG_CLICK)
  {
	 if(strcmp("1",d->dp)==0 ){  current_layer=1;return D_REDRAW;}
	 if(strcmp("2",d->dp)==0 ){  current_layer=2;return D_REDRAW;}
	 if(strcmp("3",d->dp)==0 ){  current_layer=3;return D_REDRAW;}
   }
 return ans;
}



//DRAW SQAURES
#define MAX_SQUARES 10

struct
{
	int used;
	int x;
	int y;
	int last_x;
	int last_y;
	
	BITMAP *buffer;
}square[MAX_SQUARES];


void init_squares(void)
{
	int i;

	for(i=0;i<MAX_SQUARES;i++)
	{
		square[i].used =0;
		
		square[i].x =0;
		square[i].y =0;
		
		square[i].buffer = create_bitmap(32,32);
	}

}


void make_square(int x, int y)
{
	int i;
	
	for(i=0;i<MAX_SQUARES;i++)
		if(!square[i].used)break;


}




/////CHEATING!!!!!!///////////
void update_tile_object_height(void)
{

}

int load_weapons(void)
{
	return 1;	
}

void release_weapons(void)
{
	return;	
}
int load_particles(void)
{
	return 1;	
}
int load_flames(void)
{
	return 1;	
}


void release_particles(void)
{
	return;	
}

void make_los_borders(void)
{
	return; 
}

void update_load_text(char *string)
{
	return;
}

void init_fiend_note(void)
{
	return;
}


int load_inventory_gfx(void)
{
	return 0;
}
void release_fiend_note(void)
{
	return;
}