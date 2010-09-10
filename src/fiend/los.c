////////////////////////////////////////////////////
// This file contains Line of sight gfx making, drawing
// and stuff
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <time.h>
#include <stdio.h>

#include "../draw.h"
#include "../fiend.h"
#include "../grafik4.h"


static int tile_pos_x;
static int tile_pos_y;

int los_buffer[18][18];

BITMAP *los_border[3][4]; //0=right 1=down 2=left 3=up

void clear_los_buffer(void)
{
	int i,j;

	for(i=0;i<18;i++)
		for(j=0;j<18;j++)
			los_buffer[i][j]=0;
}


//for the delter
int los_buffer_check(int x, int y)
{
	if(x<0)return 1;
	if(x>16) return 1;
	if(y<0)return 1;
	if(y>16) return 1;

	if(x-1 + tile_pos_x<0)return 1;
	if(y-1 + tile_pos_y<0)return 1;
	if(x-1 + tile_pos_x>=map->w)return 1;
	if(y-1 + tile_pos_y>=map->h)return 1;
		
	if(tile_is_wall_solid(x + tile_pos_x-1, y + tile_pos_y-1))return 1;

	return los_buffer[x][y];
}

//for the drawer..
int los_buffer_check2(int x, int y)
{
	if(x-1 + tile_pos_x<0)return 1;
	if(y-1 + tile_pos_y<0)return 1;
	if(x-1 + tile_pos_x>=map->w)return 1;
	if(y-1 + tile_pos_y>=map->h)return 1;

	
	if(x<0)return 0;
	if(x>17) return 0;
	if(y<0)return 0;
	if(y>17) return 0;

		
	//if(tile_is_wall_solid(x + tile_pos_x, y + tile_pos_y))return 1;

	return los_buffer[x][y];
}


void draw_los_buffer(BITMAP *dest, int xpos, int ypos)
{
	int i,j,l_i,l_j;
    int x,y,x1,y1;
	
    x1=0-(xpos%TILE_SIZE);//check where on the tile map you begin to draw
    y1=0-(ypos%TILE_SIZE);
	 

	x=(xpos-(xpos%TILE_SIZE))/TILE_SIZE;//what tile do you begin with 
    y=(ypos-(ypos%TILE_SIZE))/TILE_SIZE;

	tile_pos_x = xpos/32;
	tile_pos_y = ypos/32;

 
	for(i=-1;i< (virt->w/TILE_SIZE+1) ;i++)
		for(j=-1;j< (virt->h/TILE_SIZE+1) ;j++)
		{
		  	l_i = i+1;			
			l_j = j+1;			
			  
			
			if((i+x < 0)|| (j+y < 0) || (j+y > map->h-1) || (i+x > map->w-1) ) //if the tile is out side the map it is just a black square
			{
			 		   
			}
			else if(los_buffer[l_i][l_j]==0 && !tile_is_wall_solid(i+xpos/32, j+ypos/32))
			{
				
				//--Border--//

				//border right
				if(los_buffer_check2(l_i-1,l_j) && !los_buffer_check2(l_i,l_j-1) && !los_buffer_check2(l_i,l_j+1) )
				{
					draw_lightsprite(dest,los_border[0][0],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//border down
				if(los_buffer_check2(l_i,l_j-1) && !los_buffer_check2(l_i-1,l_j) && !los_buffer_check2(l_i+1,l_j) )
				{
					draw_lightsprite(dest,los_border[0][1],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//border left
				if(los_buffer_check2(l_i+1,l_j) && !los_buffer_check2(l_i,l_j-1) && !los_buffer_check2(l_i,l_j+1) )
				{
					draw_lightsprite(dest,los_border[0][2],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				
				//border up
				if(los_buffer_check2(l_i,l_j+1) && !los_buffer_check2(l_i-1,l_j) && !los_buffer_check2(l_i+1,l_j) )
				{
					draw_lightsprite(dest,los_border[0][3],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//--Inner Corner--//

				//right
				if(los_buffer_check2(l_i-1,l_j) && los_buffer_check2(l_i,l_j+1) && !los_buffer_check2(l_i+1,l_j) && !los_buffer_check2(l_i,l_j-1))
				{
					draw_lightsprite(dest,los_border[1][0],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//down
				if(los_buffer_check2(l_i-1,l_j) && los_buffer_check2(l_i,l_j-1) && !los_buffer_check2(l_i+1,l_j) && !los_buffer_check2(l_i,l_j+1))
				{
					draw_lightsprite(dest,los_border[1][1],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//left
				if(los_buffer_check2(l_i+1,l_j) && los_buffer_check2(l_i,l_j-1) && !los_buffer_check2(l_i-1,l_j) && !los_buffer_check2(l_i,l_j+1))
				{
					draw_lightsprite(dest,los_border[1][2],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//up
				if(los_buffer_check2(l_i+1,l_j) && los_buffer_check2(l_i,l_j+1) && !los_buffer_check2(l_i-1,l_j) && !los_buffer_check2(l_i,l_j-1))
				{
					draw_lightsprite(dest,los_border[1][3],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//--Outer Corner--//

				//right
				if(los_buffer_check2(l_i-1,l_j+1) && !los_buffer_check2(l_i-1,l_j) && !los_buffer_check2(l_i,l_j+1))
				{
					draw_lightsprite(dest,los_border[2][0],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
								
				//down
				if(los_buffer_check2(l_i-1,l_j-1) && !los_buffer_check2(l_i-1,l_j) && !los_buffer_check2(l_i,l_j-1))
				{
					draw_lightsprite(dest,los_border[2][1],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//left
				if(los_buffer_check2(l_i+1,l_j-1) && !los_buffer_check2(l_i+1,l_j) && !los_buffer_check2(l_i,l_j-1))
				{
					draw_lightsprite(dest,los_border[2][2],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//up
				if(los_buffer_check2(l_i+1,l_j+1) && !los_buffer_check2(l_i+1,l_j) && !los_buffer_check2(l_i,l_j+1))
				{
					draw_lightsprite(dest,los_border[2][3],x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}

				//--3 Wall corner

				//right
				if(los_buffer_check2(l_i,l_j-1) && los_buffer_check2(l_i+1,l_j) && los_buffer_check2(l_i,l_j+1))
				{
					draw_rle_sprite(dest, tile_data[0][1].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//down
				if(los_buffer_check2(l_i,l_j+1) && los_buffer_check2(l_i+1,l_j) && los_buffer_check2(l_i-1,l_j))
				{
					draw_rle_sprite(dest, tile_data[0][1].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//left
				if(los_buffer_check2(l_i,l_j-1) && los_buffer_check2(l_i-1,l_j) && los_buffer_check2(l_i,l_j+1))
				{
					draw_rle_sprite(dest, tile_data[0][1].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
				//up
				if(los_buffer_check2(l_i,l_j-1) && los_buffer_check2(l_i-1,l_j) && los_buffer_check2(l_i+1,l_j))
				{
					draw_rle_sprite(dest, tile_data[0][1].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				}
				
			}
			else if(los_buffer[l_i][l_j]==1)
			{
				draw_rle_sprite(dest, tile_data[0][1].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
				//set_trans_blender(0,0,0,180);	
				//draw_rle_sprite(dest, tile_data[0][3].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
			}
			/*else if(los_buffer[i][j]==2)
			{
				set_trans_blender(0,0,0,140);	
				draw_trans_rle_sprite(dest, tile_data[0][2].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
			}*/
			
			
		
		}
}



void update_los_buffer(int xpos, int ypos)
{
	int i,j;
    int x,y,x1,y1;
	int l_i, l_j;

	int temp_los_buffer[18][18];

	int c1,c2,c3,c4;

    
	x1=0-(xpos%TILE_SIZE);//check where on the tile map you begin to draw
    y1=0-(ypos%TILE_SIZE);

	x=(xpos-(xpos%TILE_SIZE))/TILE_SIZE;//what tile do you begin with 
    y=(ypos-(ypos%TILE_SIZE))/TILE_SIZE;

	tile_pos_x = xpos/32;
	tile_pos_y = ypos/32;

	for(i=-1;i< (virt->w/TILE_SIZE+1) ;i++)
		for(j=-1;j< (virt->h/TILE_SIZE+1) ;j++)
		{
			l_i = i+1;			
			l_j = j+1;			
			
			if((i+x < 0)|| (j+y < 0) || (j+y > map->h-1) || (i+x > map->w-1) ) //if the tile is out side the map it is just a black square
			{

			 		   
			}
			else if(!object_is_in_fov(player.x, player.y, player.angle,((x+i)*32)+16 ,((j+y)*32)+16 ,32,32,360,1))
			{
				if(!tile_is_wall_solid(i+xpos/32, j+ypos/32))
					los_buffer[l_i][l_j]=1;
			}
		}


	//make the check if los tils is "trapped" 
	//if so make em black tiles
	for(i=-1;i<17;i++)
		for(j=-1;j<17;j++)
		if(!tile_is_wall_solid(i+xpos/32, j+ypos/32) )
		{
			c1=0;
			c2=0;
			c3=0;
			c4=0;
			
			l_i = i+1;			
			l_j = j+1;			
			
			
			//above tile
			if(l_j>0)
			{
				if(los_buffer[l_i][l_j-1]==1 || tile_is_wall_solid(i+xpos/32, (j-1)+ypos/32))
					c1=1;
			}
			else
			{
				c1=1;
			}

			//right tile
			if(l_i<18)
			{
				if(los_buffer[l_i+1][l_j]==1 || tile_is_wall_solid((i+1)+xpos/32, (j)+ypos/32))
					c2=1;
			}
			else
			{
				c2=1;
			}

			//down tile
			if(l_j<18)
			{
				if(los_buffer[l_i][l_j+1]==1 || tile_is_wall_solid((i)+xpos/32, (j+1)+ypos/32))
					c3=1;
			}
			else
			{
				c3=1;
			}

			//left tile
			if(l_i>0)
			{
				if(los_buffer[l_i-1][l_j]==1 || tile_is_wall_solid((i-1)+xpos/32, (j)+ypos/32))
					c4=1;
			}
			else
			{
				c4=1;
			}

			
			//check if there is a dark piece on the map drawn
			if(los_buffer[l_i][l_j]==0)
				if((c1==1 && c2==1 && c3==1 && c4==1) || (c2==1 && c3==1 && c4==1) || (c1==1 && c3==1 && c4==1) || 
					(c1==1 && c2==1 && c4==1) || (c1==1 && c2==1 && c3==1))
				{
					los_buffer[l_i][l_j]=1;
				}
		}

		//set the temp_buufer
		for(i=0;i<18;i++)
				for(j=0;j<18;j++)
					temp_los_buffer[i][j] = los_buffer[i][j];

		
		//check if the "black tile" is a corner or side if
		//so make it clear at the temp buffer.
		for(i=-1;i<17;i++)
			for(j=-1;j<17;j++)
			{
				l_i = i+1;
				l_j = j+1;
				
				if(los_buffer[l_i][l_j]==1)
				{
					//--Border--//
					if(los_buffer_check(l_i-1,l_j) && los_buffer_check(l_i,l_j-1) && los_buffer_check(l_i,l_j+1) && !los_buffer_check(l_i+1,l_j))
					{
						temp_los_buffer[l_i][l_j]=0;
					}
					else if(los_buffer_check(l_i,l_j-1) && los_buffer_check(l_i-1,l_j) && los_buffer_check(l_i+1,l_j) && !los_buffer_check(l_i,l_j+1))
					{
						temp_los_buffer[l_i][l_j]=0;
					}
					else if(los_buffer_check(l_i+1,l_j) && los_buffer_check(l_i,l_j-1) && los_buffer_check(l_i,l_j+1) && !los_buffer_check(l_i-1,l_j))
					{
						temp_los_buffer[l_i][l_j]=0;
					}
					else if(los_buffer_check(l_i,l_j+1) && los_buffer_check(l_i-1,l_j) && los_buffer_check(l_i+1,l_j) &&  !los_buffer_check(l_i,l_j-1))
					{
						temp_los_buffer[l_i][l_j]=0;
					}
					 //--Inner Corner--//
					else if(los_buffer_check(l_i-1,l_j) && los_buffer_check(l_i,l_j+1) && !los_buffer_check(l_i+1,l_j) && !los_buffer_check(l_i,l_j-1))
					{
						temp_los_buffer[l_i][l_j]=0;
					}
					else if(los_buffer_check(l_i-1,l_j) && los_buffer_check(l_i,l_j-1) && !los_buffer_check(l_i+1,l_j) && !los_buffer_check(l_i,l_j+1))
					{
						temp_los_buffer[l_i][l_j]=0;
					}
					else if(los_buffer_check(l_i+1,l_j) && los_buffer_check(l_i,l_j-1) && !los_buffer_check(l_i-1,l_j) && !los_buffer_check(l_i,l_j+1))
					{
						temp_los_buffer[l_i][l_j]=0;
					}
					else if(los_buffer_check(l_i+1,l_j) && los_buffer_check(l_i,l_j+1) && !los_buffer_check(l_i-1,l_j) && !los_buffer_check(l_i,l_j-1))
					{
						temp_los_buffer[l_i][l_j]=0;
					}
					
				}
			}

		//the normal los buffer becomes the temp
		for(i=0;i<18;i++)
			for(j=0;j<18;j++)
				los_buffer[i][j] = temp_los_buffer[i][j];
		
		
	

}


//the isze of a corner or side of the los gfx
#define LOS_BORDER_LENGTH 29


//make the line of sight gfx
void make_los_borders(void)
{
	int i,j;
	float col, add;


	for(i=0;i<3;i++)
		for(j=0;j<4;j++)
		{
			los_border[i][j] = create_bitmap_ex(8,32,32);
			clear_to_color(los_border[i][j], 31);
		}

	
	//do the border bitmap
	add = (0 - 255)/(0 - LOS_BORDER_LENGTH);
	
	col = 0;
	for(i=0;i<LOS_BORDER_LENGTH;i++)
	{
		col+=add;
		line(los_border[0][0],i,0,i,31,col/8);
	}
	draw_sprite_h_flip(los_border[0][2],los_border[0][0],0,0);
	
	col=0;
	for(i=0;i<LOS_BORDER_LENGTH;i++)
	{
		col+=add;
		line(los_border[0][1],0,i,31,i,col/8);
	}
	draw_sprite_v_flip(los_border[0][3],los_border[0][1],0,0);
	
		
	//do the inner corner bitmap
	col = 0;
	/*for(i=0;i<LOS_BORDER_LENGTH;i++)
	{
		col+=add;
		line(los_border[1][0],i,0,i,31-i,col/8);
		line(los_border[1][0],i,31-i,31,31-i,col/8);
	}*/
	
	
	for(i=0;i<LOS_BORDER_LENGTH;i++)
	{
		col+=add;
		line(los_border[1][0],i,0,31,31-i,col/8);
	}
	draw_sprite_v_flip(los_border[1][1],los_border[1][0],0,0);
	draw_sprite_h_flip(los_border[1][2],los_border[1][1],0,0);
	draw_sprite_h_flip(los_border[1][3],los_border[1][0],0,0);
	
	for(i=0;i<32;i++)
		line(los_border[1][0],0,i,31-i,31,1);
	for(i=0;i<32;i++)
		line(los_border[1][1],0,i,i,0,1);
	for(i=0;i<32;i++)
		line(los_border[1][2],31-i,0,31,31,1);
	for(i=0;i<32;i++)
		line(los_border[1][3],i,31,31,i,1);
	
	
	
	//do outer corner bitmap
	col = 0;
	for(i=0;i<LOS_BORDER_LENGTH;i++)
	{
		col+=add;
		line(los_border[2][0],0,31-i,i,31,col/8);
	}
	draw_sprite_v_flip(los_border[2][1],los_border[2][0],0,0);
	draw_sprite_h_flip(los_border[2][2],los_border[2][1],0,0);
	draw_sprite_h_flip(los_border[2][3],los_border[2][0],0,0);
		

	
}

//check if an object is coverd with "black" los tiles.
int object_is_in_player_los(float x, float y, int w, int h, float angle, int solid)
{
	int i,j;
	int tile_x, tile_y;
	int d_x,d_y;
	int max;

	 
	
		
	if(angle==0)
	{
		if(!check_collision(x-w/2,y-h/2,w,h,map_x-20,map_y-20,520,520))
			return 0;
		
		map_tile(x-w/2, y-h/2, &tile_x, &tile_y,TILE_SIZE);
			
		for(i=tile_x;i<(w/32+2)+tile_x;i++)
			for(j=tile_y;j<(h/32+2)+tile_y;j++)
			{
				d_x = i-map_x/32;
				d_y = j-map_y/32;
				
				if(d_x>=0 && d_x<=14 && d_y>=0 && d_y<=14 && i>-1 && j>-1 && i<map->w && j<map->h)	
					if(los_buffer[d_x][d_y]==0 )
						//if(tile_info[(map->layer3+ (i) +( (j) * map->w))->tile_set].tile[(map->layer3+ (i) +( (j) * map->w))->tile_num].solid!=2)
							return 1;
			}
	}
	else
	{
		max = sqrt(w*w + h*h);
		if(!check_collision(x-max/2, y-max/2, max, max, map_x, map_y, 480, 480))
			return 0;
		
		map_tile(x-max/2, y-max/2, &tile_x, &tile_y, TILE_SIZE);

		for(i=tile_x;i<(max/32+2)+tile_x;i++)
			for(j=tile_y;j<(max/32+2)+tile_y;j++)
			{
				d_x = i-map_x/32;
				d_y = j-map_y/32;
				
				if(check_angle_collision(x,y,w + 32, h + 32, angle, i*32+16, j*32+16, 32,32,0))
					if(d_x>=0 && d_x<=15 && d_y>=0 && d_y<=15&& i>-1 && j>-1 && i<map->w && j<map->h)	
						if(los_buffer[d_x][d_y]==0)
							//if(tile_info[(map->layer3+ (i) +( (j) * map->w))->tile_set].tile[(map->layer3+ (i) +( (j) * map->w))->tile_num].solid!=2)
								return 1;

			}
	

	}


	return 0;
}


