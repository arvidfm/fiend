////////////////////////////////////////////////////
// This file contains particle drawing ,animation 
// updating,loading and making
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>

#include <stdio.h>
#include <string.h>

#include "../fiend.h"
#include "../draw.h"
#include "../grafik4.h"


PARTICLE_INFO *particle_info;
PARTICLE_DATA *particle_data;

int num_of_particles;


void reset_particles(void)
{
	int i;

	for(i=0;i<MAX_PARTICLE_DATA;i++)
		particle_data[i].used=0;

}


void make_new_particle(char *name, float x, float y, float angle, float speed,float speed_dec, int time, int level, int color)
{
	int i;
	int type=-1;
	int blood=0;

	for(i=0;i<num_of_particles;i++)
		if(strcmp(name, particle_info[i].name)==0)
		{
			type = i;
			break;
		}
	
	if(type<0 && strcmp(name,"blood")==0)
	{
		blood=1;
		type =0;
	}else if(type<0 && strcmp(name,"blood_child")==0)
	{
		blood=2;
		type =0;
	}
	else if(type<0)
		return;

	for(i=0;i<MAX_PARTICLE_DATA;i++)
		if(!particle_data[i].used)
			break;
	
	if(i>MAX_PARTICLE_DATA-1)
		i = RANDOM(0,MAX_PARTICLE_DATA-1);

	particle_data[i].blood = 0;
	
	if(blood)
		particle_data[i].blood = blood;
	
	particle_data[i].used = 1;
	particle_data[i].type = type;
	
	particle_data[i].x = x;
	particle_data[i].y = y;
	particle_data[i].angle = angle;
	particle_data[i].speed = speed;
	particle_data[i].speed_dec = speed_dec;


	particle_data[i].time = time;
	particle_data[i].level = level;
	particle_data[i].frame = 0;
	particle_data[i].next_frame = 0;

	particle_data[i].alpha = 255;
	particle_data[i].color = color;

}



void update_particles(void)
{
	int i;
	int type;
	float temp_x, temp_y;

	for(i=0;i<MAX_PARTICLE_DATA;i++)
		if(particle_data[i].used)
		{
			type = particle_data[i].type;

			xyplus(particle_data[i].speed, particle_data[i].angle, &temp_x, &temp_y);
			
			particle_data[i].x+=temp_x;
			particle_data[i].y+=temp_y;

			particle_data[i].speed-=particle_data[i].speed_dec;
			if(particle_data[i].speed<0)particle_data[i].speed=0;

			if(particle_data[i].blood==1 && RANDOM(1,13)==1)
			{
				make_new_particle("blood_child",particle_data[i].x, particle_data[i].y,0,0,0, 20,2,particle_data[i].color);
			}

			
			if(!particle_data[i].blood)
			{
				particle_data[i].next_frame++;
				if(particle_data[i].next_frame>=ANIM_SPEED)
				{
					//update frame	
					particle_data[i].next_frame =0;
					particle_data[i].frame++;
				
					if(particle_info[type].anim_frame[particle_data[i].frame]==255)
						particle_data[i].frame = particle_info[type].anim_frame[particle_data[i].frame+1];
				}
			}

			if(particle_data[i].time>-1)
			{
				particle_data[i].time--;
				if(particle_data[i].time<1)
				{
					particle_data[i].used=0;
					particle_data[i].blood=0;
				}
			}
		}
}



void draw_aa_to_masked(BITMAP *dest, BITMAP *src, int x, int y)
{
	blit(src,temp_bitmap, 0,0, 100 - src->w/2, 100 - src->h/2, src->w, src->h);

	masked_blit(temp_bitmap, dest, 100 - src->w/2, 100 - src->w/2, x,y, src->w,src->h); 
}

void draw_particles(int level)
{
	int i,type;
	int pic_num;

	//drawing_mode(DRAW_MODE_TRANS,NULL,0,0);

	for(i=0;i<MAX_PARTICLE_DATA;i++)
		if(particle_data[i].used && particle_data[i].level==level)
		{
			type = particle_data[i].type;
			pic_num = particle_info[type].anim_frame[particle_data[i].frame];
						
			if(object_is_in_player_los(particle_data[i].x,particle_data[i].y,get_bitmap_w(particle_info[type].pic[pic_num].dat),get_bitmap_h(particle_info[type].pic[pic_num].dat),0,0)) 
			{
				
				if(particle_data[i].blood)
				{
					//set_trans_blender(0,0,0,128);
					putpixel(virt,particle_data[i].x-map_x,particle_data[i].y-map_y,particle_data[i].color);
					//circlefill(virt,particle_data[i].x,particle_data[i].y,10,makecol(200,0,0));
				}
				else if(particle_info[type].aa)
				{
					draw_additive_sprite(virt,particle_info[type].pic[pic_num].dat, 
						particle_data[i].x - get_bitmap_w(particle_info[type].pic[pic_num].dat)/2-map_x,
						particle_data[i].y - get_bitmap_h(particle_info[type].pic[pic_num].dat)/2-map_y);
				}
				else if(particle_info[type].trans)
				{
					set_trans_blender(0,0,0,particle_info[type].trans_alpha);
					draw_trans_sprite(virt,particle_info[type].pic[pic_num].dat, 
						particle_data[i].x - get_bitmap_w(particle_info[type].pic[pic_num].dat)/2-map_x,
						particle_data[i].y - get_bitmap_h(particle_info[type].pic[pic_num].dat)/2-map_y);
				}	
				else if(particle_info[type].rotate)
				{
					rotate_sprite(virt,particle_info[type].pic[pic_num].dat, 
						particle_data[i].x - get_bitmap_w(particle_info[type].pic[pic_num].dat)/2-map_x,
						particle_data[i].y - get_bitmap_h(particle_info[type].pic[pic_num].dat)/2-map_y,
						degree_to_fixed(particle_data[i].angle));
				}
				else 
				{
				draw_sprite(virt,particle_info[type].pic[pic_num].dat, 
					particle_data[i].x - get_bitmap_w(particle_info[type].pic[pic_num].dat)/2-map_x,
					particle_data[i].y - get_bitmap_h(particle_info[type].pic[pic_num].dat)/2-map_y);
				}


			}
		}

	//drawing_mode(DRAW_MODE_SOLID,NULL,0,0);
}




//Load particle graphics
int load_particles(void)
{
	
	FILE *f;

	char buffer[50];
	char sprite_file_name[100][20];
    char info_file_name[100][20];
	char *file_path ="graphic\\particles\\";
	char final_path[40];
	int i,k;
	
	particle_info = calloc(sizeof(PARTICLE_INFO),MAX_PARTICLE_INFO);
	
	particle_data = calloc(sizeof(PARTICLE_DATA),MAX_PARTICLE_DATA);
	for(i=0;i<MAX_PARTICLE_DATA;i++)
		particle_data[i].used=0;
	
	f = fopen("graphic\\particles\\particles.txt", "r");      //Load the Info file
	
	if(f==NULL)
	{sprintf(fiend_errorcode,"couldn't load particles.txt");return 1;} //error testing...
	
	
	while(fscanf(f, "%s %s\n", sprite_file_name[num_of_particles], info_file_name[num_of_particles])!=EOF)
	{
		num_of_particles++;	
	}

	fclose(f);

	for(i=0;i<num_of_particles;i++)
	{
		
		//----------BEGIN GET INFO-----------------------
		strcpy(final_path, file_path);
		strcat(final_path, info_file_name[i]);    //Get the info.

		f = fopen(final_path, "r");
		if(f==NULL){sprintf(fiend_errorcode,"couldn't load %s",final_path); return 1;}//error testing...

		
		fscanf(f,"%s %d",buffer,&particle_info[i].num_of_frames);//num of frames
				
				  
		fscanf(f,"%s %s\n",buffer, particle_info[i].name);//get the name
		
		fscanf(f,"%s %d\n",buffer, &particle_info[i].aa);
		fscanf(f,"%s %d\n",buffer, &particle_info[i].trans);
		fscanf(f,"%s %d\n",buffer, &particle_info[i].trans_alpha);
		fscanf(f,"%s %d\n",buffer, &particle_info[i].rotate);

	
		k=-1;
		do
		{
			k++;
			fscanf(f,"%d ",&particle_info[i].anim_frame[k],buffer);
		}while(particle_info[i].anim_frame[k]!=-1);
			
		
		fclose(f);
		//-------END GET INFO---------------

		
		
		//--------BEGIN GET DATA------------------
		strcpy(final_path, file_path);
		strcat(final_path, sprite_file_name[i]);   //Get the tile data .

		set_color_conversion(COLORCONV_TOTAL);

		particle_info[i].pic = load_bmp_array(final_path,particle_info[i].num_of_frames);//load the graphic
		if(particle_info[i].pic==NULL){ sprintf(fiend_errorcode,"couldn't load %s",final_path); return 1;}//error testing...
		
			
			
	}
	

	
	return 0;
}


void release_particles(void)
{
	int i;

	for(i=0;i<num_of_particles;i++)
		unload_bmp_array(particle_info[i].pic);

	free(particle_info);
	free(particle_data);


}

