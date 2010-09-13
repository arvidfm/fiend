#include <allegro.h>
#include <stdio.h>
#include "../fiend.h"
#include "mapedit.h"


//extern DATAFILE *misc;

int current_lightmap=0;

BITMAP *test_buffer;
BITMAP *temp_buffer1;
BITMAP *temp_buffer2;



//These are declared here so that two functions can use em....
//very lasy...bur what da fuck....
char temp_x[8]="";
char temp_y[8]="";

char temp_bmp_h[8]="";
char temp_bmp_w[8]="";

char temp_s_h[8]="";
char temp_s_w[8]="";

char temp_r[8]="";
char temp_c_r[8]="";

char temp_w_x[8]="";
char temp_w_y[8]="";

char temp_f[8]="";
char temp_m_l[8]="";

char temp_a[8]="";



//make a new light map
void make_new_light(int x, int y)
{
	if(map->num_of_lights >= MAX_LIGHT_NUM - 1 ) //Check so we have space for the light map
	{
		alert("No more lights can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
			
	}


	map->light[ map->num_of_lights ].bitmap_w = 100;//Set it with some standard values
	map->light[ map->num_of_lights ].bitmap_h = 100;

	map->light[ map->num_of_lights ].strech_w = 100;
	map->light[ map->num_of_lights ].strech_h = 100;

	map->light[ map->num_of_lights ].x = 50;
	map->light[ map->num_of_lights ].y = 50;

	map->light[ map->num_of_lights ].r = 50;
	map->light[ map->num_of_lights ].centre_r = 1;

	map->light[ map->num_of_lights ].flash = 0;

	map->light[ map->num_of_lights ].world_x = x;
	map->light[ map->num_of_lights ].world_y = y;

	map->light[ map->num_of_lights ].max_light = 255;

	map->light[ map->num_of_lights ].active = 1;
	
	
	//lightmap_data[map->num_of_lights] = create_bitmap_ex(8,40,40);
	
	//clear_to_color(lightmap_data[map->num_of_lights], 50);
	
	//Create the lightmap and put it in lightmap_data...
	create_light_map(&lightmap_data[map->num_of_lights], &map->light[ map->num_of_lights ]);
		
	//Now we got one more lightmap
	map->num_of_lights++;

}





int d_draw_test_lightmap_proc(int msg,DIALOG *d,int c)
{
	LIGHT_DATA temp_light;
	int scaled=0;
	int h,w;
	float dw,dh;
	
	switch(msg)
	{
	case MSG_START:
		set_palette(misc[0].dat);
		test_buffer = create_bitmap(d->w, d->h);
		clear_to_color(test_buffer,makecol(255,255,255));
		break;

	case MSG_END:
		destroy_bitmap(test_buffer);
		break;

	case MSG_DRAW:
		clear_to_color(test_buffer,makecol(255,255,255));
		
		
		//set the values...
		temp_light.x = atoi(temp_x);
		temp_light.y = atoi(temp_y);

		temp_light.bitmap_w = atoi(temp_bmp_w);
		temp_light.bitmap_h = atoi(temp_bmp_h);

		temp_light.strech_w = atoi(temp_s_w);
		temp_light.strech_h = atoi(temp_s_h);

		temp_light.r = atoi(temp_r);
		temp_light.centre_r = atoi(temp_c_r);

		temp_light.world_x = atoi(temp_w_x);
		temp_light.world_y = atoi(temp_w_y);

		temp_light.flash = atoi(temp_f);
		temp_light.max_light = atoi(temp_m_l);

			
		//create the light map
		create_light_map(&temp_buffer1, &temp_light);

		
		if(temp_light.strech_w>d->w || temp_light.strech_h>d->h)//if streched to do some math......
		{
			dw = ( (float)temp_light.strech_w/(float)d->w);
			dh = ( (float)temp_light.strech_h/(float)d->h);

			if(dh>dw)
			{
				w = temp_light.strech_w / dh;
				h = temp_light.strech_h / dh;
			}

			if(dw>dh)
			{
				w = temp_light.strech_w / dw;
				h = temp_light.strech_h / dw;
			}

			temp_buffer2 = create_bitmap_ex(8,w,h);
			
			stretch_blit(temp_buffer1,temp_buffer2,0,0,temp_light.strech_w,temp_light.strech_h,0,0,w,h);
			
			destroy_bitmap(temp_buffer1);
			temp_buffer1 = create_bitmap_ex(8,w,h);

			blit(temp_buffer2, temp_buffer1,0,0,0,0,w,h);
			destroy_bitmap(temp_buffer2);

			scaled =1;

			
		}
		else //set w and h to normal values
		{
			w = temp_light.strech_w;
			h = temp_light.strech_h;

		}

		//here is a color conversion!!!
		blit(temp_buffer1,test_buffer,0,0, d->w/2-w/2, d->h/2-h/2, w, h);

		//if scaled print it
		//text_mode(0);
		if(scaled) textprintf_ex(test_buffer,font,2,2,makecol(255,0,255),0,"Scaled");
		
		textprintf_ex(test_buffer,font,230,190,makecol(255,0,255),0,"Num: %d", current_lightmap);
		//text_mode(-1);
		//Blit the test to screen
		blit(test_buffer, screen,0,0,d->x, d->y,d->w,d->h);
		

		//Get rid of the temp buffer
		destroy_bitmap(temp_buffer1);



	}


	return D_O_K;
}


//this button makes the test window get redrawn...
int d_testbutton_proc(int msg,DIALOG *d,int c)
{
	int ans;
	ans = d_button_proc(msg,d,c);
	
	if(ans==D_EXIT)
	{
		return D_REDRAW;
		
	}
	

	return D_O_K;
}



//A dialog for editing the lightmap
int edit_lightmap(void)
{

	int ans;
	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  300,  300,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 425,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     370, 425,   53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		{  d_testbutton_proc,     430, 425,   53,  20,   FG_COLOR,   BG_COLOR,     't',  D_EXIT,   0,    0,        "&Test" },

		{  d_draw_test_lightmap_proc, 255, 155,  290,  200,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,        NULL },


		{  d_text_proc,     255, 360,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "x:"},
		{  d_edit_proc,     290, 360,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_x},
		
		{  d_text_proc,     255, 370,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "y:"},
		{  d_edit_proc,     290, 370,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_y},
				
		{  d_text_proc,     255, 380,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "b w:"},
		{  d_edit_proc,     290, 380,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_bmp_w},
		
		{  d_text_proc,     255, 390,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "b h:"},
		{  d_edit_proc,     290, 390,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_bmp_h},
		
		{  d_text_proc,     255, 400,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "active:"},
		{  d_edit_proc,     310, 400,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_a},
			
			
		{  d_text_proc,     350, 360,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "s w:"},
		{  d_edit_proc,     385, 360,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_s_w},
		
		{  d_text_proc,     350, 370,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "s h:"},
		{  d_edit_proc,     385, 370,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_s_h},
		
		{  d_text_proc,     350, 380,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "r:"},
		{  d_edit_proc,     385, 380,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_r},
		
		{  d_text_proc,     350, 390,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "c r:"},
		{  d_edit_proc,     385, 390,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_c_r},
		
		

		{  d_text_proc,     445, 360,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "w x:"},
		{  d_edit_proc,     480, 360,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_w_x},
		
		{  d_text_proc,     445, 370,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "w y:"},
		{  d_edit_proc,     480, 370,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_w_y},
		
		{  d_text_proc,     445, 380,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "f:"},
		{  d_edit_proc,     480, 380,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_f},
		
		{  d_text_proc,     445, 390,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "m l:"},
		{  d_edit_proc,     480, 390,  53,   20,   FG_COLOR,   BG_COLOR,     0,  0,   5,    0,   temp_m_l},
		
		
		


		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

  
	sprintf(temp_x,"%d",map->light[current_lightmap].x);
	sprintf(temp_y,"%d",map->light[current_lightmap].y);

	sprintf(temp_bmp_h,"%d",map->light[current_lightmap].bitmap_h);
	sprintf(temp_bmp_w,"%d",map->light[current_lightmap].bitmap_w);

	sprintf(temp_s_h,"%d",map->light[current_lightmap].strech_h);
	sprintf(temp_s_w,"%d",map->light[current_lightmap].strech_w);

	sprintf(temp_r,"%d",map->light[current_lightmap].r);
	sprintf(temp_c_r,"%d",map->light[current_lightmap].centre_r);

	sprintf(temp_w_x,"%d",map->light[current_lightmap].world_x);
	sprintf(temp_w_y,"%d",map->light[current_lightmap].world_y);

	sprintf(temp_f,"%d",map->light[current_lightmap].flash);
	sprintf(temp_m_l,"%d",map->light[current_lightmap].max_light);
	
	sprintf(temp_a,"%d",map->light[current_lightmap].active);
	
	
	ans = popup_dialog(mission_dialog,-1);
  
	if(ans==1)
	{
		map->light[current_lightmap].x = atoi(temp_x);
		map->light[current_lightmap].y = atoi(temp_y);

		map->light[current_lightmap].bitmap_w = atoi(temp_bmp_w);
		map->light[current_lightmap].bitmap_h = atoi(temp_bmp_h);

		map->light[current_lightmap].strech_w = atoi(temp_s_w);
		map->light[current_lightmap].strech_h = atoi(temp_s_h);

		map->light[current_lightmap].r = atoi(temp_r);
		map->light[current_lightmap].centre_r = atoi(temp_c_r);

		map->light[current_lightmap].world_x = atoi(temp_w_x);
		map->light[current_lightmap].world_y = atoi(temp_w_y);

		map->light[current_lightmap].flash = atoi(temp_f);
		map->light[current_lightmap].max_light = atoi(temp_m_l);

		map->light[current_lightmap].active = atoi(temp_a);

		destroy_bitmap(lightmap_data[current_lightmap]);
		
		create_light_map(&lightmap_data[current_lightmap], &map->light[current_lightmap]);

	}



	return D_O_K;
}



int delete_lightmap(void)
{
 int ans;
 int i;
 ans = alert("Sure you want to delte the lightmap?",NULL,NULL,"Yes","Cancel",'\r',0);

 if(ans==1)
 {
	 /*for(i=current_lightmap;i<map->num_of_lights-1;i++)
	 {	
		destroy_bitmap(lightmap_data[i]);
		lightmap_data[i] = create_bitmap_ex(8,map->light[i+1].strech_w, map->light[i+1].strech_w);

		blit(lightmap_data[i], lightmap_data[i+1],0,0,0,0,map->light[i+1].strech_w,map->light[i+1].strech_h);
	 }
	 destroy_bitmap(lightmap_data[map->num_of_lights]-1);*/

	 for(i=current_lightmap;i<map->num_of_lights;i++)
		 destroy_bitmap(lightmap_data[i]);
	 
	 for(i=current_lightmap;i<map->num_of_lights-1;i++)
	 {
		 map->light[i].x = map->light[i+1].x;
		 map->light[i].y = map->light[i+1].y;
		 
		 map->light[i].bitmap_w = map->light[i+1].bitmap_w;
		 map->light[i].bitmap_h = map->light[i+1].bitmap_h;
		 
		 map->light[i].strech_w = map->light[i+1].strech_w;
		 map->light[i].strech_h = map->light[i+1].strech_h;

		 map->light[i].world_x = map->light[i+1].world_x;
		 map->light[i].world_y = map->light[i+1].world_y;

		 map->light[i].r = map->light[i+1].r;
		 map->light[i].centre_r = map->light[i+1].centre_r;

		 map->light[i].flash = map->light[i+1].flash;
		 map->light[i].max_light = map->light[i+1].max_light;
		 
	 }
	 map->num_of_lights--;
	 
	 for(i=current_lightmap;i<map->num_of_lights;i++)
		 create_light_map(&lightmap_data[i], &map->light[i]);
	 
	 selected = -1;
	 

 }

 


	return D_O_K;
}


int lightmap_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Edit",    edit_lightmap,      NULL,          0,         NULL},
	{ "",  NULL,      NULL,          0,         NULL},
	{ "Delete",  delete_lightmap,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_lightmap=num;

	ans=do_menu(temp_menu,mouse_x,mouse_y);

	return ans;
}
