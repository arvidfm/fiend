////////////////////////////////////////////////////
// This file contains of the draw to the screen and buffer
//
// Code by Pierre Renaux and Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>

#include "../fiend.h"
#include "../draw.h"
#include "../grafik4.h"
#include "../console.h"


#define ITEM_LIGHT_DIST 80

extern int lights_flashes;


//////////////////////////////////////////////////////////
///////  Draw The light maps and and the light mask //////
///////////////////////////////////////////////////////////

void draw_the_lights(void)
{
 int i;	
 clear_to_color(mask, map->light_level);
 
 //---The light maps-------//
 for(i=0;i<map->num_of_lights;i++)
 {
	 if(map->light[i].active)
	 {
		 //if(check_collision(map_x, map_y, 480,480, map->light[i].world_x-lightmap_data[i]->w/2, map->light[i].world_y-lightmap_data[i]->h/2, lightmap_data[i]->w, lightmap_data[i]->h))
			 if(object_is_in_player_los(map->light[i].world_x,map->light[i].world_y,map->light[i].strech_w,map->light[i].strech_h,0,0)) 
				if(map->light[i].flash)
				{
					if(lights_flashes)	
						draw_lightmap2(mask, lightmap_data[i], map->light[i].world_x - (map_x) - map->light[i].strech_w/2, map->light[i].world_y - (map_y)- map->light[i].strech_h/2); 
				}
				else 	
					draw_lightmap2(mask, lightmap_data[i], map->light[i].world_x - (map_x) - map->light[i].strech_w/2, map->light[i].world_y - (map_y)- map->light[i].strech_h/2); 
	 }

 }


 //---´The Normal Lights----//
 for(i=0;i<NORMAL_LIGHT_NUM;i++)
	draw_normal_light(mask, i);

	
 //---The light mask-------//
 draw_lightsprite(virt, mask,0,0);
}





//////////////////////////////////////////////////////////
///////  Draw The Eneymys, Npc:s ,objects  and stuff //////
///////////////////////////////////////////////////////////


void draw_the_objects(void)
{
	int j,i,x,y;
	float temp_x, temp_y;
	
	//--- The low objects thats is objects with solid<1 ---//
	for(i=0;i<map->num_of_objects;i++)
		if(object_info[map->object[i].type].solid==0)
			if(map->object[i].active)
					draw_fiend_object(virt, &object_info[map->object[i].type], map->object[i].x-map_x, map->object[i].y-map_y, map->object[i].action, map->object[i].frame, map->object[i].angle);
			
	
	draw_particles(0);

	draw_bloodpools();

	
	//--- The wall shadows --//
	draw_wall_shadows(virt,map_x, map_y);
	
	//--- The low objects thats is objects with solid==1 ---//
	for(i=0;i<map->num_of_objects;i++)
		if(object_info[map->object[i].type].solid==1 && map->object[i].energy<0)
			if(map->object[i].active)
					draw_fiend_object(virt, &object_info[map->object[i].type], map->object[i].x-map_x, map->object[i].y-map_y, map->object[i].action, map->object[i].frame, map->object[i].angle);
	
	draw_tile_layer(virt, 2,1,  map_x, map_y);

	xyplus(PLAYER_USE_LENGTH, player.angle, &temp_x, &temp_y);
	x = player.x +temp_x;
	y = player.y +temp_y;

	// Items under obejcts (angle<0)

	for(j=0;j<current_map_item_num;j++)
	{
		i = current_map_item[j];
		if(!item_data[i].picked_up && item_data[i].active && item_data[i].used && item_data[i].angle<0)
		{
			if(object_is_in_player_los(item_data[i].x,item_data[i].y,item_info[item_data[i].type].w,item_info[item_data[i].type].h,0,0)) 
			{
				if(check_collision(x-PLAYER_PICKUP_W/2,y-PLAYER_PICKUP_H/2,PLAYER_PICKUP_W,PLAYER_PICKUP_H,item_data[i].x-item_info[item_data[i].type].w/2,item_data[i].y-item_info[item_data[i].type].h/2,item_info[item_data[i].type].w,item_info[item_data[i].type].h))
				{
					draw_fiend_item(virt, item_data[i].type, item_data[i].x-map_x, item_data[i].y-map_y,item_data[i].angle,1);
				}
				else
				{
					draw_fiend_item(virt, item_data[i].type, item_data[i].x-map_x, item_data[i].y-map_y,item_data[i].angle,0);
				}
			}
		}
	}
	
	
	//--- The low objects thats is objects with solid==1 ---//
	for(i=0;i<map->num_of_objects;i++)
		if(object_info[map->object[i].type].solid==1 && map->object[i].energy>=0)
			if(map->object[i].active)
					draw_fiend_object(virt, &object_info[map->object[i].type], map->object[i].x-map_x, map->object[i].y-map_y, map->object[i].action, map->object[i].frame, map->object[i].angle);
	
	
	
	//--- The dead enemies ---//
	for(j=0;j<current_map_enemy_num;j++)
	{
		i = current_map_enemy[j];
		if(enemy_data[i].dead && enemy_data[i].active && enemy_data[i].used)
		{
			if(1)//object_is_in_player_los(enemy_data[i].x,enemy_data[i].y,enemy_info[enemy_data[i].type].w,enemy_info[enemy_data[i].type].h,0,0)) 
			{
				draw_fiend_enemy(virt, &enemy_info[enemy_data[i].type], enemy_data[i].x-map_x, enemy_data[i].y-map_y, enemy_data[i].action, enemy_data[i].frame, enemy_data[i].angle);
			}
		}
	}
	
	
	
	//----The dead npcs ---//
	for(j=0;j<current_map_npc_num;j++)
	{
		i = current_map_npc[j];
		if(npc_data[i].dead && npc_data[i].active && npc_data[i].used)
		{
			if(1)//object_is_in_player_los(npc_data[i].x,npc_data[i].y,char_info[npc_data[i].type].w,char_info[npc_data[i].type].h,0,0)) 
			{
				draw_fiend_char(virt, &char_info[npc_data[i].type], npc_data[i].x-map_x, npc_data[i].y-map_y, npc_data[i].action, npc_data[i].frame, npc_data[i].angle);
			}
		}
	}

	//--- The Player ----//
	if(player.dead && player.energy>=-char_info[0].energy*2)
	{
		draw_fiend_char(virt, &char_info[0],player.x-map_x,player.y-map_y,player.action,player.frame,player.angle);
	}

	
	//--- The items over object (angle>-1) ---//
	
	for(j=0;j<current_map_item_num;j++)
	{
		i = current_map_item[j];
		if(!item_data[i].picked_up && item_data[i].active && item_data[i].used && item_data[i].angle>-1)
		{
			if(object_is_in_player_los(item_data[i].x,item_data[i].y,item_info[item_data[i].type].w,item_info[item_data[i].type].h,0,0)) 
			{
				if(check_collision(x-PLAYER_PICKUP_W/2,y-PLAYER_PICKUP_H/2,PLAYER_PICKUP_W,PLAYER_PICKUP_H,item_data[i].x-item_info[item_data[i].type].w/2,item_data[i].y-item_info[item_data[i].type].h/2,item_info[item_data[i].type].w,item_info[item_data[i].type].h))
				{
					draw_fiend_item(virt, item_data[i].type, item_data[i].x-map_x, item_data[i].y-map_y,item_data[i].angle,1);
				}
				else
				{
					draw_fiend_item(virt, item_data[i].type, item_data[i].x-map_x, item_data[i].y-map_y,item_data[i].angle,0);
				}
			}
		}
	}
	
	//--- the shells --//
	draw_shells();

	draw_particles(1);
	
	//--- The enemies under the player---//
	for(j=0;j<current_map_enemy_num;j++)
	{
		i = current_map_enemy[j];
		if(!enemy_data[i].dead && enemy_data[i].active && enemy_data[i].used && enemy_info[enemy_data[i].type].under_player)
		{
				
			if(1)//object_is_in_player_los(enemy_data[i].x,enemy_data[i].y,enemy_info[enemy_data[i].type].w,enemy_info[enemy_data[i].type].h,0,0)) 
			{
				draw_lightsprite(virt,enemy_shadow[enemy_data[i].type], enemy_data[i].x-enemy_shadow[enemy_data[i].type]->w/2-map_x, enemy_data[i].y - enemy_shadow[enemy_data[i].type]->h/2 -map_y);
				draw_fiend_enemy(virt, &enemy_info[enemy_data[i].type], enemy_data[i].x-map_x, enemy_data[i].y-map_y, enemy_data[i].action, enemy_data[i].frame, enemy_data[i].angle);
			}
			
		}
	}
	
	//--- The Player ----//
	if(!player.dead)
	{
		draw_lightsprite(virt,char_shadow[0], player.x -char_shadow[0]->w/2-map_x, player.y - char_shadow[0]->h/2 -map_y);
		draw_fiend_char(virt, &char_info[0],player.x-map_x,player.y-map_y,player.action,player.frame,player.angle);
	}

	
	//--- The enemies ---//
	for(j=0;j<current_map_enemy_num;j++)
	{
		i = current_map_enemy[j];
		if(!enemy_data[i].dead && enemy_data[i].active && enemy_data[i].used && !enemy_info[enemy_data[i].type].under_player)
		{
				
			if(1)//object_is_in_player_los(enemy_data[i].x,enemy_data[i].y,enemy_info[enemy_data[i].type].w,enemy_info[enemy_data[i].type].h,0,0)) 
			{
				draw_lightsprite(virt,enemy_shadow[enemy_data[i].type], enemy_data[i].x-enemy_shadow[enemy_data[i].type]->w/2-map_x, enemy_data[i].y - enemy_shadow[enemy_data[i].type]->h/2 -map_y);
				draw_fiend_enemy(virt, &enemy_info[enemy_data[i].type], enemy_data[i].x-map_x, enemy_data[i].y-map_y, enemy_data[i].action, enemy_data[i].frame, enemy_data[i].angle);
			}
			
		}
	}
	
	//----The npcs ---//
	for(j=0;j<current_map_npc_num;j++)
	{
		i = current_map_npc[j];
		if(npc_data[i].active && npc_data[i].used && !npc_data[i].dead)
		{
			if(1)//object_is_in_player_los(npc_data[i].x,npc_data[i].y,char_info[npc_data[i].type].w,char_info[npc_data[i].type].h,0,0)) 
			{
				draw_lightsprite(virt,char_shadow[npc_data[i].type], npc_data[i].x-char_shadow[npc_data[i].type]->w/2-map_x, npc_data[i].y - char_shadow[npc_data[i].type]->h/2 -map_y);
				draw_fiend_char(virt, &char_info[npc_data[i].type], npc_data[i].x-map_x, npc_data[i].y-map_y, npc_data[i].action, npc_data[i].frame, npc_data[i].angle);
		
			}
			
		}
	}
	
	
	//--- The missiles--//
	draw_missiles();

	
	draw_flames();

		
	//--- The high objects thats is objects with solid>1 ---//
	for(i=0;i<map->num_of_objects;i++)
		if(object_info[map->object[i].type].solid>1 && !object_info[map->object[i].type].additive)
			if(map->object[i].active)
				if(1)//object_is_in_player_los(map->object[i].x,map->object[i].y,object_info[map->object[i].type].w,object_info[map->object[i].type].h,map->object[i].angle,0)) 
				{
					draw_fiend_object(virt, &object_info[map->object[i].type], map->object[i].x-map_x, map->object[i].y-map_y, map->object[i].action, map->object[i].frame, map->object[i].angle);
				}
	
	draw_tile_layer(virt, 3,0,  map_x, map_y);
	
	draw_tile_layer(virt, 3,1,  map_x, map_y);
	
	draw_particles(2);

	
	if(lightning_is_on)
		draw_the_lights();
	
	//--- The high Additive objects thats is objects with solid>1 ---//
	for(i=0;i<map->num_of_objects;i++)
		if(object_info[map->object[i].type].solid>1 && object_info[map->object[i].type].additive)
			if(map->object[i].active)
				if(1)//object_is_in_player_los(map->object[i].x,map->object[i].y,object_info[map->object[i].type].w,object_info[map->object[i].type].h,map->object[i].angle,0)) 
				{
					draw_fiend_object(virt, &object_info[map->object[i].type], map->object[i].x-map_x, map->object[i].y-map_y, map->object[i].action, map->object[i].frame, map->object[i].angle);
				}
		
}



//////////////////////////////////////////////////////////
///////  The main function //////
///////////////////////////////////////////////////////////
void draw_level(void)
{
	clear_los_buffer();
	update_los_buffer(map_x,map_y);

	draw_tile_layer(virt, 1,0,  map_x, map_y);
	draw_tile_layer(virt, 1,1,  map_x, map_y);
	draw_tile_layer(virt, 2,0,  map_x, map_y);
		
	draw_the_objects();
		
		
	draw_particles(3);
	draw_beams();

	draw_tile_layer(virt, 3,2,  map_x, map_y);
	
	
	draw_los_buffer(virt,map_x,map_y);

	draw_effects();

		
	draw_pickup_message();

	
	//Uptade the invemtory
	if(inventory_is_on)update_inventory_graphic();
	

	//Uptade the invemtory
	if(fiend_note_is_on)update_note_graphic();
	
	//---- Draw the current message -----//
	draw_message();

	
	//----begin some test shit....---//
	if(debug_is_on)
	{
		text_mode(-1);
		
		textprintf(virt,font_small1->dat,0,0,makecol(255,255,255),"FPS:%.1f   ",(float) show_frame_speed());
			
		textprintf(virt,font_avalon->dat,381,4,makecol(0,0,0),"Health: %d%%",player.energy);
		textprintf(virt,font_avalon->dat,380,3,makecol(255,255,255),"Health: %d%%",player.energy);

		textprintf(virt,font_small1->dat,150,0,makecol(255,255,255),"Mapname: %s",map->name);
		textprintf(virt,font_small1->dat,150,10,makecol(255,255,255),"Mapfile: %s",map_file);
	
		
		draw_engine_error();
	}

    //IF we are in the console don't blit it to screen,
	if(csl_started)
		return;

    if(screen_is_black)
    { 
  		vsync();
		acquire_screen();
		clear(screen);
		release_screen();
    }
    else
    {
		//------The main blit-----//
		if(vsync_is_on)vsync();
		acquire_screen();
		blit(virt, screen, 0,0,80,0,480,480);
		release_screen();
    }
}













/////////////////////////////////
///////TEST//////////////////////
////////////////////////////////


void draw_object_rect(BITMAP *dest, float x1, float y1, int w1, int h1, float angle1)
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
		line(dest,corner1_x[dline[i][0]], corner1_y[dline[i][0]], corner1_x[dline[i][1]], corner1_y[dline[i][1]],makecol(255,255,255));
		
	

}

