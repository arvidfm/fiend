////////////////////////////////////////////////////
// This file contains misc funcs if is not any other file then it is here...
// that usually includes smaller funcs and data hanfling creating
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


#include "draw.h"
#include "fiend.h"
#include "grafik4.h"
#include "picdata.h"
#include "draw_polygon.h"



///////////////////////////////////////////////
///////// LOOK ATT AREA CHECK //////////////
///////////////////////////////////////////
void check_look_at_areas(void)
{
	int i;

	if(!player_has_used)
		return;
	
	for(i=0;i<map->num_of_look_at_areas;i++)
		if(player_has_used && map->look_at_area[i].active &&
			check_collision(player_use_x - PLAYER_USE_W/2, player_use_y - PLAYER_USE_W/2, PLAYER_USE_W, PLAYER_USE_H,
			map->look_at_area[i].x - map->look_at_area[i].w/2, map->look_at_area[i].y - map->look_at_area[i].h/2, map->look_at_area[i].w, map->look_at_area[i].h))
		{
			make_new_message(map->look_at_area[i].message, NULL);
			break;
		}

}


///////////////////////////////////////////////
///////// ENGINE ERROR //////////////
///////////////////////////////////////////
#define ENGINE_ERROR_MAX 50
int engine_error_num=0;

typedef struct
{
	char string[50];
	int time;
}ENGINE_ERROR_DATA;

ENGINE_ERROR_DATA engine_error[ENGINE_ERROR_MAX];

void make_engine_error(char *string)
{
	int i;
	char temp_string[ENGINE_ERROR_MAX][50];
	int temp_time[ENGINE_ERROR_MAX];

	if(engine_error_num>=ENGINE_ERROR_MAX-1)
		return;
	
	for(i=0;i<engine_error_num;i++)
	{
		sprintf(temp_string[i],"%s",engine_error[i].string);
		temp_time[i] = engine_error[i].time;
	}
	
	for(i=0;i<engine_error_num;i++)
	{
		sprintf(engine_error[i+1].string,"%s",temp_string[i]);
		engine_error[i+1].time=temp_time[i]; 
	}
	
	
	sprintf(engine_error[0].string,"%s",string);
	engine_error[0].time = 600;
	engine_error_num++;	
}

void update_engine_error(void)
{
	int i;

	for(i=0;i<engine_error_num;i++)
	{
		engine_error[i].time--;
		if(engine_error[i].time<0)
			engine_error_num--;
	}
}

void draw_engine_error(void)
{
	int i;

	//text_mode(-1);
	for(i=0;i<engine_error_num;i++)
	{
		textprintf_ex(virt,font_small1->dat,0,20+((engine_error_num-1)*10)-(i*10),makecol(255,255,255),-1,"%s",engine_error[i].string);		
	}
}



///////////////////////////////////////////////
///////// PICKUP MESSAGE //////////////
///////////////////////////////////////////
#define PICKUP_MESSAGE_FADE 80
#define PICKUP_MESSAGE_ALPHA 200


typedef struct
{
	int ready;
	int time;
	int alpha;

	BITMAP *pic;

	BITMAP *buffer;
}PICKUP_DATA;

static PICKUP_DATA pickup={0,0,0,NULL,NULL};


void make_pickup_message(char *string,BITMAP *pic,int time)
{
	int i,j;
	int color;
	int val;
	int r,g,b;

	if(pickup.buffer==NULL)
	{
		pickup.buffer = create_bitmap(480, 50);
	}
	
	clear_to_color(pickup.buffer, makecol(255,0,255));

	//text_mode(-1);
	textprintf_centre_ex(pickup.buffer, font_avalon->dat,241,1,makecol(0,0,0),-1,"%s",string);
	textprintf_centre_ex(pickup.buffer, font_avalon->dat,240,0,makecol(255,255,255),-1,"%s",string);
	
	pickup.ready = 1;
	
	pickup.alpha = PICKUP_MESSAGE_ALPHA;
	pickup.time = time;

	if(pic!=NULL)
	{
		if(pickup.pic!=NULL) destroy_bitmap(pickup.pic);
		
		pickup.pic = create_bitmap(pic->w,pic->h);
		clear_to_color(pickup.pic,makecol(255,0,255));
		
		for(i=0;i<pic->w;i++)
			for(j=0;j<pic->h;j++)
			{
				color = getpixel(pic,i,j);
				
				r = getr(color);g = getg(color);b = getb(color);
				if(color==MASK_COLOR_16)
				{
				}
				else
				{
					val = ((r+g+b)/3);
					putpixel(pickup.pic,i,j,makecol(val,val,val));
				}
			}
	}
	else
	{
		pickup.pic = NULL;
	}
}


void update_pickup_message(void)
{
	if(pickup.ready)
	{
		pickup.time--;

		if(pickup.time<=PICKUP_MESSAGE_FADE)
		{
			pickup.alpha = PICKUP_MESSAGE_ALPHA * ((float)pickup.time/PICKUP_MESSAGE_FADE);
		}

		if(pickup.time<1)
			pickup.ready=0;
	}
}



void draw_pickup_message(void)
{
	if(pickup.ready)
	{
		if(pickup.alpha==255)
		{
			if(pickup.pic!=NULL){
				set_trans_blender(0,0,0,158);
				draw_trans_sprite(virt, pickup.pic, 240-pickup.pic->w/2,240-120);}

			draw_sprite(virt, pickup.buffer, 0, 145);
						
		}
		else
		{
			if(pickup.pic!=NULL){
				set_trans_blender(0,0,0,((float)pickup.alpha/200)*158);
				draw_trans_sprite(virt, pickup.pic, 240-pickup.pic->w/2,240-120);}
			
			set_trans_blender(0,0,0,pickup.alpha);
			draw_trans_sprite(virt, pickup.buffer, 0, 145);
			
		}
	}
}



///////////////////////////////////////////////
///////// BEAMS //////////////
/////////////////////////////////////////////

BEAM beam[BEAM_NUM];

void reset_beams(void)
{
	int i;

	for(i=0;i<BEAM_NUM;i++)
		beam[i].used=0;
}


void make_new_beam(char *name,int x1,int y1, int x2, int y2, int time)
{
	int i;
	int type=-1;

	for(i=0;i<num_of_particles;i++)
		if(strcmp(name, particle_info[i].name)==0)
		{
			type = i;
			break;
		}
	
	if(type<0)return;

	for(i=0;i<BEAM_NUM;i++)
		if(!beam[i].used)
			break;

	
	beam[i].type = type;
	beam[i].used = 1;
	
	beam[i].x1 = x1;
	beam[i].y1 = y1;
	beam[i].x2 = x2;
	beam[i].y2 = y2;

	beam[i].frame = 0;
	beam[i].next_frame = 0;

	beam[i].time = time;
}


void update_beams(void)
{
	int i;
	int type;

	for(i=0;i<BEAM_NUM;i++)
		if(beam[i].used)
		{
			beam[i].next_frame++;
			if(beam[i].next_frame>=ANIM_SPEED)
			{
				type = beam[i].type;

				beam[i].next_frame =0;
				beam[i].frame++;
				
				if(particle_info[type].anim_frame[beam[i].frame]==255)
					beam[i].frame = particle_info[type].anim_frame[beam[i].frame+1];
			}

			
			beam[i].time--;
			if(beam[i].time<0)beam[i].used=0;
		}
}



void draw_fiend_beam(BITMAP *dest, BITMAP *img, int x1, int y1,int x2, int y2)
{
	int i;

	V3D_f point[4];
	V3D_f temp_point[2];

	VERTEX the_point[4];
	
	float angle;

	angle = compute_angle(x2,y2,x1,y1);

	// set points of a point...
	temp_point[0].y = 0;
	temp_point[0].x = -img->w/2;
	temp_point[1].y = 0;
	temp_point[1].x = img->w/2;

	//rotate the points
	for(i=0;i<2;i++)
	{
		point[i].x = (temp_point[i].x*COS(angle)-temp_point[i].y*SIN(angle));
		point[i].y = (temp_point[i].y*COS(angle)+temp_point[i].x*SIN(angle));
	}

	//give the other point thses cords...
	point[2].x = point[0].x;
	point[2].y = point[0].y;
	point[3].x = point[1].x;
	point[3].y = point[1].y;

	//transform to world space..
	point[0].x += x1;
	point[0].y += y1;
	point[1].x += x1;
	point[1].y += y1;
	point[2].x += x2;
	point[2].y += y2;
	point[3].x += x2;
	point[3].y += y2;

	//set the u,v cords (texture cords)
	for(i=0;i<4;i++)
		point[i].c = makecol(255,0,0);

	point[0].u = 0;
	point[0].v = 0;
	point[1].u = img->w;
	point[1].v = 0;
	point[2].u = 0;
	point[2].v = distance(point[0].x,point[0].y,point[2].x,point[2].y );
	point[3].u = img->w;
	point[3].v = distance(point[1].x,point[1].y,point[3].x,point[3].y );

	for(i=0;i<4;i++)
	{
		the_point[i].x = point[i].x;
		the_point[i].y = point[i].y;
		the_point[i].u = point[i].u;
		the_point[i].v = point[i].v;
	}


	quad3d_f(dest,POLYTYPE_ATEX_MASK,img, &point[0], &point[1], &point[3],&point[2] );

	//fiend_quad3d(dest,img, &the_point[0], &the_point[1], &the_point[3],&the_point[2] );

	
	//for(i=0;i<4;i++)
		//circlefill(dest,point[i].x,point[i].y,3,makecol(255,0,255));
}



void draw_beams(void)
{
	int i;
	int type;
	int pic_num;

	for(i=0;i<BEAM_NUM;i++)
		if(beam[i].used)
		{
			type = beam[i].type;
			pic_num = particle_info[type].anim_frame[beam[i].frame];

			draw_fiend_beam(virt, particle_info[type].pic[pic_num].dat,beam[i].x1-map_x, beam[i].y1-map_y,beam[i].x2-map_x, beam[i].y2-map_y); 			
		}
}



///////////////////////////////////////////////
///////// MISSILE HITS AND BLOOD //////////////
/////////////////////////////////////////////

BLOODPOOL bloodpool[BLOODPOOL_NUM];



void make_bullet_hit_object(int i,int power)
{
	//int amount,j;
	int x_add, y_add;

	x_add = RANDOM(-5,5);
	y_add = RANDOM(-5,5);

	make_new_particle("dust",missile_data[i].x+x_add, missile_data[i].y+y_add,0,0,0, 60,2,0);
}


void make_bullet_hit_flesh(int i,int power, int color)
{
	int amount,j;
	int x_add=0, y_add=0;
	
	amount = RANDOM(15,27);
	for(j=0;j<amount;j++)
	{

		x_add = RANDOM(-3,3);
		y_add = RANDOM(-3,3);
		make_new_particle("blood",missile_data[i].x+x_add, missile_data[i].y+y_add, add_angle(missile_data[i].angle,RANDOM(150,210) )
			,(float)(RANDOM(5,12))/10,0.001, RANDOM(40,60),2,color);

	}
	
}


#define BLOOD_SPEED 0.03
#define BLOOD_COL 160

void reset_bloodpools(void)
{
	int i;

	for(i=0;i<BLOODPOOL_NUM;i++)
		bloodpool[i].used=0;
}

void make_bloodcircle(int x, int y, int size, int color)
{
	int i;

	for(i=0;i<BLOODPOOL_NUM;i++)
		if(!bloodpool[i].used) break;
	
	bloodpool[i].x = x	;
	bloodpool[i].y = y;
	bloodpool[i].max_size = size;
	bloodpool[i].size = size/4;
	bloodpool[i].speed = BLOOD_SPEED;
	
	bloodpool[i].color = color;

	bloodpool[i].used=1;
}


void make_bloodpool(int x, int y, int size, int color)
{
	int i;
	int temp = RANDOM(5,8);

	for(i=0;i<temp;i++)
	{
		make_bloodcircle(x +RANDOM(-size/2, size/2), y +RANDOM(-size/2, size/2), RANDOM(size/4, size/2),color);
	}
}

void update_bloodpools(void)
{
	int i;

	for(i=0;i<BLOODPOOL_NUM;i++)
		if(bloodpool[i].used)
		{
			if(bloodpool[i].size<bloodpool[i].max_size) 
			{
				bloodpool[i].size+=BLOOD_SPEED;
							
			}
		}
}


void draw_bloodpools(void)
{
	int i;
	float j;
	float dec;
	float alpha;
	float temp;

	for(i=0;i<BLOODPOOL_NUM;i++)
		if(bloodpool[i].used)
			if(object_is_in_player_los(bloodpool[i].x-bloodpool[i].size, bloodpool[i].y-bloodpool[i].size, bloodpool[i].size*2,bloodpool[i].size*2,0,0))
			{
				temp = bloodpool[i].size*0.5;
				
				circlefill(virt, bloodpool[i].x-map_x, bloodpool[i].y-map_y, temp*2, bloodpool[i].color );

				dec = (255)/(bloodpool[i].size*0.5);
				alpha = 255;

			 
				drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
				for(j=temp;j<bloodpool[i].size;j+=1)
				{
					set_trans_blender(0,0,0,(int)alpha);
					circle(virt, bloodpool[i].x-map_x, bloodpool[i].y-map_y, temp+j, bloodpool[i].color);
					alpha-=dec;
					if(alpha<0)alpha=0;
				}
				drawing_mode(DRAW_MODE_SOLID,NULL,0,0);
			}
}


void make_flesh_explosion(int x, int y, int size, int angle)
{
	/*int amount,j;
	int x_add,y_add;
	char stain[4][20]={"bloodstain1","bloodstain2","bloodstain3","bloodstain4"};
	char part[4][20]={"bodypart1","bodypart2","bodypart3","bodypart4"};

	amount = RANDOM(6,10);
	for(j=0;j<amount;j++)
	{
		x_add = RANDOM(-17,17);
		y_add = RANDOM(-17,17);
		make_new_particle(stain[RANDOM(0,3)],x+x_add, y+y_add, 
						add_angle(angle,RANDOM(-120,120)),0, 0, -1,0);
	}
	amount = RANDOM(7,16);
	for(j=0;j<amount;j++)
	{
		x_add = RANDOM(-5,5);
		y_add = RANDOM(-5,5);
		make_new_particle("blood2",x+x_add, y+y_add, 
						add_angle(angle,RANDOM(-120,120)),(float)(RANDOM(90,140))/100,
						0.023, -1,0);
	}
	amount = RANDOM(14,24);
	for(j=0;j<amount;j++)
	{
		x_add = RANDOM(-5,5);
		y_add = RANDOM(-5,5);
		make_new_particle("blood1",x+x_add, y+y_add, 
						RANDOM((0),359),(float)(RANDOM(150,190))/100,
						0.023, RANDOM(40,130),1);
	}
	amount = RANDOM(3,6);
	for(j=0;j<amount;j++)
	{
		x_add = RANDOM(-15,15);
		y_add = RANDOM(-15,15);
		make_new_particle(part[RANDOM(0,4)],x+x_add, y+y_add, 
						add_angle(angle,RANDOM(-90,90)),(float)(RANDOM(100,140))/100,
						0.023, -1,0);
	}*/
	

}


/////////////////////////////////////////////////
////////// FLAMES //////////////////////////////
/////////////////////////////////////////////////

BMP_ARRAY *flame_pic;

FLAMEDATA flame_data[MAX_FLAME_DATA];

void reset_flames(void)
{
	int i;

	for(i=0;i<MAX_FLAME_DATA;i++)
		flame_data[i].used=0;

}

void make_flame(int type, int x, int y, int time)
{
	int i;

	if(type<0)return;

	for(i=0;i<MAX_FLAME_DATA;i++)
		if(!flame_data[i].used)
			break;

	flame_data[i].used = 1;
	flame_data[i].type = type;

	flame_data[i].x = x;
	flame_data[i].y = y;
	flame_data[i].time = time;
	flame_data[i].num = 0;
}


void update_flames(void)
{
	int i;

	for(i=0;i<MAX_FLAME_DATA;i++)
		if(flame_data[i].used)
		{	
			if(flame_data[i].time>-1)
				flame_data[i].time--;

			if(flame_data[i].time<1)
				flame_data[i].used=0;
		}
}


void draw_flames(void)
{
	int i;
	int type;

	for(i=0;i<MAX_FLAME_DATA;i++)
		if(flame_data[i].used)
		{	
			type = flame_data[i].type;
			
			if(object_is_in_player_los(flame_data[i].x,flame_data[i].y,get_bitmap_w(flame_pic[type].dat),get_bitmap_h(flame_pic[type].dat),0,0)) 
			{
				//set_alpha_blender();
				draw_additive_sprite(virt,flame_pic[type].dat, 
					flame_data[i].x - get_bitmap_w(flame_pic[type].dat)/2-map_x,
					flame_data[i].y - get_bitmap_h(flame_pic[type].dat)/2-map_y);
			}
		}
}



int load_flames(void)
{	
	
	flame_pic = load_bmp_array("graphic/misc/flames",-1);
	
	if(flame_pic==NULL)
	{sprintf(fiend_errorcode,"couldn't load flames/");return 1;}

	return 0;
}

void release_flames(void)
{
	unload_bmp_array(flame_pic);
}


/////////////////////////////////////////////////
////////// SHELLS //////////////////////////////
/////////////////////////////////////////////////


SHELL_DATA shell_data[MAX_SHELL_NUM];



void reset_shells(void)
{
	int i;

	for(i=0;i<MAX_SHELL_NUM;i++)
		shell_data[i].used=0;

}


void make_shell(char *name, float x, float y, float z, float angle,int dir, float speed,float max_z,int time)
{
	int i;
	int type=-1;

	for(i=0;i<num_of_particles;i++)
		if(strcmp(name, particle_info[i].name)==0)
		{
			type = i;
			break;
		}
				
	if(type<0)return;
	
	for(i=0;i<MAX_SHELL_NUM;i++)
		if(!shell_data[i].used)
			break;
	

	shell_data[i].used=1;
	
	shell_data[i].type=type;

	shell_data[i].x=x;
	shell_data[i].y=y;
	shell_data[i].z=z;//
	
	shell_data[i].max_z=max_z;//
	
	shell_data[i].dir=1;
	
	shell_data[i].angle=angle;
	shell_data[i].speed=speed;//;
	shell_data[i].time=190;
}



int shell_collides(float x, float y)
{
	int i;

	for(i=0;i<map->num_of_objects;i++)
	{
		if(map->object[i].active)
			if(object_info[map->object[i].type].solid>0 && object_info[map->object[i].type].animation[map->object[i].action].solid>0)
				if( check_angle_collision(x, y, 2,2, 0,
					map->object[i].x, map->object[i].y, object_info[map->object[i].type].w,object_info[map->object[i].type].h,  ((int)(map->object[i].angle*((float)object_info[map->object[i].type].angles/360) )) *  ((float)360/object_info[map->object[i].type].angles)   ) )
						return 1;

	}
	

	if(check_tile_collision(x,y,2,2))return 1;

	return 0;
}


void update_shells(void)
{
	int i;
	int type;
	float temp_x, temp_y;

	for(i=0;i<MAX_SHELL_NUM;i++)
		if(shell_data[i].used)
		{
			type = shell_data[i].type;

			xyplus(shell_data[i].speed, shell_data[i].angle, &temp_x, &temp_y);
			
				
			/// The bouncing up and down
			if(shell_data[i].dir==0)
			{
				shell_data[i].z-=0.05;
				if(shell_data[i].z<0.5)
				{
					shell_data[i].z=0.5;
					shell_data[i].dir=1;
					shell_data[i].max_z-=0.2;
					
					if(shell_data[i].max_z<0)shell_data[i].max_z=0;
					
					if(shell_data[i].max_z>0.6)
						play_fiend_sound(particle_info[type].name,shell_data[i].x,shell_data[i].y, 1,0,100);

					
				}
			}

			if(shell_data[i].dir==1)
			{
				shell_data[i].z+=0.05;
				if(shell_data[i].z>shell_data[i].max_z)
				{
					shell_data[i].z=shell_data[i].max_z;
					shell_data[i].dir=0;
				}
			}
			
			//The speed
			shell_data[i].speed-=0.01;
			if(shell_data[i].speed<0)shell_data[i].speed=0;


			
			//The bouncing on walls and stuff
			shell_data[i].x+=temp_x;
			shell_data[i].y+=temp_y;

			if(shell_collides(shell_data[i].x, shell_data[i].y))
			{
				play_fiend_sound(particle_info[type].name,shell_data[i].x,shell_data[i].y, 1,0,100); 
				
				shell_data[i].x-=temp_x;
				shell_data[i].y-=temp_y;
				
				shell_data[i].angle=add_angle(shell_data[i].angle, 180);

			}
			
			
			//The life time
			shell_data[i].time--;
			if(shell_data[i].time<1)shell_data[i].used=0;
		}
	
}




void draw_shells(void)
{
	int i,type;
	float scale =1;
	
	for(i=0;i<MAX_SHELL_NUM;i++)
		if(shell_data[i].used)
		{
			type = shell_data[i].type;

			if(object_is_in_player_los(shell_data[i].x,shell_data[i].y,get_bitmap_w(particle_info[type].pic[0].dat),get_bitmap_h(particle_info[type].pic[0].dat),0,0)) 
			{
				rotate_scaled_sprite(virt,particle_info[type].pic[0].dat, 
					shell_data[i].x - get_bitmap_w(particle_info[type].pic[0].dat)/2-map_x,
					shell_data[i].y - get_bitmap_h(particle_info[type].pic[0].dat)/2-map_y,
					degree_to_fixed(shell_data[i].angle),
					degree_to_fixed(shell_data[i].z));
			}
		}

}






/////////////////////////////////////////////////
////////// CONFIG FILE //////////////////////////
/////////////////////////////////////////////////

struct
{
	int vsync;
	int color_depth;
	int gfx_driver;
	int lightning;
		
	int music_volume;
	int sound_volume;
	int sound_buffer_size;
	int sound_driver;
	int sound_is_on;

	int key_forward;
	int key_backward;
	int key_left;
	int key_right;
	int key_attack;
	int key_action;
	int key_pickup;
	int key_inventory;
}config_file;
	
void load_config_file(void)
{
	FILE *f;
	
	f = fopen("config.dat","rb");
	if(f==NULL)return;

	
	fread(&config_file,sizeof(config_file),1,f);
	
	fclose(f);

	vsync_is_on = config_file.vsync;
	color_depth = config_file.color_depth;
	fiend_gfx_driver = config_file.gfx_driver;
	lightning_is_on =  config_file.lightning;

	fiend_music_volume = config_file.music_volume;
	fiend_sound_volume = config_file.sound_volume;
	fiend_sound_buffer_size = config_file.sound_buffer_size;
	fiend_sound_driver = config_file.sound_driver;
	sound_is_on = config_file.sound_is_on;

	key_forward = config_file.key_forward;
	key_backward = config_file.key_backward;;
	key_left = config_file.key_left;
	key_right = config_file.key_right;;
	key_attack = config_file.key_attack;
	key_action = config_file.key_action;;
	key_pickup = config_file.key_pickup;;
	key_inventory = config_file.key_inventory;

}

void save_config_file(void)
{
	FILE *f;
	
	char file[90] = "save/config.dat";

	
	f = fopen("config.dat", "wb");
	if(f==NULL)return;

	
	config_file.vsync = vsync_is_on;
	config_file.color_depth = color_depth;
	config_file.gfx_driver = fiend_gfx_driver;
	config_file.lightning = lightning_is_on;

	config_file.music_volume = fiend_music_volume;
	config_file.sound_volume = fiend_sound_volume;
	config_file.sound_buffer_size = fiend_sound_buffer_size;
	config_file.sound_driver = fiend_sound_driver;
	config_file.sound_is_on = sound_is_on;

	config_file.key_forward = key_forward;
	config_file.key_backward = key_backward;
	config_file.key_left = key_left;
	config_file.key_right = key_right;
	config_file.key_attack = key_attack;
	config_file.key_action = key_action;
	config_file.key_pickup = key_pickup;
	config_file.key_inventory = key_inventory;

	fwrite(&config_file,sizeof(config_file),1,f);
	
	fclose(f);
}




/*void save_the_config_file(void)
{
	char file[90] = "config.dat";
	char file1[90] = "config.dat";
	char file2[90] = "config.dat";
	
	FILE *f;

	strcpy(file,"bocka");

	strcpy(file2," agaga ");

	//allegro_message("%s : %s : %s hahaha haha", the_bajs_file, file1, file2);
	
	f = fopen(the_bajs_file, "wb");
	if(f==NULL)return;

	config_file.vsync = vsync_is_on;
	config_file.color_depth = color_depth;
	config_file.gfx_driver = fiend_gfx_driver;
	config_file.lightning = lightning_is_on;

	config_file.music_volume = fiend_music_volume;
	config_file.sound_volume = fiend_sound_volume;
	config_file.sound_buffer_size = fiend_sound_buffer_size;
	config_file.sound_driver = fiend_sound_driver;
	config_file.sound_is_on = sound_is_on;

	config_file.key_forward = key_forward;
	config_file.key_backward = key_backward;
	config_file.key_left = key_left;
	config_file.key_right = key_right;
	config_file.key_attack = key_attack;
	config_file.key_action = key_action;
	config_file.key_pickup = key_pickup;
	config_file.key_inventory = key_inventory;

	fwrite(&config_file,sizeof(config_file),1,f);
	
	fclose(f);
}*/


/////////////////////////////////////////////////
////////// OTHER //////////////////////////////
/////////////////////////////////////////////////


volatile int frame_counter = 0;
static int num_of_frames=0;
static double fps=0;


void increment_frame_counter(void)
{
	frame_counter++;
}
END_OF_FUNCTION(increment_speed_counter);


void init_frame_speed()
{
	LOCK_VARIABLE(frame_counter);
	LOCK_FUNCTION(increment_frame_counter);

    if(install_int_ex(increment_frame_counter, BPS_TO_TIMER(1000))<0)return;
}

void reset_frame_speed(void)
{
	frame_counter=0;
}


double show_frame_speed(void)
{
    num_of_frames++;
	
	if(frame_counter>1000)
	{
		fps = num_of_frames/((float)frame_counter/1000);
		frame_counter =0;
		num_of_frames=0;
	}
	return fps;
	
}


void update_load_text(char *string)
{
	//text_mode(0);
	textprintf_ex(screen,font_small1->dat,0,465,makecol(255,255,255),0,"%s          ",string);
}


void get_args(int argc, char *argv[])
{
	int i;
	char temp[50];
	char temp2[50];

	for(i=1;i<argc;i++)
		if(argv[i][0] == '-' || argv[i][0] == '/')
		{
			strncpy(temp, argv[i]+1, strlen(argv[i]));
						
			if(strcasecmp(temp,"debug")==0)
			{
				debug_is_on =1;
			}
			else if(strcasecmp(temp,"vsync")==0)
			{
				vsync_is_on =1;
			}
			else if(strcasecmp(temp,"nosound")==0)
			{
				sound_is_on =0;
			}
			else if(strcasecmp(temp,"sounddriver")==0)
			{
				if(i+1<argc)
				{
					fiend_sound_driver = atoi(argv[i+1]);
					i++;
				}
			}
			else if(strcasecmp(temp,"sounddriver")==0)
			{
				if(i+1<argc)
				{
					fiend_gfx_driver = atoi(argv[i+1]);
					i++;
				}
			}
			else if(strcasecmp(temp,"map")==0)
			{
				if(i+1<argc)
				{
					strcpy(map_file,argv[i+1]);
					i++;
				}
			}
			else if(strcasecmp(temp,"mapdata")==0)
			{
				if(i+1<argc)
				{
					strcpy(temp2,argv[i+1]);
					
					put_backslash(temp2);

					sprintf(global_var_filename, "%svars.inf",temp2);
					sprintf(global_trigger_filename, "%striggers.inf",temp2);
					sprintf(npc_filename, "%snpc.inf",temp2);
					sprintf(item_filename, "%sitem.inf",temp2);
					sprintf(enemy_filename, "%senemy.inf",temp2);

					fiend_show_intro = 0;
										
					i++;
				}
			}

		}
}