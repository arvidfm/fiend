////////////////////////////////////////////////////
// This file contains NPC AI,animation and movement
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"

NPC_AI_DATA npc_ai[MAX_NPC_NUM];




/////////////////////////////////////////////////////////
//////////// MOVING THE NPC /////////////////////////////
/////////////////////////////////////////////////////////


//Check for collsion
int check_npc_collision(float x,float y, int num)
{
	int i,j;
	int type = npc_data[num].type;

	for(i=0;i<map->num_of_objects;i++)
	{
		if(map->object[i].active)
			if(object_info[map->object[i].type].solid>0 && object_info[map->object[i].type].animation[map->object[i].action].solid>0)
				if(check_angle_collision(x, y, char_info[type].w, char_info[type].h, 0, map->object[i].x, map->object[i].y, object_info[map->object[i].type].w,object_info[map->object[i].type].h,  map->object[i].angle))
					return 1;
	}
	
	for(j=0;j<current_map_enemy_num;j++)
	{
		i = current_map_enemy[j];
		if(!enemy_data[i].dead && enemy_data[i].active && enemy_data[i].used && !enemy_info[enemy_data[i].type].under_player)
			if(check_collision(x-char_info[type].w/2, y-char_info[type].h/2, char_info[type].w, char_info[type].h, enemy_data[i].x-enemy_info[enemy_data[i].type].w/2, enemy_data[i].y-enemy_info[enemy_data[i].type].h/2,enemy_info[enemy_data[i].type].w,enemy_info[enemy_data[i].type].h)) 
					return 1;
	}

	for(j=0;j<current_map_npc_num;j++)
	{
		i = current_map_npc[j];
		if(npc_data[i].active && npc_data[i].used && i!=num && !npc_data[i].dead)
			if(check_collision(x-char_info[type].w/2, y-char_info[type].h/2, char_info[type].w, char_info[type].h, npc_data[i].x-char_info[npc_data[i].type].w/2, npc_data[i].y-char_info[npc_data[i].type].h/2,char_info[npc_data[i].type].w,char_info[npc_data[i].type].h)) 
					return 1;
	}

	if(!player.dead && check_collision(x-char_info[type].w/2,y-char_info[type].h/2,char_info[type].w,char_info[type].h,
		               player.x-char_info[0].w/2,player.y-char_info[0].h/2,char_info[0].w,char_info[0].h))return 1;

	if(check_tile_collision(x-char_info[type].w/2,y-char_info[type].h/2,char_info[type].w,char_info[type].h))return 1;

	
	return 0;
}

//change the action
void change_npc_action(char *name, int num)
{
	int type = npc_data[num].type;

	if(npc_data[num].action!=char_action(&char_info[type],name))
	{
		npc_data[num].action = char_action(&char_info[type],name);
		npc_data[num].frame =0;
		npc_data[num].next_frame =0;
	}
}


//Update the moving
void update_npc_move(int num)
{
	int type;
	float temp_x, temp_y;
	float speed;
	
	
	if(npc_ai[num].hit_speed>0) //if the npc has been hit by a missile
	{
		xyplus(npc_ai[num].hit_speed, npc_ai[num].hit_angle, &temp_x, &temp_y);
		
		if(!check_npc_collision(npc_data[num].x+temp_x, npc_data[num].y, num))
			npc_data[num].x +=temp_x;
		
		if(!check_npc_collision(npc_data[num].x, npc_data[num].y+temp_y, num))
			npc_data[num].y +=temp_y;
		
		npc_ai[num].hit_speed-=0.03;
		
		if(npc_ai[num].hit_speed<0)
			npc_ai[num].hit_speed=0;
	}
	else if((npc_data[num].current_movement_path>-1 || npc_ai[num].panic) && !npc_data[num].dead)
	{
		
		type = npc_data[num].type;
		
		speed = char_info[type].speed;

		if(npc_ai[num].panic)
		{
			change_npc_action("running", num );
			speed*=char_info[type].run_add;
		}
		else
			change_npc_action("moving", num );
				 
		
		if((int)npc_data[num].angle != (int)npc_ai[num].wanted_angle)
		{
			/*if(compute_angle_distance(npc_ai[num].wanted_angle, npc_data[num].angle)<char_info[type].speed*4)
			{
				npc_data[num].angle = npc_ai[num].wanted_angle; 
			}
			else
			{
				dist = compute_angle_distance(npc_ai[num].wanted_angle, npc_data[num].angle);
		
				if(  (add_angle(npc_data[num].angle, dist)<npc_ai[num].wanted_angle+2) && (add_angle(npc_data[num].angle, dist)>npc_ai[num].wanted_angle-2) ) 
					npc_data[num].angle= add_angle(npc_data[num].angle, char_info[type].speed*2);
				else
					npc_data[num].angle= add_angle(npc_data[num].angle, -char_info[type].speed*2);
			}*/
			
			npc_data[num].angle = npc_ai[num].wanted_angle;


		}
		xyplus(speed, npc_data[num].angle, &temp_x, &temp_y);
		
		if(!check_npc_collision(npc_data[num].x+temp_x, npc_data[num].y, num))
			npc_data[num].x +=temp_x;
		
		if(!check_npc_collision(npc_data[num].x, npc_data[num].y+temp_y, num))
			npc_data[num].y +=temp_y;
		
	
	}
	else if(!npc_data[num].dead)
	{
		change_npc_action("still", num);
		
	}

}


//set all values in npc_ai to default
void total_reset_npc_ai(void)
{
	int i;

	for(i=0;i<MAX_NPC_NUM;i++)
	{
		npc_ai[i].current_point=0;
		npc_ai[i].next_point=1;
		npc_ai[i].last_point=-1;
		npc_ai[i].wanted_angle=0;
		npc_ai[i].panic=0;
		npc_ai[i].panic_x=-1;
		npc_ai[i].panic_y=-1;
		npc_ai[i].dead_and_still=0;
		npc_ai[i].animation=0;
		
	}
}

//set some values in npc_ai to default
void reset_npc_ai(void)
{
	int i;

	for(i=0;i<MAX_NPC_NUM;i++)
	{
		npc_ai[i].wanted_angle=0;
		npc_ai[i].panic=0;
		npc_ai[i].panic_x=-1;
		npc_ai[i].panic_y=-1;
		
	}
}


/////////////////////////////////////////////////////////
//////////// DIALOGS FOR THE NPC /////////////////////////
/////////////////////////////////////////////////////////

//Update the dialog...
void update_npc_dialog(int num)
{
	int i, type;
	int current_dialog;

	type = npc_data[num].type;
	
	if( npc_data[num].current_dialog>-1 && !npc_ai[num].panic && !npc_data[num].dead &&
		player_has_used && check_collision(player_use_x - PLAYER_USE_W/2, player_use_y - PLAYER_USE_W/2, PLAYER_USE_W, PLAYER_USE_H,
		npc_data[num].x - char_info[type].w/2, npc_data[num].y - char_info[type].h/2, char_info[type].w,char_info[type].h)) 
	{
		player_has_used=0;

		npc_data[num].angle = compute_angle(player.x, player.y,npc_data[num].x, npc_data[num].y);
		
		current_dialog = npc_data[num].current_dialog;
		
		for(i=0;i<npc_data[num].dialog[current_dialog].num_of_texts;i++)
			make_new_message(npc_data[num].dialog[current_dialog].text[i].string,npc_data[num].dialog[current_dialog].text[i].face);

		if(npc_data[num].dialog[current_dialog].next_dialog>-1)
			npc_data[num].current_dialog = npc_data[num].dialog[current_dialog].next_dialog;

	}
}

/////////////////////////////////////////////////////////
//////////// ANIMATION /////////////////////////////
/////////////////////////////////////////////////////////

//Update the animations for the npcs
void update_npc_animation(int num)
{
	int type;

	int tile_set, tile_num;;
	
	type = npc_data[num].type;
	
	npc_data[num].next_frame++;
	if(npc_data[num].next_frame>=ANIM_SPEED)
	{
		//update frame	
		npc_data[num].next_frame =0;
		npc_data[num].frame++;
		if(char_info[type].animation[npc_data[num].action].frame[npc_data[num].frame] ==255)
		{
			npc_data[num].frame = char_info[type].animation[npc_data[num].action].frame[npc_data[num].frame + 1 ];
			
			if(npc_ai[num].animation!=0){
				if(npc_ai[num].animation>-1)npc_ai[num].animation--;
				if(npc_ai[num].animation==0)change_npc_action("still", num);
			}
	
		}
		

		//footstep sound
		tile_set = (map->layer1+ ((int)npc_data[num].x/TILE_SIZE) +( ((int)npc_data[num].y/TILE_SIZE) * map->w))->tile_set;
		tile_num = (map->layer1+ ((int)npc_data[num].x/TILE_SIZE) +( ((int)npc_data[num].y/TILE_SIZE) * map->w))->tile_num;
		
		if(strcmp(char_info[type].animation[npc_data[num].action].name, "moving")==0 && (npc_data[num].frame==char_info[type].walk_step1 || npc_data[num].frame==char_info[type].walk_step2))
			play_fiend_sound(tile_info[tile_set].tile[tile_num].sound,npc_data[num].x,npc_data[num].y,1,0,180);
	
		if(strcmp(char_info[type].animation[npc_data[num].action].name, "running")==0 && (npc_data[num].frame==char_info[type].run_step1 || npc_data[num].frame==char_info[type].run_step2))
			play_fiend_sound(tile_info[tile_set].tile[tile_num].sound,npc_data[num].x,npc_data[num].y,1,0,180);
	
	}
}



/////////////////////////////////////////////////////////
//////////// PATH AND STUFF /////////////////////////////
/////////////////////////////////////////////////////////

//Update the path walking...
void update_npc_path(int num)
{
	int current_path;
	int goal_x, goal_y;
	int num_of_points;
	int temp;
	
	
	if(npc_data[num].current_movement_path>-1 && !npc_ai[num].panic)
	{

		current_path = npc_data[num].current_movement_path;

		//Get the pos we want to go to..
		goal_x = npc_data[num].movement_path[current_path].point[npc_ai[num].current_point].x;
		goal_y = npc_data[num].movement_path[current_path].point[npc_ai[num].current_point].y;

		//the number of points...
		num_of_points = npc_data[num].movement_path[current_path].num_of_points;


		//Check if you collide with the point,if yye do some stuff 
		if(check_collision(npc_data[num].x-2,npc_data[num].y-2,4,4,goal_x -1, goal_y-1,2,2))
		{
			//If this the first or the end point..
			if(num_of_points-1 ==  npc_ai[num].current_point || npc_ai[num].current_point==0)
			{
				//if we walk back and forward
				if(npc_data[num].movement_path[current_path].type==1)
				{
				
					if(npc_ai[num].current_point!=0)
						{
						npc_ai[num].last_point = npc_ai[num].current_point;
						npc_ai[num].current_point = npc_ai[num].current_point-1;
					}
					else
					{
						if(npc_ai[num].last_point==-1)
						{
							temp = npc_ai[num].last_point;
							npc_ai[num].last_point=npc_ai[num].current_point;
							npc_ai[num].current_point++;
						}
						else
						{	
							npc_ai[num].last_point=0;
							npc_ai[num].current_point=1;
						}
					}


					npc_ai[num].wanted_angle = get_best_npc_angle(npc_data[num].x,npc_data[num].y, goal_x, goal_y, num);
				}
				//If we walk and stop at end
				else if(npc_data[num].movement_path[current_path].type==2)
				{
					if(npc_ai[num].current_point!=0)
					npc_data[num].current_movement_path=-1;
					else
					{
						temp = npc_ai[num].last_point;
						npc_ai[num].last_point=npc_ai[num].current_point;
						npc_ai[num].current_point++;
					}
				}
				//If we walk and and then go to start at end
				else if(npc_data[num].movement_path[current_path].type==3)
				{
					if(npc_ai[num].current_point!=0)
					{
						npc_ai[num].current_point = 0;
						npc_ai[num].last_point = -1;
					}
					else
					{
						npc_ai[num].last_point=0;
						npc_ai[num].current_point=1;
					}

					npc_ai[num].wanted_angle = get_best_npc_angle(npc_data[num].x,npc_data[num].y, goal_x, goal_y,num);
				}

			}
			else //if it is just a normal point we collided with...
			{
				temp = npc_ai[num].last_point;
				npc_ai[num].last_point=npc_ai[num].current_point;
			
				if(temp<npc_ai[num].current_point)npc_ai[num].current_point++;
				else npc_ai[num].current_point--;

				npc_ai[num].wanted_angle = get_best_npc_angle(npc_data[num].x,npc_data[num].y, goal_x, goal_y,num);
			}
		}
		else //IF we did not collide witha point
		{
			npc_ai[num].wanted_angle = get_best_npc_angle(npc_data[num].x,npc_data[num].y, goal_x, goal_y, num);
		}


	}
	else if(npc_ai[num].panic)
	{
		npc_ai[num].wanted_angle =  get_best_npc_angle(npc_data[num].x,npc_data[num].y, npc_ai[num].panic_x, npc_ai[num].panic_y,num);

		if(check_collision(npc_data[num].x-10,npc_data[num].y-10,20,20,npc_ai[num].panic_x-10, npc_ai[num].panic_y-10, 20,20))
			npc_ai[num].panic_x=-1;
	}

}

/////////////////////////////////////////////////////////
//////////// DATA UPDATE /////////////////////////////
/////////////////////////////////////////////////////////


void reset_npc_data(void)
{
	int i;

	for(i=0;i<MAX_NPC_NUM;i++)
	{
		npc_ai[i].panic=0;
		npc_ai[i].panic_x=-1;
		npc_ai[i].panic_y=-1;
		npc_ai[i].hit_speed=0;
		npc_ai[i].hit_angle=0;

	}
}


void update_npc_data(int num)
{
	int type = npc_data[num].type;
	float temp_x,temp_y;
	

	////// Death stuff ////
	if(npc_data[num].energy<0 && !npc_data[num].dead)
	{
		if( npc_data[num].energy<-char_info[type].energy*3 )
		{
			npc_ai[num].dead_and_still=1;
			npc_data[num].dead=1;
			npc_data[num].active=0;
			enemy_ai[num].animation=0;
			play_fiend_sound("splash",npc_data[num].x,npc_data[num].y, 1,0,180);
			make_flesh_explosion(npc_data[num].x,npc_data[num].y,1,npc_ai[num].hit_angle);
			
		}
		else
		{
			change_npc_action("dead", num);
			npc_data[num].dead=1;
			enemy_ai[num].animation=0;
			play_fiend_sound(char_info[type].sound_death,npc_data[num].x,npc_data[num].y, 1,0,180);
		}
	}

	
	if(npc_data[num].dead && npc_ai[num].hit_speed==0 && !npc_ai[num].dead_and_still)
	{
		npc_ai[num].dead_and_still=1;

		compute_towerxy(npc_data[num].angle,char_info[type].blood_x,char_info[type].blood_y,&temp_x, &temp_y);
		
		make_bloodpool(npc_data[num].x+temp_x, npc_data[num].y+temp_y, (int)((float)char_info[type].w *0.4),makecol(200,0,0));
	}	
		
	////// Panic stuff/////	
	if(!npc_ai[num].animation)
	{
		if((gun_fired && npc_ai[num].panic))
		{

			//if(distance(player.x, player.y, npc_data[num].x, npc_data[num].y)<HEAR_RANGE)
			//	npc_ai[num].panic=RANDOM(300,400);
		}
		if(npc_damaged)
		{
			///if(object_is_in_fov(npc_data[num].x, npc_data[num].y,npc_data[num].angle, npc_data[npc_damaged].x, npc_data[npc_damaged].y,char_info[type].w,char_info[type].h,360,5))
			//	npc_ai[num].panic=RANDOM(450,600);
		}
	
		/*for(j=0;j<current_map_enemy_num;j++)
		{
			i = current_map_enemy[j];
			if(!enemy_data[i].dead && enemy_data[i].active && enemy_data[i].used)
				if(distance(enemy_data[i].x, enemy_data[i].y,npc_data[num].x, npc_data[num].y)<300 ) 
					if(object_is_in_fov(npc_data[num].x, npc_data[num].y, npc_data[num].angle, enemy_data[i].x, enemy_data[i].y, enemy_info[enemy_data[i].type].w, enemy_info[enemy_data[i].type].h,360,5)) 
					{
						npc_ai[num].panic=RANDOM(450,600);
						break;
					}
							
		}*/


		if(npc_ai[num].panic)npc_ai[num].panic--;

		if(!npc_ai[num].panic)
		{
			npc_ai[num].panic_x=-1;
			npc_ai[num].panic_y=-1;
		}

	
		if(npc_ai[num].panic && npc_ai[num].panic_x==-1)
		{
		
			do
			{
				npc_ai[num].panic_x = RANDOM(0,map->w*TILE_SIZE);
				npc_ai[num].panic_y = RANDOM(0,map->h*TILE_SIZE);

			}
			while(check_npc_collision(npc_ai[num].panic_x, npc_ai[num].panic_y, num));
		}
	}

}


/////////////////////////////////////////////////////////
//////////// MAIN //////////////////////////////////////
/////////////////////////////////////////////////////////

//Main update function....
void update_npc(void)
{
	int i,j;


	for(j=0;j<current_map_npc_num;j++)
	{
		i = current_map_npc[j];
		if(npc_data[i].used && npc_data[i].active)
		{
			if(!npc_ai[i].animation)
			{
			
				update_npc_path(i);
				update_npc_move(i);

				update_npc_dialog(i);
			}
			update_npc_data(i);
			

			update_npc_animation(i);
			
		}
	}
}


