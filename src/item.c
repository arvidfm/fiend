////////////////////////////////////////////////////
// This file contains the item drawing and loading funcs 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "grafik4.h"


#include "fiend.h"
#include "item.h"



char item_filename[80] = "maps/item.inf";

float item_light=128;
float item_light_add=4.5;



ITEM_DATA *item_data;
ITEM_INFO *item_info;
RLE_ARRAY *item_pic;
DATAFILE *big_item_pic;

//variables
int num_of_items=0;

int load_items(void)
{
	FILE *f;

	char buffer[50];
				
	item_info = calloc(sizeof(ITEM_INFO), MAX_ITEM_INFO);
	
															 
	f = fopen("graphic/misc/items.txt", "r");
	if(f==NULL){sprintf(fiend_errorcode,"couldn't load items.txt"); return 1;}//error testing...
	
	while(fscanf(f,"%s %s %s %d %s %d %s %d %s %d %s %s %s %s\n",buffer,&item_info[num_of_items].name,buffer,&item_info[num_of_items].w,buffer,&item_info[num_of_items].h, buffer,&item_info[num_of_items].num, buffer, &item_info[num_of_items].type, buffer, item_info[num_of_items].s_string, buffer,item_info[num_of_items].desc)!=EOF)
	{
		num_of_items++;
	}
		
	fclose(f);

	//big_item_pic = load_datafile("graphic/misc/big_items.dat");//load the graphic
	///if(big_item_pic==NULL) {sprintf(fiend_errorcode,"couldn't load big_items.dat");return 1;}//error testing...
	item_pic = load_rle_array("graphic/misc/items",num_of_items);//load the graphic
	if(item_pic==NULL) {sprintf(fiend_errorcode,"couldn't load items/");return 1;}//error testing...
	

	return 0;

 
}


void release_items(void)
{
	free(item_info);
	unload_rle_array(item_pic);
}




//initaialze det item stuff and clear the item struct
int init_item_data(void)
{
	int i;

	item_data = calloc(sizeof(ITEM_DATA),MAX_ITEM_DATA);

	for(i=0;i<MAX_ITEM_DATA;i++)
	{
		item_data[i].used=0;
		item_data[i].active=1;
		
		strcpy(item_data[i].map_name,"");
	}
	
	return 0;
}



//load the npc struct 
int load_item_data(void)
{
	FILE *f;
	int i;

	f = fopen(item_filename,"rb");
	if(f==NULL)
	{
		for(i=0;i<MAX_ITEM_DATA;i++)
		{
				item_data[i].used=0;
				item_data[i].active=1;
		
				strcpy(item_data[i].map_name,"");
		}
	
		return 1; //when you start fresh you have no npc:s so you can't give an error code here
	}
	
	fread(item_data, sizeof(ITEM_DATA), MAX_ITEM_DATA, f);

	fclose(f);

	return 1;
}



//save the npc struct
int save_item_data(void)
{
	FILE *f;

	f = fopen(item_filename,"wb");
	if(f==NULL)return 0;

	fwrite(item_data, sizeof(ITEM_DATA), MAX_ITEM_DATA, f);

	fclose(f);

	return 1;
}


void release_item_data(void)
{
	free(item_data);
}



void draw_fiend_item(BITMAP *dest,int num,int x, int y,float angle, int lit)
{
	fixed f_angle;
	
	f_angle = degree_to_fixed(angle);

	if(lit)
	{
		set_trans_blender(255,255,255,0);
		draw_lit_rle_sprite(dest,item_pic[num].dat,x - ((RLE_SPRITE *)item_pic[num].dat)->w/2, y - ((RLE_SPRITE *)item_pic[num].dat)->h/2,item_light);
	}
	else
	{
		draw_rle_sprite(dest,item_pic[num].dat,x - ((RLE_SPRITE *)item_pic[num].dat)->w/2, y - ((RLE_SPRITE *)item_pic[num].dat)->h/2);
	}
}



