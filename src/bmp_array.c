////////////////////////////////////////////////////
// This file contains bitmap array loading funcs
//
// all code by Thomas Grip 
///////////////////////////////////////////////////

#include <allegro.h>
#include <stdio.h>
#include <string.h>


#include "draw.h"
#include "fiend.h"
#include "grafik4.h"


#define MAX_BMP_ARRAY_DATA 100
#define MAX_RLE_ARRAY_DATA 100

BMP_ARRAY *temp_bmp_data;

RLE_ARRAY *temp_rle_data;


///////////////////////////////////////////////////////
////// THE BITMAP ARRAY FUNCTIONS /////////////////////
//////////////////////////////////////////////////////
 
static /*int*/ void find_one_file(const char *file,int attr,/*void **/int param)
{
	temp_bmp_data[0].dat = load_bmp(file,NULL);
    //return 0;
}


BMP_ARRAY* load_bmp_array(char *dir_tmp,int item_num)
{
	char file_path[90];
	char file_name[20];
	int i;
	
	char dir[256];  // NOTE: Workaround; don't call put_backslash() with a string literal (which dir_tmp is).
	sprintf(dir, "%s", dir_tmp);
	
	put_backslash(dir);
	
	if(item_num==-1)
		temp_bmp_data = calloc(sizeof(BMP_ARRAY), MAX_BMP_ARRAY_DATA);
	else
		temp_bmp_data = calloc(sizeof(BMP_ARRAY), item_num);
	
	
	if(item_num==1)
	{
		sprintf(file_path,"%s*.bmp",dir);
        printf("for_each_file_ex(\"%s\",FA_ALL,0,find_one_file,0)\n", file_path);
		i= for_each_file/*_ex*/(file_path,FA_ALL/*,0*/,find_one_file,0);

		if(i!=1){allegro_message("couldn't load %s",file_path);exit(-1);}
		
		temp_bmp_data[0].num = item_num;
	}
	else if(item_num==-1)
	{
		i=0;
		while(1)
		{
			if(i<10)
				sprintf(file_name,"00%d.bmp",i);
			else if(i<100)
				sprintf(file_name,"0%d.bmp",i);
			else
				sprintf(file_name,"%d.bmp",i);
		
			strcpy(file_path,dir);
			strcat(file_path, file_name);

			
			if(!exists(file_path))
			{
				if(i==0){allegro_message("couldn't load %s",file_path);exit(-1);}
				else{break;}
			}
			
		
			temp_bmp_data[i].dat = load_bmp(file_path,NULL);
			if(temp_bmp_data[i].dat==NULL){allegro_message("couldn't load %s",file_path);exit(-1);}

					
			i++;
		}
		temp_bmp_data[0].num = i;
	}
	else
	{
		for(i=0;i<item_num;i++)
		{
			if(i<10)
				sprintf(file_name,"00%d.bmp",i);
			else if(i<100)
				sprintf(file_name,"0%d.bmp",i);
			else
				sprintf(file_name,"%d.bmp",i);
		
			strcpy(file_path,dir);
			strcat(file_path, file_name);

			if(!exists(file_path)){allegro_message("couldn't load %s",file_path);exit(-1);}
				

			temp_bmp_data[i].dat = load_bmp(file_path,NULL);
			if(temp_bmp_data[i].dat==NULL){allegro_message("couldn't load %s",file_path);exit(-1);}

			temp_bmp_data[i].num = item_num;
		}
	}
	
	return temp_bmp_data;
}


void unload_bmp_array(BMP_ARRAY *temp)
{
	int num = temp[0].num;
	int i;

	if(temp==NULL)return;

	for(i=0;i<num;i++)
	{
		destroy_bitmap(temp[i].dat);
	}

	free(temp);
}

///////////////////////////////////////////////////////
////// THE RLE ARRAY FUNCTIONS /////////////////////
//////////////////////////////////////////////////////


static /*int*/ void find_one_rle_file(const char *file,int attr,/*void **/int param)
{
	BITMAP *bmp;
	bmp = load_bmp(file,NULL);

	temp_rle_data[0].dat = get_rle_sprite(bmp);
	 
	destroy_bitmap(bmp);
    //return 0;
}


RLE_ARRAY* load_rle_array(char *dir_tmp,int item_num)
{
	char file_path[90];
	char file_name[20];
	BITMAP *bmp;
	int i;
	
	char dir[256];  // NOTE: Workaround; don't call put_backslash() with a string literal (which dir_tmp is).
	sprintf(dir, "%s", dir_tmp);
	
	put_backslash(dir);
	
	if(item_num==-1)
		temp_rle_data = calloc(sizeof(RLE_ARRAY), MAX_RLE_ARRAY_DATA);
	else
		temp_rle_data = calloc(sizeof(RLE_ARRAY), item_num);
	
	
	if(item_num==1)
	{
		sprintf(file_path,"%s*.bmp",dir);
		i= for_each_file/*_ex*/(file_path,FA_ARCH/*,0*/,find_one_rle_file,0);

		if(i!=1){allegro_message("couldn't load %s",file_path);exit(-1);}
		
		temp_rle_data[0].num = item_num;
	}
	else if(item_num==-1)
	{
		i=0;
		while(1)
		{
			if(i<10)
				sprintf(file_name,"00%d.bmp",i);
			else if(i<100)
				sprintf(file_name,"0%d.bmp",i);
			else
				sprintf(file_name,"%d.bmp",i);
		
			strcpy(file_path,dir);
			strcat(file_path, file_name);

			
			if(!exists(file_path))
			{
				if(i==0){allegro_message("couldn't load %s",file_path);exit(-1);}
				else{break;}
			}
			
		
			bmp = load_bmp(file_path,NULL);
			temp_rle_data[i].dat = get_rle_sprite(bmp);
			destroy_bitmap(bmp);

			if(temp_rle_data[i].dat==NULL){allegro_message("couldn't load %s",file_path);exit(-1);}

					
			i++;
		}
		temp_rle_data[0].num = i;
	}
	else
	{
		for(i=0;i<item_num;i++)
		{
			if(i<10)
				sprintf(file_name,"00%d.bmp",i);
			else if(i<100)
				sprintf(file_name,"0%d.bmp",i);
			else
				sprintf(file_name,"%d.bmp",i);
		
			strcpy(file_path,dir);
			strcat(file_path, file_name);

			if(!exists(file_path)){allegro_message("couldn't load %s",file_path);exit(-1);}
				

			bmp = load_bmp(file_path,NULL);
			temp_rle_data[i].dat = get_rle_sprite(bmp);
			destroy_bitmap(bmp);

			if(temp_rle_data[i].dat==NULL){allegro_message("couldn't load %s",file_path);exit(-1);}

			temp_rle_data[i].num = item_num;
		}
	}
	
	return temp_rle_data;
}


void unload_rle_array(RLE_ARRAY *temp)
{
	int num = temp[0].num;
	int i;

	if(temp==NULL)return;

	for(i=0;i<num;i++)
	{
		destroy_rle_sprite(temp[i].dat);
	}

	free(temp);
}
