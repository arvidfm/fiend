////////////////////////////////////////////////////
// This file contains tile loading, updating and
// collision. 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include <allegro.h>

#include "fiend.h"
#include "grafik4.h"
#include "console.h"



#define TILE_ANIMATION_SPEED 20

//Info stuff
TILESET_INFO *tile_info;


//Data
RLE_ARRAY *tile_data[100];


//variables
int num_of_tilesets=0;


static int tile_frame_count=0;


void update_tiles(void)
{
	int i,j;

	for(i=0;i<num_of_tilesets;i++)
		for(j=0;j<tile_info[i].num_of_tiles;j++)
		{
			if(tile_info[i].tile[j].next_tile>-1 && tile_info[i].tile[j].anim_count >= tile_info[i].tile[j].anim_speed)
			{
				tile_info[i].tile[j].current_tile = tile_info[i].tile[tile_info[i].tile[j].current_tile].next_tile;
				tile_info[i].tile[j].anim_count=0;
			}
			tile_info[i].tile[j].anim_count++;
		}
}


int load_tiles(void)
{
	FILE *f;

	char buffer[50];
	char tile_file_name[100][60];
    char info_file_name[100][60];
	int num_of_tiles=1;
	char *file_path ="graphic\\tiles\\";
	char final_path[40];
	int i;

	
	tile_info = calloc(sizeof(TILESET_INFO), MAX_TILES);

	f = fopen("graphic\\tiles\\tiles.txt", "r");      //Load the Info file
	
	if(f==NULL)
	{sprintf(fiend_errorcode,"couldn't load tiles.txt");return 1;} //error testing...
    

	
	while(fscanf(f, "%s %s\n", tile_file_name[num_of_tilesets], info_file_name[num_of_tilesets])!=EOF)
	{
		num_of_tilesets++;	
	}

	fclose(f);

	
	
	for(i=0;i<num_of_tilesets;i++)
	{
		strcpy(final_path, file_path);
		strcat(final_path, tile_file_name[i]);   //Get the tile data .

														 
	    tile_data[i] = load_rle_array(final_path,-1);//load the graphic
		if(tile_data[i]==NULL){sprintf(fiend_errorcode,"couldn't load %s",final_path);return 1;}//error testing...
		

		strcpy(final_path, file_path);
		strcat(final_path, info_file_name[i]);    //Get the tile info.

		f = fopen(final_path, "r");
		if(f==NULL){sprintf(fiend_errorcode,"couldn't load %s",final_path); return 1;}//error testing...
		
		fscanf(f, "%s %s\n",buffer, tile_info[i].name); //get the set name and the number of tiles
		tile_info[i].num_of_tiles=0;
		
		while(fscanf(f,"%s %d %s %d %s %d %s %d %s %d %s %s\n",buffer, &tile_info[i].tile[tile_info[i].num_of_tiles].masked, buffer, &tile_info[i].tile[tile_info[i].num_of_tiles].trans, buffer, &tile_info[i].tile[tile_info[i].num_of_tiles].solid,buffer ,&tile_info[i].tile[tile_info[i].num_of_tiles].next_tile,buffer ,&tile_info[i].tile[tile_info[i].num_of_tiles].anim_speed, buffer, tile_info[i].tile[tile_info[i].num_of_tiles].sound)!=EOF)
		{
			tile_info[i].tile[tile_info[i].num_of_tiles].current_tile = tile_info[i].num_of_tiles;
			tile_info[i].tile[tile_info[i].num_of_tiles].anim_count = 0;
			tile_info[i].num_of_tiles++;
		}
		

		fclose(f);

	}
   
	
	return 0;

 
}



//realease all data that has been allocated
void release_tiles(void)
{
	int i;

	for(i=0;i<num_of_tilesets;i++)
	{
		unload_rle_array(tile_data[i]);

	}
	
	free(tile_info);
}





//draws a tilelayer....
//Don't forget that the FIRST layer is 1 NOT 0
void draw_tile_layer(BITMAP *virt, int layer,int solid, int xpos, int ypos)
{
	int i,j;
    int x,y,x1,y1;
	int tile_set[4];
	int	tile_num[4];

     x1=0-(xpos%TILE_SIZE);//check where on the tile map you begin to draw
     y1=0-(ypos%TILE_SIZE);
	 

	 x=(xpos-(xpos%TILE_SIZE))/TILE_SIZE;//what tile do you begin with 
     y=(ypos-(ypos%TILE_SIZE))/TILE_SIZE;

	 

	 for(i=-1;i< (virt->w/TILE_SIZE+1) ;i++)
      for(j=-1;j< (virt->h/TILE_SIZE+1) ;j++)
	  {
		  	  
	      if((i+x < 0)|| (j+y < 0) || (j+y > map->h-1) || (i+x > map->w-1) ) //if the tile is out side the map it is just a black square
		  {

			if(layer==3)
			 draw_rle_sprite(virt, tile_data[0][1].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
			 		   
		  }
		  else //get the set and the number of the tiles
		  {
			tile_set[1] = (map->layer1+ (i+x) +( (j+y) * map->w))->tile_set;
			tile_num[1] = (map->layer1+ (i+x) +( (j+y) * map->w))->tile_num; 
			tile_num[1] = tile_info[tile_set[1]].tile[tile_num[1]].current_tile;
		  
			tile_set[2] = (map->layer2+ (i+x) +( (j+y) * map->w))->tile_set;
			tile_num[2] = (map->layer2+ (i+x) +( (j+y) * map->w))->tile_num;  
			tile_num[2] = tile_info[tile_set[2]].tile[tile_num[2]].current_tile;
		  
		  	tile_set[3] = (map->layer3+ (i+x) +( (j+y) * map->w))->tile_set;
			tile_num[3] = (map->layer3+ (i+x) +( (j+y) * map->w))->tile_num;
			tile_num[3] = tile_info[tile_set[3]].tile[tile_num[3]].current_tile;
		  

			
			set_trans_blender(0,0,0,128);
			
			//draw_rle_sprite(virt, tile_data[ tile_set[layer] ][ tile_num[layer] ].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
					
			switch(layer) //What layer should be drawn...
			{
			case 1://Check if there any solid tiles above if not draw the tile
				if(tile_info[tile_set[1]].tile[tile_num[1]].solid==solid)
					if( tile_info[tile_set[2]].tile[tile_num[2]].masked || tile_info[tile_set[2]].tile[tile_num[2]].trans ||
						tile_info[tile_set[3]].tile[tile_num[3]].masked || tile_info[tile_set[3]].tile[tile_num[3]].trans)
						{
							draw_rle_sprite(virt, tile_data[ tile_set[layer] ][ tile_num[layer] ].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
						}
				break;

			
			case 2://Check if there any solid tiles above if not draw the tile
				if(tile_info[tile_set[2]].tile[tile_num[2]].solid==solid)
					if( tile_info[tile_set[3]].tile[tile_num[3]].masked || tile_info[tile_set[3]].tile[tile_num[3]].trans)
					{
						if(tile_set[layer]!=0 || tile_num[layer]!=0)//if it is balnk don't draw it	
							if(tile_info[tile_set[layer]].tile[tile_num[layer]].trans)//shall trans or normal drawing bee used..
								draw_trans_rle_sprite(virt, tile_data[ tile_set[layer] ][ tile_num[layer] ].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
							else
								draw_rle_sprite(virt, tile_data[ tile_set[layer] ][ tile_num[layer] ].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
					}
				break;

			
			case 3:
				if(tile_info[tile_set[3]].tile[tile_num[3]].solid==solid)  
					if(tile_set[layer]!=0 || tile_num[layer]!=0)//if it is balnk don't draw it	
						if(tile_info[tile_set[layer]].tile[tile_num[layer]].trans)//shall trans or normal drawing bee used..
							draw_trans_rle_sprite(virt, tile_data[ tile_set[layer] ][ tile_num[layer] ].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
						else
							draw_rle_sprite(virt, tile_data[ tile_set[layer] ][ tile_num[layer] ].dat, x1+(i*TILE_SIZE), y1+(j*TILE_SIZE));
								
				break;



			}

		 
		       
	 		 
		  }
	  }
	  



}



int tile_is_solid(int x, int y)
{
	int tile_set;
	int tile_num;

	int i;

	int max_solid=0;
	
	if(x<0 || x>map->w-1 || y<0 || y>map->h-1)
		return 0;
	
	for(i=0;i<3;i++)
	{
		if(i==0)
		{
			tile_set = (map->layer1+ (x) +( (y) * map->w))->tile_set;
			tile_num = (map->layer1+ (x) +( (y) * map->w))->tile_num; 			
		}
		if(i==1)
		{
			tile_set = (map->layer2+ (x) +( (y) * map->w))->tile_set;
			tile_num = (map->layer2+ (x) +( (y) * map->w))->tile_num; 			
		}
		if(i==2)
		{
			tile_set = (map->layer3+ (x) +( (y) * map->w))->tile_set;
			tile_num = (map->layer3+ (x) +( (y) * map->w))->tile_num; 			
		}

		if(tile_info[tile_set].tile[tile_num].solid>max_solid)
			max_solid = tile_info[tile_set].tile[tile_num].solid;
	}

	
	if(max_solid==2)
		return 3;
	else
		return max_solid;
}



//check if a somethings collides with a tile....
int check_tile_collision(float x, float y, int w, int h)
{
	int tile_x, tile_y;
	int tile_set,tile_num;

	int i,j;
	
	int layer;

	map_tile((int)x, (int)y, &tile_x, &tile_y,TILE_SIZE);

	
	for(layer = 0;layer<3;layer++)
		for(i=0;i<w/TILE_SIZE+2;i++)
			for(j=0;j<h/TILE_SIZE+2;j++)
			{
				if(i+tile_x<map->w && i+tile_x>=0 && j+tile_y<map->h && j+tile_y>=0)
				{
					if(layer==0){
					tile_set = (map->layer1+ (i+tile_x) +( (j+tile_y) * map->w))->tile_set;
					tile_num = (map->layer1+ (i+tile_x) +( (j+tile_y) * map->w))->tile_num; 			
					}
					if(layer==1){
					tile_set = (map->layer2+ (i+tile_x) +( (j+tile_y) * map->w))->tile_set;
					tile_num = (map->layer2+ (i+tile_x) +( (j+tile_y) * map->w))->tile_num; 			
					}
					if(layer==2){
					tile_set = (map->layer3+ (i+tile_x) +( (j+tile_y) * map->w))->tile_set;
					tile_num = (map->layer3+ (i+tile_x) +( (j+tile_y) * map->w))->tile_num; 			
					}

					if(tile_info[tile_set].tile[tile_num].solid &&
					   check_collision(x, y, w,h,(float)(tile_x+i)*TILE_SIZE,(float) (tile_y+j)*TILE_SIZE,TILE_SIZE,TILE_SIZE) )
						if(layer==2)	
							return tile_info[tile_set].tile[tile_num].solid+1;
						else
							return tile_info[tile_set].tile[tile_num].solid;
				}

			}

	return 0;	
}