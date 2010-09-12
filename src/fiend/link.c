////////////////////////////////////////////////////
// This file contains all the link funcs 
// it checks what happens when you step on a link.
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>

#include "../grafik4.h"
#include "../fiend.h"


//gets the number on link name
int get_link_num(char* name)
{
	int i;

	for(i=0;i<map->num_of_links;i++)
	{
		if(strcmp(name,map->link[i].name)==0)
			return i;
	}


}

//checks if the player has collided with link if so do some stuff
//check if the middle of the player is in the sqaure
void check_link_collison(void)
{
	int i;
	int temp=-1;
	int current_event;
	float part_of_link;
	char link_name[20];
	char map_name[80];

	//check for the collision....
	for(i=0;i<map->num_of_links;i++)
	{
		if(check_collision(player.x,player.y,1,1,map->link[i].x - map->link[i].w/2, map->link[i].y -map->link[i].h/2, map->link[i].w, map->link[i].h) )
		{
			temp = i;
			break;
		}

	}
	
	//if no link was hit exit
	if(temp==-1 || map->link[temp].event_chosen<0)
		return;
	
	//The link event in focus....
	current_event = map->link[temp].event_chosen;
	
	
	//If the link is a normal link (transports you to another map)
	if(map->link[temp].event[current_event].type==0)
	{
		
		//check if it's an vertical or horisontal link...calclate where on the link the player is...
		if(map->link[temp].type==0)
			part_of_link = (float)(player.y - (map->link[temp].y-(map->link[temp].h/2) ))/ (float)map->link[temp].h;
		else
			part_of_link = (float)(player.x - (map->link[temp].x-(map->link[temp].w/2) ))/ (float)map->link[temp].w;
	
		
		//if part of link is negative make it positive
		if(part_of_link<0)part_of_link = -part_of_link;
		
		//do this so that nothing bad happens...like the player getting stuck in a wall
		if(part_of_link<.5)part_of_link+=0.05;
		if(part_of_link>.5)part_of_link-=0.05;

		
		//Copy the links name and the new maps file name as these will be erased soon
		strcpy(link_name, map->link[temp].event[current_event].link_name);
		strcpy(map_name, map->link[temp].event[current_event].map_file);
	
	
		//This some old stuff made the game crash but it saves memory ....maybe the bug will be fixed
		//release_map(map);
		//load_map(map,map_name);
		
		//I don't think you need to clear the map....but if it buggs I'll put it back....
		//clear_map(map);
		

		///////////////////////////////////
		//// THE MAP LOADING BEGINS////////
		///////////////////////////////////

		//stop_all_sounds();
		save_local_vars();

		//Load the new map...
		printf("Loading map %s %s\n", map, map_name);
		load_edit_map(map, map_name);
		load_local_vars();
		
		///////////////////////////////////
		//// THE MAP LOADING ENDS////////
		///////////////////////////////////
			
			
		//Get the number of the link
		temp = get_link_num(link_name);

				
		//Get the new player position you need ...
		if(map->link[temp].type==0)
		{
			player.x = map->link[temp].x;//map->link[temp].y-(map->link[temp].h/2) + (map->link[temp].h*part_of_link);
			player.y = map->link[temp].y;//map->link[temp].x-(map->link[temp].w/2) + (map->link[temp].w/2);
		}
		else
		if(map->link[temp].type==1)
		{
			player.x = map->link[temp].x;//map->link[temp].x-(map->link[temp].w/2) + (map->link[temp].w*part_of_link);
			player.y = map->link[temp].y;//map->link[temp].y-(map->link[temp].h/2) + (map->link[temp].h/2);
		}
		
		
		//If the player collides with a new link make him walk in the same angles as before
		//until he don't touch the link anymore
		while(check_collision(player.x,player.y,1,1,map->link[temp].x - map->link[temp].w/2, map->link[temp].y -map->link[temp].h/2, map->link[temp].w, map->link[temp].h))
		{
			
			if(map->link[temp].type==0)
			{
				if(player.last_dx>0)
					player.x+= 1;
				else
					player.x-= 1;
			}
			else if(map->link[temp].type==1)
			{
				if(player.last_dy>0)
					player.y+= 1;
				else
					player.y-= 1;
			}
			
			
		}
		
		//if palyer is stuck in a tile then move him out of there!
		if(check_tile_collision(player.x-char_info[0].w/2, player.y-char_info[0].h/2  ,char_info[0].w, char_info[0].h))
		{
			if(map->link[temp].type==0)
				player.y-=3;
			else
				player.x-=3;
			
			if(check_tile_collision(player.x-char_info[0].w/2, player.y-char_info[0].h/2  ,char_info[0].w, char_info[0].h))
			{
				if(map->link[temp].type==0)
					player.y+=6;
				else
					player.x+=6;
			}
		}


		
		//Update the map....
		check_triggers(0);
		get_current_map_objects();
		
		reset_npc_data();
		reset_enemy_data();
		reset_shells();
		reset_bloodpools();
		reset_missiles();
		reset_flames();
		reset_particles();
		reset_effects();
		reset_enemy_ai();
		reset_npc_ai();
		reset_beams();

		update_before_map();
		
		speed_counter=0;
			
	}
	else//The link just shows a message....
	{
		//get the player out of the  link 
		player.x-=player.last_dx;
		player.y-=player.last_dy;
		
		//show the message
		make_new_message(map->link[temp].event[current_event].message,NULL);
		
		//make the player move and turn a bit....
		auto_move_player(player.x-(player.last_dx*8),player.y-(player.last_dy*8),char_info[0].speed );


	}







}



