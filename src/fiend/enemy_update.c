////////////////////////////////////////////////////
// This file contains enemy AI, anmimation and movement 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"


ENEMY_AI_DATA enemy_ai[MAX_ENEMY_DATA];



#define  DAMAGE_CHECK 160 //time when to check damage taken and see if the enemy shall run..
#define  HAVE_SEEN_LENGTH 2000 //time that the enemy searches for the player after haven seen him

/////////////////////////////////////////////////////////
//////////// MISC /////////////////////////////
/////////////////////////////////////////////////////////

//set all values in enemy_ai to default
void total_reset_enemy_ai(void)
{
	int i;

	for(i=0;i<MAX_ENEMY_DATA;i++)
	{
		enemy_ai[i].current_point=0;
		
		enemy_ai[i].next_point=1;
		enemy_ai[i].last_point=-1;
		
		enemy_ai[i].wanted_angle=0;
		enemy_ai[i].dead_and_still=0;

		enemy_ai[i].walking_random=0;

		enemy_ai[i].attacking=0;
		enemy_ai[i].running=0;
		enemy_ai[i].speaking=0;

		enemy_ai[i].last_dx=0;
		enemy_ai[i].last_dy=0;

		enemy_ai[i].found_player=0;

		enemy_ai[i].damage_taken=0;
		enemy_ai[i].damage_count=RANDOM(0,DAMAGE_CHECK);

		enemy_ai[i].have_seen_player=0;


		enemy_ai[i].animation=0;

	}
}

//set some values in enemy_ai to default
void reset_enemy_ai(void)
{
	int i;

	for(i=0;i<MAX_ENEMY_DATA;i++)
	{
		enemy_ai[i].wanted_angle=0;
		
		enemy_ai[i].walking_random=0;

		enemy_ai[i].attacking=0;
		enemy_ai[i].running=0;
		enemy_ai[i].speaking=0;

		enemy_ai[i].last_dx=0;
		enemy_ai[i].last_dy=0;

		enemy_ai[i].found_player=0;

		enemy_ai[i].damage_taken=0;
		enemy_ai[i].damage_count=RANDOM(0,DAMAGE_CHECK);

		enemy_ai[i].have_seen_player=0;

	}
}

/////////////////////////////////////////////////////////
//////////// MOVING /////////////////////////////
/////////////////////////////////////////////////////////

//change the action
void change_enemy_action(char *name, int num)
{
	int type = enemy_data[num].type;

	if(enemy_data[num].action!=enemy_action(&enemy_info[type],name))
	{
		enemy_data[num].action = enemy_action(&enemy_info[type],name);
		enemy_data[num].frame =0;
		enemy_data[num].nextframe =0;
	}
}


//Check for collsion
int check_enemy_collision(float x,float y, int num, int borders)
{
	int i,j;
	int type = enemy_data[num].type;
	float x1,x2;
	float y1,y2;

	for(i=0;i<map->num_of_objects;i++)
	{
		if(map->object[i].active)
			if(object_info[map->object[i].type].solid>0 && object_info[map->object[i].type].animation[map->object[i].action].solid>0)
				if(check_angle_collision(x, y, enemy_info[type].w, enemy_info[type].h, 0, map->object[i].x, map->object[i].y, object_info[map->object[i].type].w,object_info[map->object[i].type].h,  map->object[i].angle))
					return 1;
	}
	
	for(j=0;j<current_map_enemy_num;j++)
	{
		i = current_map_enemy[j];
		if(!enemy_data[i].dead && enemy_data[i].active && enemy_data[i].used && i!=num && !enemy_info[enemy_data[i].type].under_player) 
			if(check_collision(x-enemy_info[type].w/2, y-enemy_info[type].h/2, enemy_info[type].w, enemy_info[type].h, enemy_data[i].x-enemy_info[enemy_data[i].type].w/2, enemy_data[i].y-enemy_info[enemy_data[i].type].h/2,enemy_info[enemy_data[i].type].w,enemy_info[enemy_data[i].type].h)) 
					return 1;
	}

	for(j=0;j<current_map_npc_num;j++)
	{
		i = current_map_npc[j];
		if(npc_data[i].active && npc_data[i].used && !npc_data[i].dead)
			if(check_collision(x-enemy_info[type].w/2, y-enemy_info[type].h/2, enemy_info[type].w, enemy_info[type].h, npc_data[i].x-char_info[npc_data[i].type].w/2, npc_data[i].y-char_info[npc_data[i].type].h/2,char_info[npc_data[i].type].w,char_info[npc_data[i].type].h)) 
					return 1;
	}

	if(!player.dead && check_collision(x-enemy_info[type].w/2,y-enemy_info[type].h/2,enemy_info[type].w,enemy_info[type].h,
		               player.x-char_info[0].w/2,player.y-char_info[0].h/2,char_info[0].w,char_info[0].h))return 1;

	if(check_tile_collision(x-enemy_info[type].w/2,y-enemy_info[type].h/2,enemy_info[type].w,enemy_info[type].h))return 1;

	
	if(borders)
	{
		x1 = x-enemy_info[type].w/2;
		x2 = x+enemy_info[type].w/2;
		y1 = y-enemy_info[type].h/2;
		y2 = y+enemy_info[type].h/2;

		if(x1<0 || y1<0 || x2>map->w*32 || y2>map->h*32)
			return 1;
	}

	return 0;
}



///The move update
void update_enemy_move(int num)
{
	int type = enemy_data[num].type;
	float temp_x, temp_y;
	float speed;
	

	enemy_ai[num].last_dx=0;
	enemy_ai[num].last_dy=0;
	
	
	if( ( (enemy_ai[num].walking_random==1) || (enemy_data[num].current_mission==MISSION_PATROL && enemy_data[num].current_movement_path>-1) || 
		enemy_ai[num].running || enemy_ai[num].found_player) && !enemy_data[num].dead  && !enemy_ai[num].attacking)
	{
		
		type = enemy_data[num].type;
		
		speed = enemy_info[type].speed;

		
		if(enemy_ai[num].found_player || enemy_ai[num].running)
		{
			speed*= enemy_info[type].run_add;
			change_enemy_action("running", num );
		}
		else
		{
			change_enemy_action("moving", num );
		}
				
		if((int)enemy_data[num].angle != (int)enemy_ai[num].wanted_angle)
		{
			/*if(compute_angle_distance(enemy_ai[num].wanted_angle, enemy_data[num].angle)<enemy_info[type].speed*4)
			{
				enemy_data[num].angle = enemy_ai[num].wanted_angle; 
			}
			else
			{
				dist = compute_angle_distance(enemy_ai[num].wanted_angle, enemy_data[num].angle);
		
				if(  (add_angle(enemy_data[num].angle, dist)<enemy_ai[num].wanted_angle+2) && (add_angle(enemy_data[num].angle, dist)>enemy_ai[num].wanted_angle-2) ) 
					enemy_data[num].angle= add_angle(enemy_data[num].angle, enemy_info[type].speed*4);
				else
					enemy_data[num].angle= add_angle(enemy_data[num].angle, -enemy_info[type].speed*4);
			}*/
			enemy_data[num].angle = enemy_ai[num].wanted_angle;
		}
		
		if((int)enemy_ai[num].wanted_angle==(int)enemy_data[num].angle)
		{
			xyplus(speed, enemy_data[num].angle, &temp_x, &temp_y);
		
			if(!check_enemy_collision(enemy_data[num].x+temp_x, enemy_data[num].y, num,enemy_ai[num].walking_random))
			{
				enemy_data[num].x +=temp_x;
				enemy_ai[num].last_dx=temp_x;
			}
			else
			{
				if(enemy_ai[num].running)enemy_ai[num].running++;
				if(enemy_ai[num].walking_random)enemy_ai[num].random_count=0;
			}
		
			if(!check_enemy_collision(enemy_data[num].x, enemy_data[num].y+temp_y, num,enemy_ai[num].walking_random))
			{
				enemy_data[num].y +=temp_y;
				enemy_ai[num].last_dy=temp_y;
			}
			else
			{
				if(enemy_ai[num].running)enemy_ai[num].running++;
				if(enemy_ai[num].walking_random)enemy_ai[num].random_count=0;
			}

		}
		
	
	}
	else if(!enemy_data[num].dead && !enemy_ai[num].attacking)
	{
		change_enemy_action("still", num);
		
	}






}

//////////////////////////////////////////////////////
/////////// CHOSING THE ANGLE //////////////////////////////
/////////////////////////////////////////////////////


void update_enemy_angle(int num)
{
	int current_path;
	int goal_x, goal_y;
	int num_of_points;
	int temp;
	int type = enemy_data[num].type;

	
	//--UPDATE RANDOM
	if(!enemy_ai[num].running && !enemy_ai[num].attacking && !(enemy_data[num].current_mission==MISSION_PATROL && enemy_data[num].current_movement_path<0 && !enemy_ai[num].found_player) )
	{
		if( (enemy_data[num].current_mission==MISSION_PLAYER || enemy_data[num].current_mission==MISSION_NPC) && !enemy_ai[num].found_player && !enemy_ai[num].running) 
		{
			if(RANDOM(0,enemy_info[type].move_random)==1 )
			{
				enemy_ai[num].walking_random=1;
				if(enemy_ai[num].last_dx==0 || enemy_ai[num].last_dy==0)
				{		
					enemy_ai[num].wanted_angle = add_angle(enemy_ai[num].wanted_angle,RANDOM(-110, -250));
				}
				else
				{
					enemy_ai[num].wanted_angle = RANDOM(0,360);
				}

				enemy_ai[num].random_count = RANDOM(30,140);

			}
		}
		else if(RANDOM(0,enemy_info[type].move_random)==1)
		{
			enemy_ai[num].walking_random=1;
			enemy_ai[num].wanted_angle = add_angle(enemy_ai[num].wanted_angle,RANDOM(-enemy_info[type].move_random_length,enemy_info[type].move_random_length));
			enemy_ai[num].random_count = RANDOM(30,enemy_info[type].move_random_time);
		}
	}
	
	///
	if(enemy_ai[num].walking_random)
	{
		enemy_ai[num].random_count--;
		if(enemy_ai[num].random_count<1)
			enemy_ai[num].walking_random=0;

	}	
	//--UPDATE PATH
	else if(enemy_data[num].current_mission==MISSION_PATROL && enemy_data[num].current_movement_path>-1 && !enemy_ai[num].attacking && !enemy_ai[num].running && !enemy_ai[num].found_player)
	{

		current_path = enemy_data[num].current_movement_path;

		//Get the pos we want to go to..
		goal_x = enemy_data[num].movement_path[current_path].point[enemy_ai[num].current_point].x;
		goal_y = enemy_data[num].movement_path[current_path].point[enemy_ai[num].current_point].y;

		//the number of points...
		num_of_points = enemy_data[num].movement_path[current_path].num_of_points;


		//Check if you collide with the point,if yye do some stuff 
		if(check_collision(enemy_data[num].x-2,enemy_data[num].y-2,4,4,goal_x -1, goal_y-1,2,2))
		{
			//If this the first or the end point..
			if(num_of_points-1 ==  enemy_ai[num].current_point || enemy_ai[num].current_point==0)
			{
				//if we walk back and forward
				if(enemy_data[num].movement_path[current_path].type==1)
				{
				
					if(enemy_ai[num].current_point!=0)
					{
						enemy_ai[num].last_point = enemy_ai[num].current_point;
						enemy_ai[num].current_point = enemy_ai[num].current_point-1;
					}
					else
					{
						if(enemy_ai[num].last_point==-1)
						{
							temp = enemy_ai[num].last_point;
							enemy_ai[num].last_point=enemy_ai[num].current_point;
							enemy_ai[num].current_point++;
						}
						else
						{	
							enemy_ai[num].last_point=0;
							enemy_ai[num].current_point=1;
						}
					}


					enemy_ai[num].wanted_angle = get_best_enemy_angle(enemy_data[num].x,enemy_data[num].y, goal_x, goal_y, num,0);
				}
				//If we walk and stop at end
				else if(enemy_data[num].movement_path[current_path].type==2)
				{
					if(enemy_ai[num].current_point!=0)
					enemy_data[num].current_movement_path=-1;
					else
					{
						temp = enemy_ai[num].last_point;
						enemy_ai[num].last_point=enemy_ai[num].current_point;
						enemy_ai[num].current_point++;
					}
				}
				//If we walk and and then go to start at end
				else if(enemy_data[num].movement_path[current_path].type==3)
				{
					if(enemy_ai[num].current_point!=0)
					{
						enemy_ai[num].current_point = 0;
						enemy_ai[num].last_point = -1;
					}
					else
					{
						enemy_ai[num].last_point=0;
						enemy_ai[num].current_point=1;
					}

					enemy_ai[num].wanted_angle = get_best_enemy_angle(enemy_data[num].x,enemy_data[num].y, goal_x, goal_y,num,0);
				}

			}
			else //if it is just a normal point we collided with...
			{
				temp = enemy_ai[num].last_point;
				enemy_ai[num].last_point=enemy_ai[num].current_point;
			
				if(temp<enemy_ai[num].current_point)enemy_ai[num].current_point++;
				else enemy_ai[num].current_point--;

				enemy_ai[num].wanted_angle = get_best_enemy_angle(enemy_data[num].x,enemy_data[num].y, goal_x, goal_y,num,0);
			}
		}
		else //if we did not collide with a point
		{
			enemy_ai[num].wanted_angle = get_best_enemy_angle(enemy_data[num].x,enemy_data[num].y, goal_x, goal_y, num,0);
		}


	}//--UPDATE FOUND PLAYER
	else if(enemy_ai[num].found_player && !enemy_ai[num].attacking && !enemy_ai[num].running)
	{
			enemy_ai[num].wanted_angle = get_best_enemy_angle(enemy_data[num].x,enemy_data[num].y, enemy_ai[num].last_player_x, enemy_ai[num].last_player_y, num,0);

	}
	else if(enemy_ai[num].running)
	{
		//if(!find_best_xy(enemy_data[num].x,enemy_data[num].y,enemy_ai[num].run_x, enemy_ai[num].run_y, enemy_info[type].w,enemy_info[type].h, &x, &y,1))
		//	enemy_ai[num].running=0;
				
		enemy_ai[num].wanted_angle = get_best_enemy_angle(enemy_data[num].x,enemy_data[num].y, enemy_ai[num].run_x, enemy_ai[num].run_y, num,1);
		
		if(check_collision(enemy_data[num].x-2,enemy_data[num].y-2,4,4, enemy_ai[num].run_x-2,enemy_ai[num].run_y-2,4,4))
			enemy_ai[num].running=0;

		if(enemy_ai[num].running>50)
			enemy_ai[num].running=0;
	}
	

}




/////////////////////////////////////////////////////////
//////////// ANIMATION /////////////////////////////
/////////////////////////////////////////////////////////

//Update the animations for the enemys
void update_enemy_animation(int num)
{
	int type;

	int tile_set, tile_num;;
	
	type = enemy_data[num].type;
	
	enemy_data[num].nextframe++;
	if(enemy_data[num].nextframe>=ANIM_SPEED)
	{
		//update frame	
		enemy_data[num].nextframe =0;
		enemy_data[num].frame++;
		if(enemy_info[type].animation[enemy_data[num].action].frame[enemy_data[num].frame] ==255)
		{
			enemy_data[num].frame = enemy_info[type].animation[enemy_data[num].action].frame[enemy_data[num].frame + 1 ];
			if(enemy_ai[num].animation!=0){
				if(enemy_ai[num].animation>-1)enemy_ai[num].animation--;
				if(enemy_ai[num].animation==0)change_enemy_action("still", num);}
		}
		

		//footstep sound
		tile_set = (map->layer1+ ((int)enemy_data[num].x/TILE_SIZE) +( ((int)enemy_data[num].y/TILE_SIZE) * map->w))->tile_set;
		tile_num = (map->layer1+ ((int)enemy_data[num].x/TILE_SIZE) +( ((int)enemy_data[num].y/TILE_SIZE) * map->w))->tile_num;
		
		if(strcmp(enemy_info[type].animation[enemy_data[num].action].name, "moving")==0 && (enemy_data[num].frame==enemy_info[type].walk_step1 || enemy_data[num].frame==enemy_info[type].walk_step2))
		{
			if(strcmp(enemy_info[type].step_sound,"normal")==0)
				play_fiend_sound(tile_info[tile_set].tile[tile_num].sound,enemy_data[num].x,enemy_data[num].y,1,0,180);
			else
				play_fiend_sound(enemy_info[type].step_sound,enemy_data[num].x,enemy_data[num].y,1,0,180);
		}
		else if(strcmp(enemy_info[type].animation[enemy_data[num].action].name, "running")==0 && (enemy_data[num].frame==enemy_info[type].run_step1 || enemy_data[num].frame==enemy_info[type].run_step2))
		{
			if(strcmp(enemy_info[type].step_sound,"normal")==0)
				play_fiend_sound(tile_info[tile_set].tile[tile_num].sound,enemy_data[num].x,enemy_data[num].y,1,0,180);
			else
				play_fiend_sound(enemy_info[type].step_sound,enemy_data[num].x,enemy_data[num].y,1,0,180);
		}

	}
}


/////////////////////////////////////////////////////////
//////////// DATA UPDATE /////////////////////////////
/////////////////////////////////////////////////////////


void reset_enemy_data(void)
{
	int i;

	for(i=0;i<MAX_ENEMY_DATA;i++)
	{
		enemy_ai[i].hit_speed=0;
		enemy_ai[i].hit_angle=0;
	}
}


void update_enemy_data(int num)
{
	int type = enemy_data[num].type;
	int w_type;
	int fov,i,j;
	float angle;
	float temp_x, temp_y;
	int x,y;
	int temp;
	

		/*textprintf(screen,font,0,0,makecol(255,255,255),"r: %d   ",enemy_ai[num].running);
		textprintf(screen,font,0,10,makecol(255,255,255),"a: %d   ",enemy_ai[num].attacking);
		textprintf(screen,font,0,20,makecol(255,255,255),"fp: %d   ",enemy_ai[num].found_player);
		textprintf(screen,font,0,30,makecol(255,255,255),"hs: %d   ",enemy_ai[num].have_seen_player);
		textprintf(screen,font,0,40,makecol(255,255,255),"dc: %d   ",enemy_ai[num].damage_count);
		textprintf(screen,font,0,50,makecol(255,255,255),"dt: %d   ",enemy_ai[num].damage_taken);*/
	
	if(!enemy_ai[num].animation && !enemy_ai[num].running && !enemy_info[type].friendly)
	{
		////// ---The Attacking---- ////
		if(enemy_ai[num].found_player && enemy_ai[num].hit_speed==0 && !enemy_ai[num].attacking && !enemy_data[num].dead && !player.dead)
		if(path_is_clear(enemy_data[num].x, enemy_data[num].y,0,player.x, player.y,2,0) )
			for(i=0;i<3;i++)
			{
				if(strcmp(enemy_info[type].weapon_name[i],"null")!=0)
					if(distance(enemy_data[num].x, enemy_data[num].y,player.x, player.y)<=enemy_info[type].weapon_range[i] && RANDOM(0,enemy_info[type].weapon_random[i])==0)
					{
						enemy_ai[num].attacking= enemy_info[type].weapon_length[i];
						enemy_ai[num].attack_num = i;
						enemy_ai[num].can_attack=0;
						enemy_ai[num].weapon_was_shot=0;
					
						enemy_data[num].angle = compute_angle(player.x, player.y,enemy_data[num].x, enemy_data[num].y);

					
						w_type = get_weapon_num(enemy_info[type].weapon_name[i]);

						change_enemy_action(weapon_info[w_type].player_action, num);
					}
			}
	
		if(enemy_ai[num].attacking>0 && !enemy_data[num].dead)
		{
			w_type = get_weapon_num(enemy_info[type].weapon_name[enemy_ai[num].attack_num]);

			
			if(enemy_ai[num].can_attack>=weapon_info[w_type].shot_length )
			{
		
				//Show a lightmap and flame
				compute_towerxy(enemy_data[num].angle,weapon_info[w_type].flame_x,weapon_info[w_type].flame_y,&temp_x, &temp_y);

				make_normal_light(enemy_data[num].x + temp_x, enemy_data[num].y + temp_y, weapon_info[w_type].light_c, weapon_info[w_type].light_r, weapon_info[w_type].flame_length);
					
				make_flame(weapon_info[w_type].flame_num,enemy_data[num].x+temp_x, enemy_data[num].y+temp_y,weapon_info[w_type].flame_length);					
					
				//Play the weapon sound
				play_fiend_sound(weapon_info[w_type].sound,enemy_data[num].x,enemy_data[num].y,1,0,230);

				//Make a missile
				for(i=0;i<weapon_info[w_type].num_of_missiles;i++)
				{
					//angle = ((float)i/weapon_info[w_type].num_of_missiles)*(float)weapon_info[w_type].missile_angle;
					
					//angle = (weapon_info[w_type].missile_angle/2)-angle;

					angle = RANDOM(-weapon_info[w_type].missile_angle, weapon_info[w_type].missile_angle);

					compute_towerxy( enemy_data[num].angle ,weapon_info[w_type].missile_x,weapon_info[w_type].missile_y,&temp_x, &temp_y);
					make_new_missile(enemy_data[num].x+temp_x, enemy_data[num].y+temp_y, add_angle(enemy_data[num].angle,angle), w_type);
				}

				enemy_ai[num].can_attack=0;

				enemy_ai[num].weapon_was_shot=1;
				enemy_ai[num].release_shell = weapon_info[w_type].shell_after_shot_length;
			}


			if(enemy_ai[num].weapon_was_shot && enemy_ai[num].release_shell<1)
			{	
				//shoot a shell
				compute_towerxy(enemy_data[num].angle,weapon_info[w_type].shell_x,weapon_info[w_type].shell_y,&temp_x, &temp_y);

				make_shell(weapon_info[w_type].shell_name,enemy_data[num].x+temp_x, enemy_data[num].y+temp_y, (float)(RANDOM(8,12))/10, add_angle(enemy_data[num].angle, 90), 1,(float)(RANDOM(16,19))/10,(float)(RANDOM(14,20))/10,190);
				enemy_ai[num].weapon_was_shot=0;		
			}
			else
			{
				enemy_ai[num].release_shell--;
			}

			enemy_ai[num].can_attack++;
			enemy_ai[num].attacking--;

			if(enemy_ai[num].attacking<1 && enemy_info[type].attack_from_behind )
				if(RANDOM(1,enemy_info[type].attack_from_behind)==1)
				{
					temp_x = -100;
				
					for(i=0;i<map->num_of_path_nodes;i++)
					{
						if(distance(map->path_node[i].x,map->path_node[i].y, player.x,player.y)<200)
							if(object_is_in_fov(map->path_node[i].x, map->path_node[i].y, enemy_data[num].angle, player.x, player.y, 1,1, 360,1) )
								if(RANDOM(0,1)==1)
									if(find_best_xy(enemy_data[num].x,enemy_data[num].y,map->path_node[i].x, map->path_node[i].y, enemy_info[type].w,enemy_info[type].h, &x, &y,1))
									{
										temp_x = map->path_node[i].x;
										temp_y = map->path_node[i].y;
										break;
									}
					}

							

					if(temp_x!=-100)
					{
						enemy_ai[num].run_x = temp_x;
						enemy_ai[num].run_y = temp_y;

						enemy_ai[num].running = 1;

						enemy_ai[num].attacking=0;
						//enemy_ai[num].found_player=0;
						enemy_ai[num].walking_random=0;
				
					}
					
				}

		
		}
	}

	////// ---Death stuff---- ////
	if(enemy_data[num].energy<0 && !enemy_data[num].dead)
	{
		/*if( enemy_data[num].energy<-enemy_info[type].energy*4 )
		{
			enemy_ai[num].dead_and_still=1;
			enemy_data[num].dead=1;
			enemy_data[num].active=0;
			enemy_ai[num].animation=0;
			play_fiend_sound("splash",enemy_data[num].x,enemy_data[num].y, 1,0,180);
			make_flesh_explosion(enemy_data[num].x,enemy_data[num].y,1, enemy_ai[num].hit_angle);
		}
		else
		{*/
			change_enemy_action("dead", num);
			enemy_data[num].dead=1;
			enemy_ai[num].animation=0;
						
			play_fiend_sound(enemy_info[type].sound_death,enemy_data[num].x,enemy_data[num].y, 1,0,180);
		//}
	}

	
	if(enemy_data[num].dead && enemy_ai[num].hit_speed==0 && !enemy_ai[num].dead_and_still)
	{
		enemy_ai[num].dead_and_still=1;
		
		compute_towerxy(enemy_data[num].angle,enemy_info[type].blood_x,enemy_info[type].blood_y,&temp_x, &temp_y);
			
		if(enemy_info[type].show_bloodpool)
			make_bloodpool(enemy_data[num].x+temp_x, enemy_data[num].y+temp_y, (int)((float)enemy_info[type].w *0.4),enemy_info[type].blood_color);
	}
	

	////// ---Check if the Enemy can Find the Player---- /////
		
	
	if(!enemy_ai[num].animation && !enemy_ai[num].running && !enemy_info[type].friendly)
	{
		if(player.dead)
		{
			enemy_ai[num].found_player =0;
		}
	
		//if enemy has not seen the player for a while he can't find him...
		if(enemy_ai[num].found_player)
			enemy_ai[num].found_player--;

	
		//if the enemy is near an enemy that has found the player ans sees him he also finds the player...
		if(!enemy_ai[num].found_player && !player.dead)
			for(j=0;j<current_map_enemy_num;j++)
			{	
				i = current_map_enemy[j];
				if(enemy_data[i].used && enemy_data[i].active && enemy_ai[i].found_player)
				if(object_is_in_fov(enemy_data[i].x, enemy_data[i].y, enemy_data[num].angle, player.x, player.y, char_info[0].w, char_info[0].h, 360,1))
				{
				
					//the enemy is the same sort and you wotk in team then get better info
					if(type = enemy_data[i].type && enemy_info[type].attack_in_group)
					{
						if(distance(enemy_data[i].x,enemy_data[i].y,enemy_data[num].x,enemy_data[num].y)<enemy_info[type].hear_range*0.75)
						{
							enemy_ai[num].found_player=FOUND_LENGTH;
							enemy_ai[num].last_player_x = player.x;
							enemy_ai[num].last_player_y = player.y;
						}
					}
					else
					{
						if(distance(enemy_data[i].x,enemy_data[i].y,enemy_data[num].x,enemy_data[num].y)<100)
						{
							enemy_ai[num].found_player=FOUND_LENGTH;
							enemy_ai[num].last_player_x = enemy_data[i].x;
							enemy_ai[num].last_player_y = enemy_data[i].y;
						}
					}
				}
			}

	
		//check if the enemy can see the player
		//if he has found the player he gets a large fov
		if(enemy_ai[num].have_seen_player)
			fov=360;
		else
			fov = enemy_info[type].fov;

		if(!player.dead && object_is_in_fov(enemy_data[num].x, enemy_data[num].y, enemy_data[num].angle, player.x, player.y, char_info[0].w, char_info[0].h, fov,5))
		{
			// if enemy has motionon based seeing check if player has moved...
			if( (enemy_info[type].eye_type==1 && (player.last_dx!=0 || player.last_dy!=0) ) || enemy_info[type].eye_type==0)
			{
				enemy_ai[num].found_player=FOUND_LENGTH;
				enemy_ai[num].last_player_x = player.x;
				enemy_ai[num].last_player_y = player.y;
			}
		}

		//if the enemy can smell he always knows where the player is...
		if(!player.dead && enemy_info[type].can_smell && enemy_ai[num].found_player)
		{
			enemy_ai[num].last_player_x = player.x;
			enemy_ai[num].last_player_y = player.y;
		}


		//check if the enemy can hear the player
		if(gun_fired && !player.dead)//Gun shot
			if(enemy_info[type].can_hear && distance(player.x, player.y, enemy_data[num].x, enemy_data[num].y)<enemy_info[type].hear_range)
			{
				enemy_ai[num].found_player=FOUND_LENGTH;
				enemy_ai[num].last_player_x = player.x;
				enemy_ai[num].last_player_y = player.y;
			}
		/*if(!player.dead && player.running)//foot steps from a running player..
			if(enemy_info[type].can_hear && distance(player.x, player.y, enemy_data[num].x, enemy_data[num].y)<enemy_info[type].hear_range/2)
			{
				enemy_ai[num].found_player=FOUND_LENGTH;
				enemy_ai[num].last_player_x = player.x;
				enemy_ai[num].last_player_y = player.y;
			}*/

	}


	////// ---Check if the Enemy will Run---- /////
	
	if(!enemy_ai[num].animation && enemy_info[type].run_when_hurt && !enemy_ai[num].running && !enemy_data[num].dead)
	{
		temp = ((float)enemy_info[type].run_when_hurt/100)*(float)enemy_info[type].energy;
		
		if((enemy_ai[num].damage_count>DAMAGE_CHECK || enemy_ai[num].damage_taken>temp) && !enemy_ai[num].attacking)
		{
			if(enemy_ai[num].damage_taken>temp)
			{				
					
				temp_x = -100;
				
				for(i=0;i<map->num_of_path_nodes;i++)
				{
					if(distance(map->path_node[i].x,map->path_node[i].y, enemy_data[num].x,enemy_data[num].y)<400)
						if(!object_is_in_fov(map->path_node[i].x, map->path_node[i].y, enemy_data[num].angle, player.x, player.y, char_info[0].w, char_info[0].h, 360,1) )
							if(find_best_xy(enemy_data[num].x,enemy_data[num].y,map->path_node[i].x, map->path_node[i].y, enemy_info[type].w,enemy_info[type].h, &x, &y,1))
							{
								temp_x = map->path_node[i].x;
								temp_y = map->path_node[i].y;
								break;
							}
				}

							

				if(temp_x!=-100)
				{
					enemy_ai[num].run_x = temp_x;
					enemy_ai[num].run_y = temp_y;

					enemy_ai[num].running = 1;

					enemy_ai[num].attacking=0;
					enemy_ai[num].found_player=0;
					enemy_ai[num].walking_random=0;

					
				}

				
			}
			
			enemy_ai[num].damage_taken=0;
			enemy_ai[num].damage_count=0;
			
		}
		enemy_ai[num].damage_count++;
	}

	////// ---Check if the Enemy will Regenerate---- /////
	if(enemy_info[type].regenerate>0 &&  !enemy_data[num].dead)
	{
		enemy_data[num].energy+=enemy_info[type].regenerate;
		if(enemy_data[num].energy > enemy_info[type].energy)
			enemy_data[num].energy = enemy_info[type].energy;
	}

	////// ---Check if the Enemy will Speak---- /////
	
	if(!enemy_ai[num].animation && enemy_ai[num].speaking<1 && !enemy_data[num].dead && !enemy_ai[num].running)
	{
		if(!enemy_ai[num].attacking && !enemy_ai[num].found_player && !enemy_ai[num].running)
		{
			for(i=0;i<3;i++)
				if(RANDOM(0,enemy_info[type].sound_ambient_random[i])==1)
				{
					play_fiend_sound(enemy_info[type].sound_ambient[i],enemy_data[num].x,enemy_data[num].y, 1,0,180);
					enemy_ai[num].speaking = SPEAK_LENGTH;
				}
		}

		if(enemy_ai[num].found_player)
		{
			for(i=3;i<5;i++)
				if(strcmp(enemy_info[type].sound_ambient[i],"none")!=0 && RANDOM(0,enemy_info[type].sound_ambient_random[i])==0)
				{
					play_fiend_sound(enemy_info[type].sound_ambient[i],enemy_data[num].x,enemy_data[num].y, 1,0,180);
					enemy_ai[num].speaking = SPEAK_LENGTH;
				}
		}


	}
	else
	{
		enemy_ai[num].speaking--;
	}


	if(enemy_ai[num].found_player)
		enemy_ai[num].have_seen_player=HAVE_SEEN_LENGTH;
	
	if(!enemy_ai[num].found_player && enemy_ai[num].have_seen_player>0)
		enemy_ai[num].have_seen_player--;


}


/////////////////////////////////////////////////////////
//////////// MAIN //////////////////////////////////////
/////////////////////////////////////////////////////////


//Main update function....
void update_enemy(void)
{
	int i,j;


	for(j=0;j<current_map_enemy_num;j++)
	{
		i = current_map_enemy[j];
		if(enemy_data[i].used && enemy_data[i].active)
		{
			if(!enemy_ai[i].animation)
			{
				update_enemy_angle(i);
				update_enemy_move(i);
			}

			update_enemy_data(i);
						
			update_enemy_animation(i);

		}	
	}
}

