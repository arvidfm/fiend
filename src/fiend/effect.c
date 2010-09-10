////////////////////////////////////////////////////
// This file contains the effect funcs, effects are used by 
// triggers. 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"



EFFECT_DATA effect_data[MAX_EFFECT_NUM];

//ha seffect allready faded out the scrren?
int screen_is_black=0;

//clear all the effect data
void reset_effects(void)
{
	int i;

	for(i=0;i<MAX_EFFECT_NUM;i++)
		effect_data[i].used=0;
}

//make an effect. the parameters has diffrent meaning for diffrent 
//effects.
void make_effect(int type,int num,int x, int y,int z,int time)
{
	int i;
	
	for(i=0;i<MAX_EFFECT_NUM;i++)
		if(!effect_data[i].used)break;

	effect_data[i].num = num;
	effect_data[i].x =x;
	effect_data[i].y =y;
	effect_data[i].z =z;
	effect_data[i].time =time;
	effect_data[i].type =type;
	effect_data[i].used =1;
	effect_data[i].sp1 =0;
	effect_data[i].sp2 =0;

}

//update the effect data.
void update_effects(void)
{
	int i,j;
	float temp;
	float temp_x, temp_y;
	
	
	for(i=0;i<MAX_EFFECT_NUM;i++)
		if(effect_data[i].used)
		{
			if(effect_data[i].time>-1)effect_data[i].time--;
			if(effect_data[i].time==0)effect_data[i].used=0;
						
			if(effect_data[i].type==EFFECT_OBJECT_MOVE)
			{
				if(effect_data[i].time==-1)
				{
					effect_data[i].time = distance(effect_data[i].x,effect_data[i].y, map->object[effect_data[i].num].x, map->object[effect_data[i].num].y)/((float)effect_data[i].z/100);
					temp = 	compute_angle(effect_data[i].x,effect_data[i].y, map->object[effect_data[i].num].x, map->object[effect_data[i].num].y);
					
					xyplus((float)effect_data[i].z/100, temp, &temp_x, &temp_y);
					effect_data[i].sp1 = temp_x;
					effect_data[i].sp2 = temp_y;

					effect_data[i].x = map->object[effect_data[i].num].x;
					effect_data[i].y = map->object[effect_data[i].num].y;
				}

				effect_data[i].x+=effect_data[i].sp1;
				effect_data[i].y+=effect_data[i].sp2;
				
				map->object[effect_data[i].num].x = effect_data[i].x;
				map->object[effect_data[i].num].y = effect_data[i].y;
				
			}

			if(effect_data[i].type==EFFECT_LIGHT_MOVE)
			{
				if(effect_data[i].time==-1)
				{
					effect_data[i].time = distance(effect_data[i].x,effect_data[i].y, map->light[effect_data[i].num].world_x, map->light[effect_data[i].num].world_y)/((float)effect_data[i].z/100);
					temp = 	compute_angle(effect_data[i].x,effect_data[i].y, map->light[effect_data[i].num].world_x, map->light[effect_data[i].num].world_y);
					
					xyplus((float)effect_data[i].z/100, temp, &temp_x, &temp_y);
					effect_data[i].sp1 = temp_x;
					effect_data[i].sp2 = temp_y;

					effect_data[i].x = map->light[effect_data[i].num].world_x;
					effect_data[i].y = map->light[effect_data[i].num].world_y;
				}

				effect_data[i].x+=effect_data[i].sp1;
				effect_data[i].y+=effect_data[i].sp2;
				
				map->light[effect_data[i].num].world_x = effect_data[i].x;
				map->light[effect_data[i].num].world_y = effect_data[i].y;
				
			}


			if(effect_data[i].type==EFFECT_QUAKE)
			{
				map_x+= RANDOM(-effect_data[i].z, effect_data[i].z);
				map_y+= RANDOM(-effect_data[i].z, effect_data[i].z);
			}

			if(effect_data[i].type==EFFECT_RAIN)
			{
				if(RANDOM(0,effect_data[i].x)==0)
				{
					make_new_particle("rain",RANDOM(map_x-20,map_x+520),RANDOM(map_y-20,map_y+520),0,0,0,30,1,-1);			
				}
			}
			
			if(effect_data[i].type==EFFECT_THUNDER)
			{
				effect_data[i].x++;
				
				//check if the flash comes
				if(effect_data[i].x>100 && effect_data[i].sp1==0)
					if(RANDOM(0,400)==1)
					{
						effect_data[i].sp1 = 1;
						effect_data[i].sp2 = map->light_level;
						map->light_level = 31;
						effect_data[i].x = 100;
					}

				//if flash end the flash..sp becomes 2 time for sounds....
				if(effect_data[i].x>106 && effect_data[i].sp1==1)
				{
					effect_data[i].sp1 = 2;
					map->light_level = effect_data[i].sp2;
				}

				//play the sound after about 1 second
				if(effect_data[i].x>RANDOM(170,190) && effect_data[i].sp1==2)
				{
					play_fiend_sound("thunder",0,0,0,0,220);
					effect_data[i].sp1 = 3;
				}

				//after about 5 seconds its time for a new flash..
				if(effect_data[i].x>RANDOM(490, 600) && effect_data[i].sp1==3)
				{
					effect_data[i].sp1=0;
					effect_data[i].x =0;
				}
			}

			if(effect_data[i].type==EFFECT_FADEOUT)
			{
				if(effect_data[i].sp1==0)
				{
					effect_data[i].sp2 = ((float)effect_data[i].x/480)*480;
				}
				
				if(effect_data[i].x<1)
				{
					effect_data[i].sp1=480;
				}
				else
				{
					effect_data[i].sp1+=effect_data[i].sp2;
					if(effect_data[i].sp1>480)
						effect_data[i].sp1 = 480;
				}
				
			}

			if(effect_data[i].type==EFFECT_FADEIN)
			{
				if(effect_data[i].sp1==0)
				{
					temp=-1;
					for(j=0;j<MAX_EFFECT_NUM;j++)
						if(effect_data[j].used && effect_data[j].type==EFFECT_FADEOUT)
						{
							temp = j;
							break;
						}
					if(temp==-1)
					{
						//effect_data[i].used=0;
						effect_data[i].sp2 = ((float)effect_data[i].x/480)*480;
					}
					else
					{
						effect_data[(int)temp].used=0;
						effect_data[i].sp2 = ((float)effect_data[i].x/480)*480;
					}
				}

				if(effect_data[i].x<1)
				{
					effect_data[i].sp1=480;
				}
				else
				{
					effect_data[i].sp1+=effect_data[i].sp2;
					if(effect_data[i].sp1>480)
						effect_data[i].used = 0;
				}

							

			}
							

			if(effect_data[i].type==EFFECT_LIGHTLEVEL_FADE)
			{
				if(map->light_level>effect_data[i].x)
				{
					effect_data[i].z-=effect_data[i].y;
								
					map->light_level = effect_data[i].z/100;

					if(map->light_level<effect_data[i].x)
					{
						map->light_level= effect_data[i].x;
						effect_data[i].used=0;
					}
					
					if(map->light_level<0)
						map->light_level=0;
				}
				else
				{
					effect_data[i].z+=effect_data[i].y;
								
					map->light_level = effect_data[i].z/100;

					if(map->light_level>effect_data[i].x)
					{
						map->light_level= effect_data[i].x;
						effect_data[i].used=0;
					}
					if(map->light_level>30)
						map->light_level=30;
				}
			}




		}


}




// draw effects that need to be drawn.
void draw_effects(void)
{
	int i;
	float j,k;
	float line_space;
	
	for(i=0;i<MAX_EFFECT_NUM;i++)
		if(effect_data[i].used)
		{
	
			if(effect_data[i].type==EFFECT_FADEOUT)
			{
				line_space = 11-((effect_data[i].sp1/480)*10);
				
				/*for(j=0;j<480;j+=line_space)
				{
					hline(virt,0,j,effect_data[i].sp1,0);
				}
				for(j=1;j<480;j+=line_space)
				{
					hline(virt,480,j,480-effect_data[i].sp1,0);
				}*/
				for(j=0;j<480;j+=3)
					for(k=0;k<480;k+=3)
						putpixel(virt,j,k,0);


			}
			
			if(effect_data[i].type==EFFECT_FADEIN)
			{
				line_space = ((effect_data[i].sp1/480)*10)+1;
				
				for(j=0;j<480;j+=line_space)
				{
					hline(virt,0,j,480-effect_data[i].sp1,0);
				}
				for(j=1;j<480;j+=line_space)
				{
					hline(virt,480,j,effect_data[i].sp1,0);
				}

			}	



		}
}



//fadeout/fadein funcs
void fiend_fadeout(int speed)
{
	BITMAP *bmp;
	int i;
	
	bmp = create_bitmap(480,480);

	blit(virt,bmp,0,0,0,0,480,480);

	for(i=0;i<256;i+=speed)
	{
		set_trans_blender(0,0,0,0);
		draw_lit_sprite(virt,bmp,0,0,i);
		vsync();
		blit(virt,screen,0,0,80,0,480,480);
	}
	screen_is_black=1;

	destroy_bitmap(bmp);
	speed_counter=0;

}

void fiend_fadein(int speed)
{
	BITMAP *bmp;
	int i;
	
	bmp = create_bitmap(480,480);

	screen_is_black=1;
	
	map_x = player.x - 240;
	map_y = player.y - 240;
	draw_level();

	blit(virt,bmp,0,0,0,0,480,480);
	
	for(i=256;i>0;i-=speed)
	{
		set_trans_blender(0,0,0,0);
		draw_lit_sprite(virt,bmp,0,0,i);
		vsync();
		blit(virt,screen,0,0,80,0,480,480);
	}
	screen_is_black=0;

	destroy_bitmap(bmp);
	speed_counter=0;

}