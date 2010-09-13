#include <allegro.h>
#include "../fiend.h"
#include "../draw.h"
#include "mapedit.h"
#include "../grafik4.h"

#include "../NPC.h"



BITMAP *t_buffer;
BITMAP *t_mask;



//soem extern functions
extern void make_new_light(int x, int y);
extern int lightmap_menu(int num);


extern void make_new_link(int x, int y);
extern int link_menu(int num);


extern void make_new_object(int x, int y);
extern int object_menu(int num);

extern void make_new_soundemitor(int x, int y);
extern int soundemitor_menu(int num);

extern void make_new_area(int x, int y);
extern int area_menu(int num);

extern void make_new_look_at_area(int x, int y);
extern int look_at_area_menu(int num);

extern void make_new_npc(int x, int y);
extern int npc_menu(int num);
extern void make_new_npc_path(int x, int y);
extern int npc_path_menu(int num);

extern void make_new_enemy(int x, int y);
extern int enemy_menu(int num);

extern void make_new_item(int x, int y);
extern int item_menu(int num);

extern void make_new_pathnode(int x, int y);
extern int pathnode_menu(int num);

extern int change_npc_path_xy(int,int,int num);

extern int npc_path_x(int num);
extern int npc_path_y(int num);
extern int npc_path_num(void);

extern void make_new_enemy_path(int x, int y);
extern int enemy_path_menu(int num);
extern int change_enemy_path_xy(int,int,int num);

extern int enemy_path_x(int num);
extern int enemy_path_y(int num);
extern int enemy_path_num(void);

extern int sorted_tile_list[];

int selected=-1;


void object_rect(BITMAP *dest, float x1, float y1, int w1, int h1, float angle1, int col);



void draw_the_lights(void)
{
 int mouse_map_x;
 int mouse_map_y;
 int i;
 

 clear_to_color(t_mask, map->light_level);


 mouse_map_x = (mouse_x - 10)/32;
 mouse_map_y = (mouse_y - 40)/32;
	

 
 for(i=0;i<map->num_of_lights;i++)
 {
	 if(map->light[i].active)
		draw_lightmap(t_mask, lightmap_data[i], map->light[i].world_x - (map_x*32) - map->light[i].strech_w/2, map->light[i].world_y - (map_y*32)- map->light[i].strech_h/2); 
 }
 
 draw_lightsprite(t_buffer, t_mask,0,0); 

 	
 
}


void draw_the_info(void)
{
 int i,j;
 float temp_x, temp_y;
 int tile_x,tile_y;


 
 rect(t_buffer, 0-map_x*32,0-map_y*32,(0-map_x*32)+map->w*32,(0-map_y*32)+map->h*32,makecol(120,255,100));

 if(current_layer>1)
 {
	for(i=0;i<15;i++)
		for(j=0;j<15;j++)
		{
			tile_x = i+map_x;
			tile_y = j+map_y;
			if(tile_x>-1 && tile_x<map->w && tile_y>-1 && tile_y<map->h)
			{
				if(current_layer==3)
					if((map->layer3+ (tile_x) +( (tile_y) * map->w))->tile_num != 0 || (map->layer3+ (tile_x) +( (tile_y) * map->w))->tile_set !=0)
						if(tile_info[(map->layer3+ (tile_x) +( (tile_y) * map->w))->tile_set].tile[(map->layer3+ (tile_x) +( (tile_y) * map->w))->tile_num].masked==0)		
							rect(t_buffer,i*32,j*32,i*32+32,j*32+32,makecol(120,255,100)); 
						else
							rect(t_buffer,i*32,j*32,i*32+32,j*32+32,makecol(255,0,255)); 

				if(current_layer==2)
					if(tile_info[(map->layer2+ (tile_x) +( (tile_y) * map->w))->tile_set].tile[(map->layer2+ (tile_x) +( (tile_y) * map->w))->tile_num].masked==0)
						rect(t_buffer,i*32,j*32,i*32+32,j*32+32,makecol(120,255,100)); 

			}
		}
 }
 
 
 if(current_editmode==EDITMODE_PLAYER)
	rect(t_buffer,  map->player_x - char_info[0].w/2- (map_x*32), map->player_y - char_info[0].h/2- (map_y*32),
	map->player_x + char_info[0].w/2- (map_x*32), map->player_y + char_info[0].h/2- (map_y*32), makecol(120,255,100));


 if(current_editmode==EDITMODE_LIGHTS)
	for(i=0;i<map->num_of_lights;i++)
		if(selected==i)
			rect(t_buffer,map->light[i].world_x - (map_x*32)- map->light[i].strech_w/2, map->light[i].world_y - (map_y*32)- map->light[i].strech_h/2,
		     map->light[i].world_x - (map_x*32)+ map->light[i].strech_w- map->light[i].strech_w/2, map->light[i].world_y - (map_y*32)+ map->light[i].strech_h- map->light[i].strech_h/2,makecol(120,255,100));
			else
			rect(t_buffer,map->light[i].world_x - (map_x*32)- map->light[i].strech_w/2, map->light[i].world_y - (map_y*32)- map->light[i].strech_h/2,
		     map->light[i].world_x - (map_x*32)+ map->light[i].strech_w- map->light[i].strech_w/2, map->light[i].world_y - (map_y*32)+ map->light[i].strech_h- map->light[i].strech_h/2,makecol(255,0,255));
 
 //text_mode(-1);

 if(current_editmode==EDITMODE_LINKS)
	for(i=0;i<map->num_of_links;i++)
	{
		textout_centre_ex(t_buffer,font,map->link[i].name, map->link[i].x - (map_x*32), map->link[i].y - (map_y*32), makecol(255,0,255), -1);
		if(selected==i)
			rect(t_buffer,  map->link[i].x - map->link[i].w/2- (map_x*32), map->link[i].y - map->link[i].h/2- (map_y*32),
			     map->link[i].x + map->link[i].w/2- (map_x*32), map->link[i].y + map->link[i].h/2- (map_y*32), makecol(120,255,100));
			else
			rect(t_buffer,  map->link[i].x - map->link[i].w/2- (map_x*32), map->link[i].y - map->link[i].h/2- (map_y*32),
			     map->link[i].x + map->link[i].w/2- (map_x*32), map->link[i].y + map->link[i].h/2- (map_y*32), makecol(255,0,255));
	}

 if(current_editmode==EDITMODE_OBJECTS)
	for(i=0;i<map->num_of_objects;i++)
	{
		xyplus(object_info[map->object[i].type].h/2, map->object[i].angle, &temp_x, &temp_y);
		
		if(selected==i)
			circlefill(t_buffer,map->object[i].x+temp_x- (map_x*32), map->object[i].y+temp_y- (map_y*32),3, makecol(120,255,100));
		else
			circlefill(t_buffer,map->object[i].x+temp_x- (map_x*32), map->object[i].y+temp_y- (map_y*32),3, makecol(255,0,255));
		

		if(selected==i)
			object_rect(t_buffer,  map->object[i].x - (map_x*32), map->object[i].y - (map_y*32),
			     object_info[map->object[i].type].w, object_info[map->object[i].type].h, map->object[i].angle, makecol(120,255,100));
			else
			object_rect(t_buffer,  map->object[i].x- (map_x*32), map->object[i].y - (map_y*32),
			     object_info[map->object[i].type].w, object_info[map->object[i].type].h, map->object[i].angle, makecol(255,0,255));
	}
 
	 
 if(current_editmode==EDITMODE_SOUNDEMITORS)
	for(i=0;i<map->num_of_soundemitors;i++)
	{
		textout_centre_ex(t_buffer,font,map->soundemitor[i].sound_name, map->soundemitor[i].x - (map_x*32), map->soundemitor[i].y - (map_y*32)+10, makecol(255,0,255),-1);
		if(selected==i)
			circlefill(t_buffer, map->soundemitor[i].x- (map_x*32), map->soundemitor[i].y- (map_y*32),3,makecol(120,255,100));
		else
			circlefill(t_buffer, map->soundemitor[i].x- (map_x*32), map->soundemitor[i].y- (map_y*32),3,makecol(255,0,255));
	}	

 if(current_editmode==EDITMODE_AREAS)
	for(i=0;i<map->num_of_areas;i++)
	{
		circlefill(t_buffer, map->area[i].x - (map_x*32), map->area[i].y - (map_y*32),3,makecol(255,0,255));
		textout_centre_ex(t_buffer,font,map->area[i].name, map->area[i].x - (map_x*32), map->area[i].y - (map_y*32)+3, makecol(255,0,255),-1);
		if(selected==i)
			rect(t_buffer,  map->area[i].x - map->area[i].w/2- (map_x*32), map->area[i].y - map->area[i].h/2- (map_y*32),
			     map->area[i].x + map->area[i].w/2- (map_x*32), map->area[i].y + map->area[i].h/2- (map_y*32), makecol(120,255,100));
			else
			rect(t_buffer,  map->area[i].x - map->area[i].w/2- (map_x*32), map->area[i].y - map->area[i].h/2- (map_y*32),
			     map->area[i].x + map->area[i].w/2- (map_x*32), map->area[i].y + map->area[i].h/2- (map_y*32), makecol(255,0,255));
	}
 
 if(current_editmode==EDITMODE_LOOK_AT_AREAS)
	for(i=0;i<map->num_of_look_at_areas;i++)
	{
		circlefill(t_buffer, map->look_at_area[i].x - (map_x*32), map->look_at_area[i].y - (map_y*32),3,makecol(255,0,255));
		textout_centre_ex(t_buffer,font,map->look_at_area[i].name, map->look_at_area[i].x - (map_x*32), map->look_at_area[i].y - (map_y*32)+3, makecol(255,0,255),-1);
		textout_centre_ex(t_buffer,font,"Look", map->look_at_area[i].x - (map_x*32), map->look_at_area[i].y - (map_y*32)-9, makecol(255,0,255),-1);
		if(selected==i)
			rect(t_buffer,  map->look_at_area[i].x - map->look_at_area[i].w/2- (map_x*32), map->look_at_area[i].y - map->look_at_area[i].h/2- (map_y*32),
			     map->look_at_area[i].x + map->look_at_area[i].w/2- (map_x*32), map->look_at_area[i].y + map->look_at_area[i].h/2- (map_y*32), makecol(120,255,100));
			else
			rect(t_buffer,  map->look_at_area[i].x - map->look_at_area[i].w/2- (map_x*32), map->look_at_area[i].y - map->look_at_area[i].h/2- (map_y*32),
			     map->look_at_area[i].x + map->look_at_area[i].w/2- (map_x*32), map->look_at_area[i].y + map->look_at_area[i].h/2- (map_y*32), makecol(255,0,255));
	}


 if(current_editmode==EDITMODE_NPCS)
	for(i=0;i<MAX_NPC_NUM;i++)
		if(strcmp(npc_data[i].map_name, map->name)==0 && npc_data[i].used)
			if(selected==i)
				rect(t_buffer,  npc_data[i].x - char_info[npc_data[i].type].w/2- (map_x*32), npc_data[i].y - char_info[npc_data[i].type].h/2- (map_y*32),
				npc_data[i].x + char_info[npc_data[i].type].w/2- (map_x*32), npc_data[i].y + char_info[npc_data[i].type].h/2- (map_y*32), makecol(120,255,100));
			else
				rect(t_buffer,  npc_data[i].x - char_info[npc_data[i].type].w/2- (map_x*32), npc_data[i].y - char_info[npc_data[i].type].h/2- (map_y*32),
				npc_data[i].x + char_info[npc_data[i].type].w/2- (map_x*32), npc_data[i].y + char_info[npc_data[i].type].h/2- (map_y*32), makecol(255,0,255));
				

 if(current_editmode==EDITMODE_NPC_PATH)
	for(i=0;i<npc_path_num();i++)
	{
		circlefill(t_buffer, npc_path_x(i)- (map_x*32), npc_path_y(i)- (map_y*32),3,makecol(255,0,255));
		if(selected==i)
			textprintf_centre_ex(t_buffer, font,npc_path_x(i)- (map_x*32), npc_path_y(i)- (map_y*32)+4, makecol(120,255,100),-1,"Point %d",i);
		else
			textprintf_centre_ex(t_buffer, font,npc_path_x(i)- (map_x*32), npc_path_y(i)- (map_y*32)+4, makecol(255,0,255),-1,"Point %d",i);
	}	

 if(current_editmode==EDITMODE_ENEMY)
	for(i=0;i<MAX_ENEMY_DATA;i++)
		if(strcmp(enemy_data[i].map_name, map->name)==0 && enemy_data[i].used)
			if(selected==i)
				rect(t_buffer,  enemy_data[i].x - enemy_info[enemy_data[i].type].w/2- (map_x*32), enemy_data[i].y - enemy_info[enemy_data[i].type].h/2- (map_y*32),
				enemy_data[i].x + enemy_info[enemy_data[i].type].w/2- (map_x*32), enemy_data[i].y + enemy_info[enemy_data[i].type].h/2- (map_y*32), makecol(120,255,100));
			else
				rect(t_buffer,  enemy_data[i].x - enemy_info[enemy_data[i].type].w/2- (map_x*32), enemy_data[i].y - enemy_info[enemy_data[i].type].h/2- (map_y*32),
				enemy_data[i].x + enemy_info[enemy_data[i].type].w/2- (map_x*32), enemy_data[i].y + enemy_info[enemy_data[i].type].h/2- (map_y*32), makecol(255,0,255));
			
 if(current_editmode==EDITMODE_ENEMY_PATH)
	for(i=0;i<enemy_path_num();i++)
	{
		circlefill(t_buffer, enemy_path_x(i)- (map_x*32), enemy_path_y(i)- (map_y*32),3,makecol(255,0,255));
		if(selected==i)
			textprintf_centre_ex(t_buffer, font,enemy_path_x(i)- (map_x*32), enemy_path_y(i)- (map_y*32)+4, makecol(120,255,100),-1,"Point %d",i);
		else
			textprintf_centre_ex(t_buffer, font,enemy_path_x(i)- (map_x*32), enemy_path_y(i)- (map_y*32)+4, makecol(255,0,255),-1,"Point %d",i);
	}	
 
	
 if(current_editmode==EDITMODE_PATHNODE)
	for(i=0;i<map->num_of_path_nodes;i++)
	{
		if(selected==i)
		{
			rect(t_buffer,map->path_node[i].x-10- (map_x*32), map->path_node[i].y-10- (map_y*32),map->path_node[i].x+10- (map_x*32),map->path_node[i].y+10- (map_y*32),makecol(120,255,100));
			circlefill(t_buffer, map->path_node[i].x- (map_x*32), map->path_node[i].y- (map_y*32), 3, makecol(120,255,100));
		}
		else
		{
			rect(t_buffer,map->path_node[i].x-10- (map_x*32), map->path_node[i].y-10- (map_y*32),map->path_node[i].x+10- (map_x*32),map->path_node[i].y+10- (map_y*32),makecol(255,0,255));
			circlefill(t_buffer, map->path_node[i].x- (map_x*32), map->path_node[i].y- (map_y*32), 3, makecol(255,0,255));
		}
	}	
	
	
 if(current_editmode==EDITMODE_ITEMS)
	for(i=0;i<MAX_ITEM_DATA;i++)
		if(strcmp(item_data[i].map_name, map->name)==0 && item_data[i].used)
			if(selected==i)
				rect(t_buffer,  item_data[i].x - item_info[item_data[i].type].w/2- (map_x*32), item_data[i].y - item_info[item_data[i].type].h/2- (map_y*32),
				item_data[i].x + item_info[item_data[i].type].w/2- (map_x*32), item_data[i].y + item_info[item_data[i].type].h/2- (map_y*32), makecol(120,255,100));
			else
				rect(t_buffer,  item_data[i].x - item_info[item_data[i].type].w/2- (map_x*32), item_data[i].y - item_info[item_data[i].type].h/2- (map_y*32),
				item_data[i].x + item_info[item_data[i].type].w/2- (map_x*32), item_data[i].y + item_info[item_data[i].type].h/2- (map_y*32), makecol(255,0,255));




}



void draw_the_objects(void)
{
	int i;

	for(i=0;i<map->num_of_objects;i++)
	{
		if(object_info[map->object[i].type].solid==0)
			draw_fiend_object(t_buffer, &object_info[map->object[i].type], map->object[i].x-map_x*32, map->object[i].y-map_y*32, 0,0,map->object[i].angle);
		
	}
	
	draw_wall_shadows(t_buffer,map_x*32, map_y*32);
	 
	
	for(i=0;i<map->num_of_objects;i++)
	{
		if(object_info[map->object[i].type].solid==1 && map->object[i].energy<0)
			draw_fiend_object(t_buffer, &object_info[map->object[i].type], map->object[i].x-map_x*32, map->object[i].y-map_y*32, 0,0,map->object[i].angle);
		
	}
	
	draw_tile_layer(t_buffer, 2,1,  map_x*32, map_y*32);
	
	
	for(i=0;i<map->num_of_objects;i++)
	{
		if(object_info[map->object[i].type].solid==1 && map->object[i].energy>=0)
			draw_fiend_object(t_buffer, &object_info[map->object[i].type], map->object[i].x-map_x*32, map->object[i].y-map_y*32, 0,0,map->object[i].angle);
		
	}

	for(i=0;i<MAX_ITEM_DATA;i++)
	{
		if(item_data[i].used)
			if(strcmp(item_data[i].map_name, map->name)==0)
				draw_fiend_item(t_buffer, item_data[i].type,item_data[i].x-map_x*32, item_data[i].y-map_y*32,item_data[i].angle,0);

	}

	
	for(i=0;i<MAX_NPC_NUM;i++)
	{
		if(npc_data[i].used)
			if(strcmp(npc_data[i].map_name, map->name)==0)
				draw_fiend_char(t_buffer, &char_info[npc_data[i].type],npc_data[i].x-map_x*32, npc_data[i].y-map_y*32,0,0,npc_data[i].angle);

	}
	
	for(i=0;i<MAX_ENEMY_DATA;i++)
	{
		if(enemy_data[i].used)
			if(strcmp(enemy_data[i].map_name, map->name)==0)
				draw_fiend_enemy(t_buffer, &enemy_info[enemy_data[i].type],enemy_data[i].x-map_x*32, enemy_data[i].y-map_y*32,0,0,enemy_data[i].angle);

	}

	if(current_editmode==EDITMODE_PLAYER)
		 draw_fiend_char(t_buffer, &char_info[0],map->player_x-map_x*32, map->player_y-map_y*32,0,0,map->player_angle);
	
	
	for(i=0;i<map->num_of_objects;i++)
	{
		if(object_info[map->object[i].type].solid>1 && !object_info[map->object[i].type].additive)
			draw_fiend_object(t_buffer, &object_info[map->object[i].type], map->object[i].x-map_x*32, map->object[i].y-map_y*32, 0,0,map->object[i].angle);
	}

	 draw_tile_layer(t_buffer, 3,0,  map_x*32, map_y*32);
	
	 for(i=0;i<map->num_of_objects;i++)
	 {
		if(object_info[map->object[i].type].solid>1 && object_info[map->object[i].type].additive)
			draw_fiend_object(t_buffer, &object_info[map->object[i].type], map->object[i].x-map_x*32, map->object[i].y-map_y*32, 0,0,map->object[i].angle);
	 }

	 draw_tile_layer(t_buffer, 3,1,  map_x*32, map_y*32);
	
}



int d_drawmap_proc(int msg,DIALOG *d,int c)
{
 int i,j;

 int ans = D_O_K;
 
 int temp1,temp2,pic;

 static int mouse1_down = 0;
 static int mouse2_down = 0;

 int temp_w;
 int temp_h;

	
 int tile_x;
 int tile_y;
	
 int tile_num;
 int tile_set;
 
 int mouse_map_x;
 int mouse_map_y;
 
 switch(msg)
 {
 case MSG_START:
	 t_buffer = create_bitmap(d->w, d->h);
	 t_mask = create_bitmap_ex(8,d->w, d->h);
	 clear_to_color(t_buffer, makecol(255,0,0));
	 clear_to_color(t_mask, map->light_level);
	 init_draw();
	 break;
 
 case MSG_END:
	 destroy_bitmap(t_buffer);
	 break;

 case MSG_DRAW:
	 	 
	 draw_tile_layer(t_buffer, 1,0,  map_x*32, map_y*32);
	 draw_tile_layer(t_buffer, 1,1,  map_x*32, map_y*32);
	 draw_tile_layer(t_buffer, 2,0,  map_x*32, map_y*32);

	  
	 draw_the_objects();
	 
	 draw_the_lights();	
	 draw_tile_layer(t_buffer, 3,2,  map_x*32, map_y*32);
	 draw_the_info();
	 

	 vsync();
	 blit(t_buffer, screen, 0,0,d->x, d->y, d->w, d->h);

	 break;

 case MSG_CLICK:


	mouse_map_x = (mouse_x - d->x + (map_x*32));
	mouse_map_y = (mouse_y - d->y + (map_y*32));
	
	
	//------BEGIN EDIT MODE TILES------
	if(current_editmode==EDITMODE_TILES)
	{
		map_tile( mouse_x -d->x , mouse_y-d->y, &tile_x, &tile_y,TILE_SIZE); 

		tile_x+=map_x;
		tile_y+=map_y;

		if(tile_x<0 || tile_x>=map->w || tile_y<0 || tile_y>=map->h) break;

		
		if(mouse_b & 2)
		{
			if(current_layer==1)
			{
				tile_num = (map->layer1+ (tile_x) +( (tile_y) * map->w))->tile_num;
				tile_set = (map->layer1+ (tile_x) +( (tile_y) * map->w))->tile_set;
			}
			if (current_layer==2)
			{
				tile_num = (map->layer2+ (tile_x) +( (tile_y) * map->w))->tile_num;
				tile_set = (map->layer2+ (tile_x) +( (tile_y) * map->w))->tile_set;
			}
			if (current_layer==3)
			{
				tile_num = (map->layer3+ (tile_x) +( (tile_y) * map->w))->tile_num;
				tile_set = (map->layer3+ (tile_x) +( (tile_y) * map->w))->tile_set;
			}

			current_tile = tile_num;
			current_tileset = tile_set;

			for(i=0;i<num_of_tilesets;i++)
				if(sorted_tile_list[i] == tile_set)
				{
					main_dialog[10].d1 = i;
					break;
				}
			
		
			show_mouse(NULL);
			SEND_MESSAGE(&main_dialog[10], MSG_DRAW, 0);
			SEND_MESSAGE(&main_dialog[11], MSG_DRAW, 0);
			show_mouse(screen);

	
		}

		if(mouse_b & 1)
		{
			if(current_layer==1 && !tile_info[current_tileset].tile[current_tile].masked)
			{
				(map->layer1+ (tile_x) +( (tile_y) * map->w))->tile_num = current_tile;
				(map->layer1+ (tile_x) +( (tile_y) * map->w))->tile_set = current_tileset;
			}
			if(current_layer==2)
			{
				(map->layer2+ (tile_x) +( (tile_y) * map->w))->tile_num = current_tile;
				(map->layer2+ (tile_x) +( (tile_y) * map->w))->tile_set = current_tileset;
			}
			if (current_layer==3)
			{
				(map->layer3+ (tile_x) +( (tile_y) * map->w))->tile_num = current_tile;
				(map->layer3+ (tile_x) +( (tile_y) * map->w))->tile_set = current_tileset;
			}

			return D_REDRAWME;


		}
	}

	//------END EDIT MODE TILES------


	temp1 = 0;
	
	
	//------BEGIN EDIT MODE LIGHTS------
	if(current_editmode==EDITMODE_LIGHTS)
	{
		
		if(selected>=0)
		{
			temp_w = map->light[selected].strech_w/2;
			temp_h = map->light[selected].strech_h/2;
		}
		

		if((mouse_b & 1) && !mouse1_down && selected<0)
		{
			make_new_light( ((mouse_map_x - temp_w)-((mouse_map_x - temp_w)%x_grid))+temp_w,
				((mouse_map_y - temp_h)-((mouse_map_y - temp_h)%y_grid))+temp_h); //make a new light map!
			
			mouse1_down=1;
			return D_REDRAWME;
					
			
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			map->light[selected].world_x = ((mouse_map_x - temp_w)-((mouse_map_x - temp_w)%x_grid))+temp_w;
			map->light[selected].world_y = ((mouse_map_y - temp_h)-((mouse_map_y - temp_h)%y_grid))+temp_h;


			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=0;i<map->num_of_lights;i++)
				if(check_collision(mouse_map_x, mouse_map_y, 1,1, map->light[i].world_x- map->light[i].strech_w/2, map->light[i].world_y- map->light[i].strech_h/2, map->light[i].strech_w, map->light[i].strech_h) )
				{
					temp1=1; //a light has been pressed
					temp2=i; // the light number....
					break;
				}
		
			if(temp1 == 1) //If a light was pressed....
			{
				lightmap_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAWME;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}


	}
	//------END EDIT MODE LIGHTS------
	


	//------BEGIN EDIT MODE PLAYER POS------
	if(current_editmode==EDITMODE_PLAYER)
	{
		if(mouse_b & 1)
		{
			map->player_x = mouse_map_x;
			map->player_y = mouse_map_y;
			return D_REDRAWME;
			

		}


	}
	
	
	//------END EDIT MODE PLAYER POS------



	//------BEGIN EDIT MODE LINKS--------
	if(current_editmode==EDITMODE_LINKS)
	{
		
		

		if((mouse_b & 1) && !mouse1_down && selected<0)
		{
			make_new_link(mouse_map_x, mouse_map_y); //make a new link!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			map->link[selected].x = mouse_map_x;
			map->link[selected].y = mouse_map_y;


			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=0;i<map->num_of_links;i++)
				if(check_collision(mouse_map_x, mouse_map_y, 1,1, map->link[i].x - map->link[i].w/2, map->link[i].y - map->link[i].h/2, map->link[i].w, map->link[i].h )) 
				{
					temp1=1; //a light has been pressed
					temp2=i; // the light number....
					break;
				}
		
			if(temp1 == 1) //If a link was pressed....
			{
				link_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	}




	//-----END EDIT MODE LINKS--------

	





	//------BEGIN EDIT MODE OBJECTS--------
	if(current_editmode==EDITMODE_OBJECTS)
	{
		
		
		//!!!G�r om den h�r s� att den passar me roterade object �ss�!!!
		if(selected>=0)
		{
			pic = (int)map->object[selected].angle/90;
		
			if(pic == 0 || pic == 2 || pic == 4)
			{
				temp_w = object_info[map->object[selected].type].w/2;
				temp_h = object_info[map->object[selected].type].h/2;
			}
			else
			{
				temp_h = object_info[map->object[selected].type].w/2;
				temp_w = object_info[map->object[selected].type].h/2;
			}
		}
		
		if((mouse_b & 1) && !mouse1_down && selected<0)
		{
			make_new_object(((mouse_map_x - temp_w)-((mouse_map_x - temp_w)%x_grid))+temp_w,
				((mouse_map_y - temp_h)-((mouse_map_y - temp_h)%x_grid))+temp_h); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			
			
			map->object[selected].x = ((mouse_map_x - temp_w)-((mouse_map_x - temp_w)%x_grid))+temp_w;
			map->object[selected].y = ((mouse_map_y - temp_h)-((mouse_map_y - temp_h)%y_grid))+temp_h;

			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=map->num_of_objects-1;i>-1;i--)
				if(check_angle_collision(mouse_map_x,mouse_map_y, 2,2, 0,map->object[i].x, map->object[i].y,object_info[map->object[i].type].w,object_info[map->object[i].type].h,map->object[i].angle)) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
			
			if(temp1 == 1) //If an object was pressed....
			{
				object_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	}




	//-----END EDIT MODE OBJECTS--------



	//------BEGIN EDIT MODE SOUNDEMITORS--------
	if(current_editmode==EDITMODE_SOUNDEMITORS)
	{
		
		

		if((mouse_b & 1) && !mouse1_down && selected<0)
		{
			make_new_soundemitor(mouse_map_x, mouse_map_y); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			map->soundemitor[selected].x = mouse_map_x;
			map->soundemitor[selected].y = mouse_map_y;


			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=0;i<map->num_of_soundemitors;i++)
				if(check_collision(mouse_map_x, mouse_map_y, 1,1, map->soundemitor[i].x - 40, map->soundemitor[i].y - 10, 80, 20)) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
		
			if(temp1 == 1) //If an object was pressed....
			{
				soundemitor_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	  
	}




	//-----END EDIT MODE SOUNDEMITORS--------


	//------BEGIN EDIT MODE LOOK AT AREAS--------
	if(current_editmode==EDITMODE_LOOK_AT_AREAS)
	{
		
		

		if((mouse_b & 1) && !mouse1_down && selected<0)
		{
			make_new_look_at_area(mouse_map_x, mouse_map_y); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			map->look_at_area[selected].x = mouse_map_x;
			map->look_at_area[selected].y = mouse_map_y;


			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=0;i<map->num_of_look_at_areas;i++)
				if(check_collision(mouse_map_x, mouse_map_y, 1,1, map->look_at_area[i].x - map->look_at_area[i].w/2, map->look_at_area[i].y - map->look_at_area[i].h/2, map->look_at_area[i].w, map->look_at_area[i].h)) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
		
			if(temp1 == 1) //If an object was pressed....
			{
				look_at_area_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	  
	}




	//-----END EDIT MODE LOOK AT AREAS--------

	//------BEGIN EDIT MODE AREAS--------
	if(current_editmode==EDITMODE_AREAS)
	{
		
		

		if((mouse_b & 1) && !mouse1_down && selected<0)
		{
			make_new_area(mouse_map_x, mouse_map_y); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			map->area[selected].x = mouse_map_x;
			map->area[selected].y = mouse_map_y;


			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=0;i<map->num_of_areas;i++)
				if(check_collision(mouse_map_x, mouse_map_y, 1,1, map->area[i].x - map->area[i].w/2, map->area[i].y - map->area[i].h/2, map->area[i].w, map->area[i].h)) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
		
			if(temp1 == 1) //If an object was pressed....
			{
				area_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	  
	}




	//-----END EDIT MODE AREAS--------
	
	
	//------BEGIN EDIT MODE NPCS--------
	if(current_editmode==EDITMODE_NPCS)
	{
		
		

		if((mouse_b & 1) && !mouse1_down && selected<0)
		{	
		
			make_new_npc(mouse_map_x, mouse_map_y); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			npc_data[selected].x = mouse_map_x;
			npc_data[selected].y = mouse_map_y;


			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=0;i<MAX_NPC_NUM;i++)
				if(strcmp(npc_data[i].map_name, map->name)==0 && npc_data[i].used && check_collision(mouse_map_x, mouse_map_y, 1,1, npc_data[i].x - char_info[npc_data[i].type].w/2, npc_data[i].y - char_info[npc_data[i].type].h/2, char_info[npc_data[i].type].w, char_info[npc_data[i].type].h)) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
		
			if(temp1 == 1) //If an object was pressed....
			{
				npc_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	  
	}




	//-----END EDIT MODE NPCS--------

	//------BEGIN EDIT MODE NPC PATH--------
	if(current_editmode==EDITMODE_NPC_PATH)
	{
	
		if((mouse_b & 1) && !mouse1_down && selected<0)
		{
			
			make_new_npc_path(mouse_map_x,mouse_map_y); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			change_npc_path_xy(mouse_map_x,mouse_map_y,selected);

			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			
			for(i=0;i<npc_path_num();i++)
				if(check_collision(mouse_map_x, mouse_map_y, 1,1, npc_path_x(i)-40,npc_path_y(i)-20,80,40  )) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
		
			if(temp1 == 1) //If an object was pressed....
			{
				npc_path_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	  
	}




	//-----END EDIT MODE NPC PATH--------

	//------BEGIN EDIT MODE ENEMY--------
	if(current_editmode==EDITMODE_ENEMY)
	{
		
		

		if((mouse_b & 1) && !mouse1_down && selected<0)
		{	
		
			make_new_enemy(mouse_map_x, mouse_map_y); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			enemy_data[selected].x = mouse_map_x;
			enemy_data[selected].y = mouse_map_y;


			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=0;i<MAX_ENEMY_DATA;i++)
				if(strcmp(enemy_data[i].map_name, map->name)==0 && enemy_data[i].used && check_collision(mouse_map_x, mouse_map_y, 1,1, enemy_data[i].x - enemy_info[enemy_data[i].type].w/2, enemy_data[i].y - enemy_info[enemy_data[i].type].h/2, enemy_info[enemy_data[i].type].w, enemy_info[enemy_data[i].type].h)) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
		
			if(temp1 == 1) //If an object was pressed....
			{
				enemy_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	  
	}

	//-----END EDIT MODE ENEMY--------

	//------BEGIN EDIT MODE ENEMY PATH--------
	if(current_editmode==EDITMODE_ENEMY_PATH)
	{
	
		if((mouse_b & 1) && !mouse1_down && selected<0)
		{
			
			make_new_enemy_path(mouse_map_x,mouse_map_y); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			change_enemy_path_xy(mouse_map_x,mouse_map_y,selected);

			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			
			for(i=0;i<enemy_path_num();i++)
				if(check_collision(mouse_map_x, mouse_map_y, 1,1, enemy_path_x(i)-40,enemy_path_y(i)-20,80,40  )) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
		
			if(temp1 == 1) //If an object was pressed....
			{
				enemy_path_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	  
	}




	//-----END EDIT MODE ENEMY PATH--------
	
	//------BEGIN EDIT MODE ITEM--------
	if(current_editmode==EDITMODE_ITEMS)
	{
		
		

		if((mouse_b & 1) && !mouse1_down && selected<0)
		{	
		
			make_new_item(mouse_map_x, mouse_map_y); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			item_data[selected].x = mouse_map_x;
			item_data[selected].y = mouse_map_y;


			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=0;i<MAX_ITEM_DATA;i++)
				if(strcmp(item_data[i].map_name, map->name)==0 && item_data[i].used && check_collision(mouse_map_x, mouse_map_y, 1,1, item_data[i].x - item_info[item_data[i].type].w/2, item_data[i].y - item_info[item_data[i].type].h/2, item_info[item_data[i].type].w, item_info[item_data[i].type].h)) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
		
			if(temp1 == 1) //If an object was pressed....
			{
				item_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	  
	}

	//-----END EDIT MODE ITEM--------
	

	//------BEGIN EDIT MODE PATHNODE--------
	if(current_editmode==EDITMODE_PATHNODE)
	{
		
		if((mouse_b & 1) && !mouse1_down && selected<0)
		{	
		
			make_new_pathnode(mouse_map_x, mouse_map_y); //make a new object!
			
			mouse1_down=1;
			return D_REDRAWME;
					
		}
		
		if((mouse_b & 1) && selected>-1)
		{
			map->path_node[selected].x = mouse_map_x;
			map->path_node[selected].y = mouse_map_y;


			mouse1_down=1;
			return D_REDRAWME;
					
			
		}

		
		if((mouse_b & 2) && !mouse2_down)
		{
			for(i=0;i<MAX_PATHNODE_NUM;i++)
				if(check_collision(mouse_map_x, mouse_map_y, 1,1,map->path_node[i].x-10, map->path_node[i].y-10,20,20)) 
				{
					temp1=1; //a object has been pressed
					temp2=i; // the object number....
					break;
				}
		
			if(temp1 == 1) //If an object was pressed....
			{
				pathnode_menu(temp2);
				mouse2_down=1;
				selected=temp2;
				return D_REDRAW;

		
			}
			else
			{
				selected=-1;
				return D_REDRAWME;

			}
		 
			
			mouse2_down=1;
		}
	
	  
	}

	//-----END EDIT MODE PATHNODE--------

	//------BEGIN EDIT MODE BRUSH------
	if(current_editmode==EDITMODE_BRUSH)
	{
		map_tile( mouse_x -d->x , mouse_y-d->y, &tile_x, &tile_y,TILE_SIZE); 

		tile_x+=map_x;
		tile_y+=map_y;

		
		if(mouse_b & 1)
		{
			for(i=0;i<brush[brush_chosen].w;i++)
				for(j=0;j<brush[brush_chosen].h;j++)
				{
					tile_set = brush[brush_chosen].data[i][j].tile_set;
					tile_num = brush[brush_chosen].data[i][j].tile_num;
					
					if(tile_x+i>=0 && tile_x+i<map->w || tile_y+j>=0 || tile_y+j<map->h)
					{
						if(current_layer==1 && !tile_info[tile_set].tile[tile_num].masked)
						{
							(map->layer1+ (tile_x+i) +( (tile_y+j) * map->w))->tile_num = tile_num;
							(map->layer1+ (tile_x+i) +( (tile_y+j) * map->w))->tile_set = tile_set;
						}
						if(current_layer==2)
						{
							(map->layer2+ (tile_x+i) +( (tile_y+j) * map->w))->tile_num = tile_num;
							(map->layer2+ (tile_x+i) +( (tile_y+j) * map->w))->tile_set = tile_set;
						}
						if (current_layer==3)
						{
							(map->layer3+ (tile_x+i) +( (tile_y+j) * map->w))->tile_num = tile_num;
							(map->layer3+ (tile_x+i) +( (tile_y+j) * map->w))->tile_set = tile_set;
						}
					}
				}
				
			return D_REDRAWME;
		}
	}

	//------END EDIT MODE BRUSH------

	//------BEGIN EDIT MODE SHADOWS------

	if(current_editmode==EDITMODE_SHADOWS)
	{
		map_tile( mouse_x -d->x , mouse_y-d->y, &tile_x, &tile_y,TILE_SIZE); 

		tile_x+=map_x;
		tile_y+=map_y;

		if(tile_x<0 || tile_x>=map->w || tile_y<0 || tile_y>=map->h) break;

		
		if(mouse_b & 2)
		{
			current_shadow = *(map->shadow+ (tile_x) +( (tile_y) * map->w));
						
			show_mouse(NULL);
			SEND_MESSAGE(&main_dialog[33], MSG_DRAW, 0);
			SEND_MESSAGE(&main_dialog[34], MSG_DRAW, 0);
			show_mouse(screen);

	
		}

		if(mouse_b & 1)
		{
			*(map->shadow+ (tile_x) +( (tile_y) * map->w)) = current_shadow;
			

			return D_REDRAWME;


		}
	}
	//------END EDIT MODE SHADOWS------


	break;			

	

	case MSG_IDLE:
		if( !(mouse_b & 1) && mouse1_down)
			mouse1_down=0;
		if( !(mouse_b & 2) && mouse2_down)
			mouse2_down=0;

		
		break;


 }


 
 return D_O_K;
}


void object_rect(BITMAP *dest, float x1, float y1, int w1, int h1, float angle1, int col)
{
	int i;
	float corner1_x[4];
	float corner1_y[4];

	float temp_corner1_x[4];
	float temp_corner1_y[4];

	
	int dline[4][2]={{0,1},{1,2},{2,3},{3,0}};
	
	////-------Make the corners------////
	//for 1:
	temp_corner1_x[0] = -w1/2;
	temp_corner1_y[0] = -h1/2;
	
	temp_corner1_x[1] = w1/2;
	temp_corner1_y[1] = -h1/2;

	temp_corner1_x[2] = w1/2;
	temp_corner1_y[2] = h1/2;

	temp_corner1_x[3] = -w1/2;
	temp_corner1_y[3] = h1/2;
	
	
	//--------Rotate the corners------////
	
	for(i=0;i<4;i++)
	{
		corner1_x[i] = (temp_corner1_x[i]*COS(angle1)-temp_corner1_y[i]*SIN(angle1))+x1;
		corner1_y[i] = (temp_corner1_y[i]*COS(angle1)+temp_corner1_x[i]*SIN(angle1))+y1;
	}
	
	
	//test stuff
	for(i=0;i<4;i++)
		line(dest,corner1_x[dline[i][0]], corner1_y[dline[i][0]], corner1_x[dline[i][1]], corner1_y[dline[i][1]],col);
		
	

}
