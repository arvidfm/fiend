////////////////////////////////////////////////////
// This file Contains all the lowlevel AI routince 
// like line of sight.
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"

//these are used so that a npc or enemy only checks for the best angle at certaian points.
//see length determins the length betwwen checks. 
#define SEE_LENGTH 30
static int enemy_see_count[MAX_ENEMY_DATA];
static int npc_see_count[MAX_ENEMY_DATA];


//finds the best angle for an enemy is it wants to get to the goal.
float get_best_enemy_angle(float start_x,float  start_y,float  goal_x,float  goal_y,int num,int check_player)
{
	int temp_x=goal_x, temp_y=goal_y;
	
	if(enemy_see_count[num]>SEE_LENGTH)
	{
		enemy_see_count[num]=0;
		find_best_xy(start_x,start_y,goal_x, goal_y, enemy_info[enemy_data[num].type].w,enemy_info[enemy_data[num].type].h, &temp_x, &temp_y,check_player);
		
		return compute_angle(temp_x, temp_y,start_x, start_y);
	}
	else
	{
		enemy_see_count[num]++;
		
		return enemy_data[num].angle;
	}
}	


//finds the best angle for an npcv if it wants to get to the goal.
float get_best_npc_angle(float start_x,float  start_y,float  goal_x,float  goal_y,int num)
{
	int temp_x=goal_x, temp_y=goal_y;
	
	if(npc_see_count[num]>SEE_LENGTH)
	{
		npc_see_count[num]=0;
		find_best_xy(start_x,start_y,goal_x, goal_y, char_info[npc_data[num].type].w,char_info[npc_data[num].type].h, &temp_x, &temp_y,0);
		
		return compute_angle(temp_x, temp_y,start_x, start_y);
	}
	else
	{
		npc_see_count[num]++;
		
		return npc_data[num].angle;
	}
}





////////////////////////////////////////
///// LINE OF SIGHT STUFF /////////////
///////////////////////////////////////



//check if obejct with eye_x,eye_y,eye_agngle and fov can see an object
//at x,y with w and h. Corners is the number of corners checked maximum is
//5 minimum is 1, 1 is the middle.
int object_is_in_fov(float eye_x, float eye_y,float eye_angle, float x, float y, int w, int h, float fov, int corners)
{
	int i;
	float point_x[5];
	float point_y[5];

	float max_angle;
	float min_angle;

	float object_angle;
	
	//make the 5 points
	point_x[0] = x;
	point_y[0] = y;
	point_x[1] = x-w/2;
	point_y[1] = y-h/2;
	point_x[2] = x+h/2;
	point_y[2] = y+w/2;
	point_x[3] = x+w/2;
	point_y[3] = y-h/2;
	point_x[4] = x-h/2;
	point_y[4] = y-w/2;
	
	//make the max and of min angle in the fov
	max_angle = add_angle(eye_angle, fov/2);
	min_angle = add_angle(eye_angle, -(fov/2));

	for(i=0;i<corners;i++)
	{
		object_angle=compute_angle(point_x[i], point_y[i], eye_x, eye_y);
		
		//check if the point is in the field of view
		if(max_angle>min_angle)
		{
			if(object_angle<min_angle || object_angle>max_angle)
				return 0;	
		}
		else
		{	
			if(object_angle<min_angle && object_angle>max_angle)
				return 0;	
		}

		if(path_is_clear(eye_x, eye_y, eye_angle, point_x[i],point_y[i],3,0))
			return 1;

	}	
	

	return 0;
}




static char tile_object_solidity[MAX_LAYER_H*MAX_LAYER_W];



void update_tile_object_height(void)
{
	int i,j,k;
	int min_x,max_x,min_y,max_y,temp;
	int type;
		
	memset(tile_object_solidity,0,MAX_LAYER_H*MAX_LAYER_W);
	
	
	//check for objects
	for(i=0;i<map->num_of_objects;i++)
		if(object_info[map->object[i].type].animation[map->object[i].action].solid>0)
		{
			
			temp = (int)map->object[i].angle/90;
			type = map->object[i].type;

									
			if(temp == 0 || temp == 2 || temp == 4)
			{
				min_x = (map->object[i].x - object_info[type].w/2)/32;
				max_x = (map->object[i].x + object_info[type].w/2)/32;
				min_y = (map->object[i].y - object_info[type].h/2)/32;
				max_y = (map->object[i].y + object_info[type].h/2)/32;

				for(j=min_x;j<max_x+1;j++)
					for(k=min_y;k<max_y+1;k++)
						if(min_y>-1 && min_x>-1 && max_x<map->w && max_y<map->h )
							if(check_collision(map->object[i].x-object_info[type].w/2,  map->object[i].y-object_info[type].h/2,  object_info[type].w, object_info[type].h,j*32,k*32,32,32)	)
							{
								if(tile_object_solidity[j+k*MAX_LAYER_W]<object_info[type].solid)
									tile_object_solidity[j+k*MAX_LAYER_W]=object_info[type].solid;
							}
			}
			else
			{
				min_x = (map->object[i].x - object_info[type].h/2)/32;
				max_x = (map->object[i].x + object_info[type].h/2)/32;
				min_y = (map->object[i].y - object_info[type].w/2)/32;
				max_y = (map->object[i].y + object_info[type].w/2)/32;

				for(j=min_x;j<max_x+1;j++)
					for(k=min_y;k<max_y+1;k++)
						if(min_y>-1 && min_x>-1 && max_x<map->w && max_y<map->h )
							if(check_collision(map->object[i].x-object_info[type].h/2,  map->object[i].y-object_info[type].w/2,  object_info[type].h, object_info[type].w,j*32,k*32,32,32)	)
							{
								if(tile_object_solidity[j+k*MAX_LAYER_W]<object_info[type].solid)
									tile_object_solidity[j+k*MAX_LAYER_W]=object_info[type].solid;
							}

			}


		}



	
	//check for player
	/*if(!player.dead)
	{
		min_x = (player.x - char_info[0].w/2)/32;
		max_x = (player.x + char_info[0].w/2)/32;
		min_y = (player.y - char_info[0].h/2)/32;
		max_y = (player.y + char_info[0].h/2)/32;

		for(j=min_x;j<max_x+1;j++)
			for(k=min_y;k<max_y+1;k++)
				if(min_y>-1 && min_x>-1 && max_x<map->w && max_y<map->h )
					if(check_collision(player.x - char_info[0].w/2,  player.y -char_info[0].h/2,  char_info[0].w, char_info[0].h,j*32,k*32,32,32))
					{
							if(tile_object_solidity[j+k*MAX_LAYER_W]<1)
								tile_object_solidity[j+k*MAX_LAYER_W]=1;
					}
	}*/


}


//Checks if a tile is clear 
//first if the tile has solidity or above and then if any objects of
//solidity or above is in it.
static int tile_is_not_clear(int x,int y,int solidity,int check_player)
{
	
	if(tile_is_solid(x, y)>=solidity)
		return 1;

	if(x>-1 && y>-1 && x<map->w && y<map->h)
		if(tile_object_solidity[x+y*MAX_LAYER_W]>=solidity)
			return 1;

	

	if(check_player)
	{
		if(!player.dead)
			if( check_angle_collision(player.x,  player.y,  char_info[0].w, char_info[0].h,0,x*32,y*32,32,32,0))
			{
				return 1;
			}
	}

	return 0;
}



//Function used by object_is_in_fov (amongst other).
//check if the line between eye_x,y and x,y has any obejcts tiles.
//with solidity. IF check_player is 1 then it also checks for the player.
int path_is_clear(float eye_x, float eye_y,float eye_angle, float x, float y, int solidity, int check_player)
{
	int tile_x;
	int tile_y;
	float p_x;
	float p_y;
	
	
	float object_angle;
	
	float dx;
	float dy;

	int check=0;

	float point_dist;
		
		
	check=0;

	object_angle=compute_angle(x, y, eye_x, eye_y);

	point_dist = distance(x,y,eye_x,eye_y);
		
				
	//CHECK IF THERE IS TILES IN THE WAY

	//--Check any horisontal collisons--

	//find first intersect
	if(object_angle>270 || object_angle<90)
		p_y = (int)(eye_y/32) * 32 - 1;
	else
		p_y = (int)(eye_y/32) * 32 + 32;
	tile_y = p_y/32;

	p_x = eye_x + (eye_y-p_y)*TAN(object_angle);
	tile_x = p_x/32;

		
	if(tile_is_not_clear(tile_x, tile_y,solidity,check_player) && point_dist>= distance(p_x,p_y,eye_x,eye_y))
			check=1;
				
		
	if(check==0)
	{
		//get the adds that we will use soon.....
		if(object_angle>270 || object_angle<90)
			dy=-32;
		else
			dy=32;
		
		if(object_angle>270 || object_angle<90)
			dx = 32*TAN(object_angle);
		else
			dx = -32*TAN(object_angle);


		
		//Loop til we come to the point
		while(check==0 && point_dist>= distance(p_x,p_y,eye_x,eye_y))
		{
			p_x+=dx;
			p_y+=dy;

			tile_x=p_x/32;
			tile_y=p_y/32;
	
					
			if(tile_is_not_clear(tile_x, tile_y,solidity,check_player) && point_dist>= distance(p_x,p_y,eye_x,eye_y))
				check=1;
		}

	}
		

	if(check==0)//--Check any vertical collisons--
	{
		//find first intersect
		if(object_angle>0 && object_angle<180)
			p_x = (int)(eye_x/32) * 32 + 32;
		else
			p_x = (int)(eye_x/32) * 32 - 1;
		tile_x = p_x/32;
			p_y = eye_y + (eye_x-p_x)/TAN(object_angle);
		tile_y = p_y/32;
		
		
		if(tile_is_not_clear(tile_x, tile_y,solidity,check_player) && point_dist>= distance(p_x,p_y,eye_x,eye_y))
			check=1;
	}

	if(check==0)
	{
		//get the adds that we will use soon.....
		if(object_angle>0 && object_angle<180)
			dx=32;
		else
			dx=-32;
		
		if(object_angle>0 && object_angle<180)
			dy = -32/TAN(object_angle);
		else
			dy = 32/TAN(object_angle);

		//Loop til we come to the point
		while(check==0 && point_dist>= distance(p_x,p_y,eye_x,eye_y))
		{
			p_x+=dx;
			p_y+=dy;
			tile_x=p_x/32;
			tile_y=p_y/32;
	
					
			if(tile_is_not_clear(tile_x, tile_y,solidity,check_player) && point_dist>= distance(p_x,p_y,eye_x,eye_y))
					check=1;
		}
	}
		
		
		
	/*//CHECK IF THERE IS OBJECTS IN THE WAY
	if(check==0)
	{
		for(j=0;j<map->num_of_objects;j++)
		{
			if(map->object[j].active)
				if(object_info[map->object[j].type].solid>=solidity && object_info[map->object[j].type].animation[map->object[j].action].solid>0)
					if( check_line_angle_collision(map->object[j].x,  map->object[j].y,  object_info[map->object[j].type].w  ,object_info[map->object[j].type].h/4,  ((int)(map->object[j].angle*((float)object_info[map->object[j].type].angles/360) )) *  ((float)360/object_info[map->object[j].type].angles)  ,
						eye_x, eye_y, x, y ))
					{
						check=1;
						break;
					}

		}
	}

	//CHECK IF THE PLAYER IS IN THE WAY
	if(check==0 && check_player)
	{
		if(!player.dead)
			if( check_line_angle_collision(player.x,  player.y,  char_info[0].w, char_info[0].h,0,eye_x, eye_y, x, y ))
			{
				check=1;
			}
	}*/


		
	if(check==0)
	{
		return 1;
	}
	else
	{
	
	}



	return 0;
}




//stuff that can be used in get_tile height!!!
/*size = sqrt(object_info[map->object[i].type].h*object_info[map->object[i].type].h + object_info[map->object[i].type].w*object_info[map->object[i].type].w)/2;

min_x = (map->object[i].x - size)/32;
max_x = (map->object[i].x + size)/32;
min_y = (map->object[i].y - size)/32;
max_y = (map->object[i].y + size)/32;

for(j=min_x;j<max_x+1;j++)
	for(k=min_y;k<max_y+1;k++)
	if(min_y>-1 && min_x>-1 && max_x<map->w && max_y<map->h )
		if(check_angle_collision(map->object[i].x,  map->object[i].y,  object_info[map->object[i].type].w  ,object_info[map->object[i].type].h,  map->object[i].angle,j*32+16,k*32+16,32,32,0)	)
		{
			if(tile_object_solidity[j+k*MAX_LAYER_W]<object_info[map->object[i].type].solid)
			tile_object_solidity[j+k*MAX_LAYER_W]=object_info[map->object[i].type].solid;
		}
		*/
