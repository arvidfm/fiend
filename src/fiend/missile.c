////////////////////////////////////////////////////
// This file contains missle funcs
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <allegro.h>

#include <stdio.h>
#include <string.h>

#include "../fiend.h"
#include "../grafik4.h"
#include "../draw.h"


//Gloabal vars that can trigger events
int npc_damaged=0;


void reset_missiles(void)
{
	int i;

	for(i=0;i<MAX_MISSILES;i++)
		missile_data[i].used=0;

}



void make_new_missile(int x, int y, float angle, int type)
{
	int i;
	int data_num=-1;
	int p_type=-1;
	
	for(i=0;i<MAX_MISSILES;i++)
		if(!missile_data[i].used)
		{
			data_num = i;
			break;
		}

	if(data_num<0)return;

	
	for(i=0;i<num_of_particles;i++)
		if(strcmp(weapon_info[type].missile_name, particle_info[i].name)==0)
		{
			p_type = i;
			break;
		}

	missile_data[data_num].p_type=p_type;
	
	missile_data[data_num].used=1;
	
	missile_data[data_num].x=x;
	missile_data[data_num].y=y;
	missile_data[data_num].start_x=x;
	missile_data[data_num].start_y=y;
	
	missile_data[data_num].angle=angle;
	missile_data[data_num].type=type;

	missile_data[data_num].min_damage=weapon_info[type].min_damage;
	missile_data[data_num].max_damage=weapon_info[type].max_damage;
	missile_data[data_num].speed=weapon_info[type].speed;
	missile_data[data_num].frame=0;
	missile_data[data_num].next_frame=0;
	
	missile_data[data_num].dist=0;


	

}


int bullet_object_collision(float x, float y)
{
	int i;

	for(i=0;i<map->num_of_objects;i++)
	{
		if(map->object[i].active)
			if(object_info[map->object[i].type].solid>1 && object_info[map->object[i].type].animation[map->object[i].action].solid>0)
				if( check_angle_collision(x, y, 2,2, 0,
					map->object[i].x, map->object[i].y, object_info[map->object[i].type].w,object_info[map->object[i].type].h,  ((int)(map->object[i].angle*((float)object_info[map->object[i].type].angles/360) )) *  ((float)360/object_info[map->object[i].type].angles)   ) )
						return 1;

	}
	return 0;
}


int missile_hit(int i,int type)
{
	int j,k;
	
	if(check_tile_collision(missile_data[i].x, missile_data[i].y,2,2)>1 || bullet_object_collision(missile_data[i].x, missile_data[i].y) || !missile_data[i].used)
		return 1;
	
	if(!player.dead && check_collision(missile_data[i].x, missile_data[i].y,2,2, player.x-char_info[0].w/2, player.y-char_info[0].h/2,char_info[0].w,char_info[0].h)) 
		return 1;
	
	for(j=0;j<current_map_npc_num;j++)
	{
		k = current_map_npc[j];
		if(npc_data[k].active && npc_data[k].used && !npc_data[k].dead )
			if(check_collision(missile_data[i].x, missile_data[i].y,2,2, npc_data[k].x-char_info[npc_data[k].type].w/2, npc_data[k].y-char_info[npc_data[k].type].h/2,char_info[npc_data[k].type].w,char_info[npc_data[k].type].h)) 
				return 1;
	}


	for(j=0;j<current_map_enemy_num;j++)
	{
		k = current_map_enemy[j];
		if(enemy_data[k].active && enemy_data[k].used && !enemy_data[k].dead && weapon_info[type].hit_enemy)
			if(check_collision(missile_data[i].x, missile_data[i].y,2,2, enemy_data[k].x-enemy_info[enemy_data[k].type].w/2, enemy_data[k].y-enemy_info[enemy_data[k].type].h/2,enemy_info[enemy_data[k].type].w,enemy_info[enemy_data[k].type].h)) 
				return 1;
	}



	return 0;
}


void make_blood(int i, int type, int color)
{

	//make the blood
	if(weapon_info[type].strength<0)
	{
		make_bullet_hit_flesh(i,-1,color);
	}
	else if(weapon_info[type].strength<5)
	{
		make_bullet_hit_flesh(i,0,color);
	}
	else if(weapon_info[type].strength<20)
	{	
		make_bullet_hit_flesh(i,1,color);
	}
	else 
	{
		make_bullet_hit_flesh(i,2,color);
	}
}
	

void update_missiles(void)
{
	int i,j,k;
	float temp_x, temp_y;
	int type;
	int p_type;
	int e_type;
	int temp;
		
	npc_damaged=0;

	for(i=0;i<MAX_MISSILES;i++)
		if(missile_data[i].used)
		{
		
			/// Move the missile
			type = missile_data[i].type;

			xyplus(missile_data[i].speed, missile_data[i].angle, &temp_x, &temp_y);
		
			missile_data[i].x+=temp_x;
			missile_data[i].y+=temp_y;

			if(weapon_info[type].beam)
			{
				while(!missile_hit(i,type) && missile_data[i].dist<weapon_info[type].range )
				{
					missile_data[i].x+=temp_x;
					missile_data[i].y+=temp_y;
					missile_data[i].dist++;
				}
			
				if(weapon_info[type].beam>1)
					make_new_beam(weapon_info[type].missile_name,missile_data[i].start_x, missile_data[i].start_y,missile_data[i].x,missile_data[i].y,weapon_info[type].beam);			
			}

			missile_data[i].dist++;
			
			missile_data[i].speed-=weapon_info[type].range_dec;
			if(missile_data[i].speed<0)missile_data[i].speed=0;

			missile_data[i].min_damage-=weapon_info[type].damage_range_dec;
			if(missile_data[i].min_damage<0)missile_data[i].min_damage=0;
			
			missile_data[i].max_damage-=weapon_info[type].damage_range_dec;
			if(missile_data[i].max_damage<0)missile_data[i].max_damage=0;

			//ANIMATION
			
			if(missile_data[i].p_type>-1)
			{
				p_type = missile_data[i].p_type;

				missile_data[i].next_frame++;
				if(missile_data[i].next_frame>=ANIM_SPEED)
				{
					//update frame	
					missile_data[i].next_frame =0;
					missile_data[i].frame++;
				
					if(particle_info[p_type].anim_frame[missile_data[i].frame]==255)
						missile_data[i].frame = particle_info[p_type].anim_frame[missile_data[i].frame+1];
				}
			}
			
			//LIGHT
			if(weapon_info[type].missile_light_r>0)
				make_normal_light(missile_data[i].x,missile_data[i].y, weapon_info[type].missile_light_c, weapon_info[type].missile_light_r, 1);
			
			//PARTICLES
			for(j=0;j<weapon_info[type].particle_num;j++)
				make_new_particle(weapon_info[type].particle_name, missile_data[i].x+RANDOM(-6,6), missile_data[i].y+RANDOM(-6,6), 
							add_angle(missile_data[i].angle,180+(RANDOM(-30,30)) ),
							0,0, RANDOM(10,30),2,0);


			////COLLISON DETECTION///////

			//if it has traveld too long destroy
			if(missile_data[i].dist>weapon_info[type].range)
				missile_data[i].used=0;
			
			//for tiles and objects
			if(check_tile_collision(missile_data[i].x, missile_data[i].y,2,2)>1 || bullet_object_collision(missile_data[i].x, missile_data[i].y) || !missile_data[i].used)
			{
				
				if(strcmp(weapon_info[type].explosion_name,"normal")==0)
				{
					if(weapon_info[type].strength<0)
					{
						make_bullet_hit_object(i,-1);
					}
					else if(weapon_info[type].strength<5)
					{
						make_bullet_hit_object(i,0);
					}
					else if(weapon_info[type].strength<20)
					{
						make_bullet_hit_object(i,1);
					}
					else if(weapon_info[type].strength<60)
					{
						make_bullet_hit_object(i,2);
					}
				}
				else
				{
					for(k=0;k<weapon_info[type].num_of_explosions;k++)
						make_new_particle(weapon_info[type].explosion_name, missile_data[i].x +RANDOM(-10,10), missile_data[i].y+RANDOM(-10,10),0,0,0,70,3,0);
				}

				missile_data[i].used=0;
			}
			
			/////// CHECK PLAYER HIT //////////////////
			if(!player.dead && check_angle_collision(missile_data[i].x, missile_data[i].y,2,2,0, player.x, player.y,char_info[0].hit_w,char_info[0].hit_h,player.angle)) 
			{
				make_blood(i,type,makecol(200,0,0));
				
				player.energy-=RANDOM( (int)missile_data[i].min_damage,(int)missile_data[i].max_damage); 
					
				if(player.energy>=0)
				{
					//player.frame =0;
					//player.nextframe =0;
					//change_player_action("hit");
				}
								
				missile_data[i].used=0;

			}

			/////// CHECK NPC HIT //////////////////
			for(j=0;j<current_map_npc_num;j++)
			{
				k = current_map_npc[j];
				if(npc_data[k].active && npc_data[k].used && !npc_data[k].dead )
					if(check_angle_collision(missile_data[i].x, missile_data[i].y,2,2,0, npc_data[k].x, npc_data[k].y,char_info[npc_data[k].type].hit_w,char_info[npc_data[k].type].hit_h,npc_data[k].angle)) 
					{

						make_blood(i,type,makecol(200,0,0));
						
						//make em panic!
						//npc_ai[k].panic=500;
						
						//an npc is hit!
						npc_damaged=k;
						
						npc_data[k].energy-=RANDOM((int)missile_data[i].min_damage,(int)missile_data[i].max_damage);
						
						if(npc_data[k].energy>=0)
						{
							//npc_data[k].frame =0;
							//npc_data[k].next_frame =0;
							//change_npc_action("hit", k);
						}
								
						missile_data[i].used=0;

					}
			}

			
			
			//////// CHECK ENEMY HIT /////////////////////////
			for(j=0;j<current_map_enemy_num;j++)
			{
				k = current_map_enemy[j];
				if(enemy_data[k].active && enemy_data[k].used && !enemy_data[k].dead && weapon_info[type].hit_enemy)
					if(check_angle_collision(missile_data[i].x, missile_data[i].y,2,2,0, enemy_data[k].x, enemy_data[k].y,enemy_info[enemy_data[k].type].hit_w,enemy_info[enemy_data[k].type].hit_h,enemy_data[k].angle)) 
					{
						e_type = enemy_data[k].type;
						//make the blood
						if(weapon_info[type].strength<0)
						{
							make_bullet_hit_flesh(i,-1,enemy_info[e_type].blood_color);
						}
						else if(weapon_info[type].strength<5)
						{
							make_bullet_hit_flesh(i,0,enemy_info[e_type].blood_color);
						}
						else if(weapon_info[type].strength<20)
						{	
							make_bullet_hit_flesh(i,1,enemy_info[e_type].blood_color);
						}
						else //if(weapon_info[type].strength<60)
						{
							make_bullet_hit_flesh(i,2,enemy_info[e_type].blood_color);
						}

						temp = RANDOM((int)missile_data[i].min_damage,(int)missile_data[i].max_damage);
						enemy_data[k].energy-=temp;
						enemy_ai[k].damage_taken+=temp;

						//if the player is in fov, attack!!!
						if(object_is_in_fov(enemy_data[k].x, enemy_data[k].y, enemy_data[k].angle, player.x, player.y, char_info[0].w, char_info[0].h, 360,5))
						{
							enemy_ai[k].found_player=FOUND_LENGTH;
							enemy_ai[k].last_player_x = player.x;
							enemy_ai[k].last_player_y = player.y;
						}
						
						if(enemy_data[k].energy>=0)
						{
							//enemy_data[k].frame =0;
							//enemy_data[k].nextframe =0;
							//change_enemy_action("hit", k);
						}
								
						missile_data[i].used=0;

					}
			}



		}
}



void draw_missiles(void)
{
	int p_type;
	int i;

	for(i=0;i<MAX_MISSILES;i++)
		if(missile_data[i].used && missile_data[i].p_type>-1)
		{
			p_type = missile_data[i].p_type;

			
			if(object_is_in_player_los(missile_data[i].x,missile_data[i].y,get_bitmap_w(particle_info[p_type].pic[missile_data[i].frame].dat),get_bitmap_h(particle_info[p_type].pic[missile_data[i].frame].dat),0,0)) 
			{
				if(particle_info[p_type].aa)
				{
					//set_alpha_blender();
					draw_additive_sprite(virt,particle_info[p_type].pic[missile_data[i].frame].dat, 
						missile_data[i].x - get_bitmap_w(particle_info[p_type].pic[missile_data[i].frame].dat)/2-map_x,
						missile_data[i].y - get_bitmap_h(particle_info[p_type].pic[missile_data[i].frame].dat)/2-map_y);
				}
				else if(particle_info[p_type].trans)
				{	
					set_trans_blender(0,0,0,particle_info[p_type].trans_alpha);
					draw_trans_sprite(virt,particle_info[p_type].pic[missile_data[i].frame].dat, 
						missile_data[i].x - get_bitmap_w(particle_info[p_type].pic[missile_data[i].frame].dat)/2-map_x,
						missile_data[i].y - get_bitmap_h(particle_info[p_type].pic[missile_data[i].frame].dat)/2-map_y);
				}	
				else if(particle_info[p_type].rotate)
				{	
					rotate_sprite(virt,particle_info[p_type].pic[missile_data[i].frame].dat, 
						missile_data[i].x - get_bitmap_w(particle_info[p_type].pic[missile_data[i].frame].dat)/2-map_x,
						missile_data[i].y - get_bitmap_h(particle_info[p_type].pic[missile_data[i].frame].dat)/2-map_y,
						degree_to_fixed(missile_data[i].angle));
				}
				else 
				{
					draw_sprite(virt,particle_info[p_type].pic[missile_data[i].frame].dat, 
						missile_data[i].x - get_bitmap_w(particle_info[p_type].pic[missile_data[i].frame].dat)/2-map_x,
						missile_data[i].y - get_bitmap_h(particle_info[p_type].pic[missile_data[i].frame].dat)/2-map_y);
				}

			}


		}		



}