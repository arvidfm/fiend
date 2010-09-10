////////////////////////////////////////////////////
// This file contains the funcs for the menu in the 
// start of the game...
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <allegro.h>
#include <string.h>
#include <stdio.h>


#include "../fiend.h"
#include "../grafik4.h"
#include "../draw.h"


#define FIEND_FG_COLOR makecol(255,255,255)
#define FIEND_BG_COLOR makecol(0,0,0)

#define MENU_MAIN 0
#define MENU_NEWGAME 1
#define MENU_LOADGAME 2
#define MENU_OPTIONS 3
#define MENU_GAMMA 4
#define MENU_EXIT 5
#define MENU_DEFINE 6
#define MENU_WANNAEXIT 7
#define MENU_ADJUST 8
#define MENU_WANNANEW 9


#define MAX_ROWS 20

#define WAVE_LENGTH 200



extern int fiend_load_saved_game;
extern int fiend_new_game;


static BITMAP *bmp_back;
static BITMAP *bmp_fade;
static BITMAP *bmp_text[MAX_ROWS];

static DATAFILE *font_menu;
static DATAFILE *font_menu2;
static menu_in_game;

static int return_value=0;

static int menu_up_down=0; 
static int menu_down_down=0; 
static int menu_right_down=0; 
static int menu_left_down=0; 
static int menu_enter_down=0;
static int menu_esc_down=0;

static int current_menu;
static int next_menu;

static int quit_menu=0;

static int max_rows[] = {5,0,6,4,0,0,10,2,1,2,2};

static int menu_row;

static int wave_x=0;
static float wave_y=0;

static int define_key =-1;
static int define_key_flash =0;
static int define_key_enter_off =0;



static int fading=0;
static int alpha = 0;

static int text_alpha[MAX_ROWS];


int play_menu_sound(char *name, int loop)
{
	int i,num;
	int found_sound=0;
	int found_data=0;
	int temp;


	if(!sound_is_on)return -1;
	
	///////// Find The sound number ////////////
	
	for(i=0;i<num_of_sounds;i++)
		if(strcmp(sound_info[i].name,name)==0)
		{
			found_sound=1;
			num=i;
			break;
		}
	if(!found_sound)return -1;

	//if the sound has more then one alternative (num>1) randomize between em
	if(sound_info[num].num>1)
	{
		temp = RANDOM(0,sound_info[num].num-1);
		num+=temp;
	}
	
	
	FSOUND_Sample_SetDefaults(sound_info[num].sound,-1,sound_info[num].volume,128,200);
	
	if(loop)
		FSOUND_Sample_SetLoopMode(sound_info[num].sound,FSOUND_LOOP_NORMAL);

	
	FSOUND_PlaySound(FSOUND_FREE, sound_info[num].sound);
	
	return 	i;	
}



static void draw_wave_text(BITMAP *src, BITMAP *dest, int x, int y)
{
	int i;

	float x_add;
	
	for(i=0;i<src->h;i++)
	{
		x_add = (fixtof( fsin( degree_to_fixed(i*10 + wave_x) ) )+1)*WAVE_LENGTH;
		stretch_blit(src,dest,0,i,src->w,1,x-x_add ,y+i,src->w+x_add*2,1);
	}

}

static void draw_medium_text(BITMAP *dest, char *text, int x, int y,int row)
{
	
	textout(dest,font_menu2->dat,text, x,y-2,makecol(20,20,20));
	textout(dest,font_menu2->dat,text, x,y+2,makecol(20,20,20));
	textout(dest,font_menu2->dat,text, x,y,makecol(text_alpha[row],text_alpha[row],text_alpha[row]));
}



static void draw_large_text(BITMAP *dest, char *text, int x, int y)
{
	int y_add;

	y_add = (fixtof( fsin( degree_to_fixed(wave_y) ) ))*5;


	textout_centre(dest,font_menu->dat,text, x,y-6+y_add*2,makecol(40,40,40));
	textout_centre(dest,font_menu->dat,text, x,y+6+y_add*2,makecol(40,40,40));
	textout_centre(dest,font_menu->dat,text, x,y-2+y_add,makecol(90,90,90));
	textout_centre(dest,font_menu->dat,text, x,y+2+y_add,makecol(90,90,90));
	textout_centre(dest,font_menu->dat,text, x,y,makecol(220,220,220));
}

static void draw_head_text(BITMAP *dest, char *text, int x, int y)
{
	int y_add;

	y_add = (fixtof( fsin( degree_to_fixed(wave_y) ) ))*5;


	textout_centre(dest,font_menu->dat,text, x,y-6+y_add*2,makecol(20,20,20));
	textout_centre(dest,font_menu->dat,text, x,y+6+y_add*2,makecol(20,20,20));
	textout_centre(dest,font_menu->dat,text, x,y-2+y_add,makecol(50,50,50));
	textout_centre(dest,font_menu->dat,text, x,y+2+y_add,makecol(50,50,50));
	textout_centre(dest,font_menu->dat,text, x,y,makecol(110,110,110));
}

static void fade_menu(void)
{
	if(fading !=0)
	{
		blit(virt,bmp_fade, 0, 150,0,0,480,330);

		set_trans_blender(0,0,0,0);
		draw_lit_sprite(virt,bmp_fade,0,150, alpha);

	}

}




static void update_menu_gfx(void)
{
	int i;
	int x_add;
	char temp_string[60];
	char temp_string2[60];
	
	SAVEDATA temp_save;
    FILE *f;
	int s,h,m;

	
	clear(virt);
	
	x_add = (fixtof( fsin( degree_to_fixed(wave_y) ) ))*8;
	
	blit(bmp_back, virt,0,0,0,0,bmp_back->w,bmp_back->h);
	draw_additive_sprite(virt, bmp_back,x_add,0);
	draw_additive_sprite(virt, bmp_back,-x_add,0);


	if(current_menu == MENU_MAIN)
	{
		for(i=0;i<max_rows[MENU_MAIN];i++)
			clear_to_color(bmp_text[i], makecol(0,0,0));
		
		draw_large_text(bmp_text[0], "New Game",240,0);
		draw_large_text(bmp_text[1], "Load Game",240,0);
		draw_large_text(bmp_text[2], "Options",240,0);
		draw_large_text(bmp_text[3], "Gamma",240,0);
		draw_large_text(bmp_text[4], "Exit",240,0);
		
		if(menu_row == 0)
		{
			draw_wave_text(bmp_text[0],virt,0,160);		
		}
		else
			blit(bmp_text[0],virt,0,0,0,160,bmp_text[0]->w,bmp_text[0]->h);
		
		if(menu_row == 1)
		{
			draw_wave_text(bmp_text[1],virt,0,200);		
		}
		else
			blit(bmp_text[1],virt,0,0,0,200,bmp_text[0]->w,bmp_text[0]->h);
		
		if(menu_row == 2)
		{
			draw_wave_text(bmp_text[2],virt,0,240);		
		}
		else
			blit(bmp_text[2],virt,0,0,0,240,bmp_text[0]->w,bmp_text[0]->h);
		
		if(menu_row == 3)
		{
			draw_wave_text(bmp_text[3],virt,0,280);		
		}
		else
			blit(bmp_text[3],virt,0,0,0,280,bmp_text[0]->w,bmp_text[0]->h);
		
		if(menu_row == 4)
		{
			draw_wave_text(bmp_text[4],virt,0,320);		
		}
		else
			blit(bmp_text[4],virt,0,0,0,320,bmp_text[0]->w,bmp_text[0]->h);
	}
	else if(current_menu == MENU_LOADGAME)
	{
		clear_to_color(bmp_text[0], makecol(0,0,0));

		draw_head_text(bmp_text[0], "Load Game",240,0);

		draw_wave_text(bmp_text[0],virt,0,150);

		for(i=0;i<5;i++)
		{
			
			sprintf(temp_string2,"save/save%d.sav",i+1);
		
			if(exists(temp_string2))
			{
				f = fopen(temp_string2, "rb");
	
				fread(&temp_save, sizeof(SAVEDATA),1,f);

				fclose(f);
				
				s = (temp_save.play_time/60)%60;
				m = (temp_save.play_time/(60*60))%60;
				h = temp_save.play_time/(60*60*60);
								
				sprintf(temp_string,"%s %d:%d:%d",temp_save.name,h,m,s);
			
				draw_medium_text(virt, temp_string, 120,200+i*25,i);  
			}
			else
			{
				draw_medium_text(virt, "[empty]", 120,200+i*25,i);  
			}
		}
		
		draw_medium_text(virt, "Back", 120, 350,5);  
	}
	else if(current_menu == MENU_OPTIONS)
	{
		clear_to_color(bmp_text[0], makecol(0,0,0));

		draw_head_text(bmp_text[0], "Options",240,0);

		draw_wave_text(bmp_text[0],virt,0,150);
		
		draw_medium_text(virt, "Sound Volume:", 120, 200,0);  
		draw_medium_text(virt, "Vsync:", 120, 225,1);  
		draw_medium_text(virt, "Define Keys", 120, 250,2);  
		draw_medium_text(virt, "Back", 120, 300,3);

		
		if(vsync_is_on)
			draw_medium_text(virt, "On", 300, 225,1);
		else
			draw_medium_text(virt, "Off", 300, 225,1);


		i = (((float)fiend_sound_volume/255)*100);
		sprintf(temp_string, "%d%%",i); 
		
		draw_medium_text(virt, temp_string, 300, 200,0);  
		
	}
	else if(current_menu == MENU_DEFINE)
	{
		clear_to_color(bmp_text[0], makecol(0,0,0));

		draw_head_text(bmp_text[0], "Options",240,0);

		draw_wave_text(bmp_text[0],virt,0,150);
		
		draw_medium_text(virt, "Attack/Pickup:", 120, 200,0);  
		draw_medium_text(virt, "Interact:", 120, 220,1);  
		draw_medium_text(virt, "Draw Gun:", 120, 240,2);  
		draw_medium_text(virt, "Strafe:", 120, 260,3);
		draw_medium_text(virt, "Inventory:", 120, 280,4);

		draw_medium_text(virt, "Forward:", 120, 300,5);  
		draw_medium_text(virt, "Backward:", 120, 320,6);  
		draw_medium_text(virt, "Turn Left:", 120, 340,7);  
		draw_medium_text(virt, "Turn Right:", 120, 360,8);
		draw_medium_text(virt, "Back", 120, 400,9);


		key_2_str(key_attack,temp_string);
		if(define_key != 0)
			draw_medium_text(virt, temp_string, 270, 200,0);  
		else if(define_key>-1)
			draw_medium_text(virt, "???", 270, 200,0);  
			
		key_2_str(key_action,temp_string);
		if(define_key != 1)
			draw_medium_text(virt, temp_string, 270, 220,1);  
		else if(define_key>-1)
			draw_medium_text(virt, "???", 270, 220,1);  
		
		key_2_str(key_pickup,temp_string);
		if(define_key != 2)
			draw_medium_text(virt, temp_string, 270, 240,2);  
		else if(define_key>-1)
			draw_medium_text(virt, "???", 270, 240,2);  
		
		key_2_str(key_strafe,temp_string);
		if(define_key != 3)
			draw_medium_text(virt, temp_string, 270, 260,3);  
		else if(define_key>-1)
			draw_medium_text(virt, "???", 270, 260,3);  
		
		key_2_str(key_inventory,temp_string);
		if(define_key != 4)
			draw_medium_text(virt, temp_string, 270, 280,4);  
		else if(define_key>-1)
			draw_medium_text(virt, "???", 270, 280,4);  
		


		key_2_str(key_forward,temp_string);
		if(define_key != 5)
			draw_medium_text(virt, temp_string, 270, 300,5);  
		else if(define_key>-1)
			draw_medium_text(virt, "???", 270, 300,5);  
		
		key_2_str(key_backward,temp_string);
		if(define_key != 6)
			draw_medium_text(virt, temp_string, 270, 320,6);  
		else if(define_key>-1)
			draw_medium_text(virt, "???", 270, 320,6);  
		
		key_2_str(key_left,temp_string);
		if(define_key != 7)
			draw_medium_text(virt, temp_string, 270, 340,7);  
		else if(define_key>-1)
			draw_medium_text(virt, "???", 270, 340,7);  
		
		key_2_str(key_right,temp_string);
		if(define_key != 8)
			draw_medium_text(virt, temp_string, 270, 360,8);  
		else if(define_key>-1)
			draw_medium_text(virt, "???", 270, 360,8);  
		
	}
	else if(current_menu == MENU_WANNAEXIT)
	{
		clear_to_color(bmp_text[0], makecol(0,0,0));

		draw_head_text(bmp_text[0], "EXIT",240,0);

		draw_wave_text(bmp_text[0],virt,0,150);
		
		draw_medium_text(virt, "Sure you wanna quit?", 120, 200,3);  
				
		draw_medium_text(virt, "No", 120, 240,0);  
		draw_medium_text(virt, "Yes", 120, 265,1);  
	}
	else if(current_menu == MENU_ADJUST)
	{
		clear_to_color(bmp_text[0], makecol(0,0,0));

		draw_head_text(bmp_text[0], "Gamma",240,0);

		draw_wave_text(bmp_text[0],virt,0,150);

		
		rect(virt,160,200,320,250,makecol(60,200,70));

		textout_centre(virt,font_menu->dat,"F I E N D",240,205,makecol(10,15,10));

		
		textout(virt,font_avalon->dat,"To get the most out of Fiend it is important that",30,260,makecol(200,200,200));
		textout(virt,font_avalon->dat,"you have the correct monitor settings.",30,277,makecol(200,200,200));
		textout(virt,font_avalon->dat,"Follow the steps below to get the optimal settings.",30,294,makecol(200,200,200));
		
		textout(virt,font_avalon->dat,"1. Turn contrast to max.",30,328,makecol(200,200,200));
		textout(virt,font_avalon->dat,"2. Turn up brigthness until you see \"Fiend\" in",30,345,makecol(200,200,200));
		textout(virt,font_avalon->dat,"the green box",53,362,makecol(200,200,200));
		textout(virt,font_avalon->dat,"3. Turn down brigthness until \"Fiend\" disappears.",30,379,makecol(200,200,200));
		
		
	
		draw_medium_text(virt, "Back", 210,420,0);  
	}
	else if(current_menu == MENU_WANNANEW)
	{
		clear_to_color(bmp_text[0], makecol(0,0,0));

		draw_head_text(bmp_text[0], "New Game",240,0);

		draw_wave_text(bmp_text[0],virt,0,150);
		
		draw_medium_text(virt, "Start a new game?", 120, 200,3);  
				
		draw_medium_text(virt, "No", 120, 240,0);  
		draw_medium_text(virt, "Yes", 120, 265,1);  
	}
	




	
	fade_menu();
	
	//if(vsync_is_on)vsync();
	//acquire_screen();
	blit(virt,screen,0,0,80,0,virt->w,virt->h);

}

static void update_menu_logic(void)
{
	int i;
	
	wave_x+=3;
	wave_y+=1;

	
	if(fading == 1 && alpha<255)
	{
		alpha+=4;
		if(alpha>255)alpha = 255;
		if(alpha == 255)
		{
			fading = -1;
			current_menu = next_menu;
			menu_row = 0; 
		}
	}
	if(fading == -1 && alpha>0)
	{
		alpha-=4;
		if(alpha<0)alpha = 0;
		if(alpha == 0)
		{
			fading = 0;
		}
	}

	if(wave_x>360)wave_x=0;
	if(wave_y>360)wave_y=0;


	for(i=0;i<MAX_ROWS;i++)
	{
		if(menu_row == i)
		{
			text_alpha[i]+=8;

			if(text_alpha[i]>255)
				text_alpha[i] = 255;
		}
		else
		{
			text_alpha[i]-=5;
			
			if(text_alpha[i]<80)
				text_alpha[i] = 80;

		}
	}

	
	if(define_key!=-1)
	{	
		if(!define_key_enter_off && !key[KEY_ENTER])
			define_key_enter_off = 1;
		
		if(define_key_enter_off)
		{
			for(i=0;i<KEY_MAX;i++)
			{
				if(key[i])
				{
					if(define_key==0){key_attack=i;define_key=-1;break;}
					else if(define_key==1){key_action=i;define_key=-1;break;}
					else if(define_key==2){key_pickup=i;define_key=-1;break;}
					else if(define_key==3){key_strafe=i;define_key=-1;break;}
					else if(define_key==4){key_inventory=i;define_key=-1;break;}
					else if(define_key==5){key_forward=i;define_key=-1;break;}
					else if(define_key==6){key_backward=i;define_key=-1;break;}
					else if(define_key==7){key_left=i;define_key=-1;break;}
					else if(define_key==8){key_right=i;define_key=-1;break;}

				}
			}
			menu_up_down = 1;
			menu_down_down = 1;
			menu_enter_down = 1;
		}

	}


	if(fading == 0 || define_key !=0) 
	{
		
		if(!key[KEY_LEFT])menu_left_down=0;
		if(key[KEY_LEFT] && !menu_left_down)
		{
			
			if(current_menu == MENU_OPTIONS)
			{
				if(menu_row == 0)
				{
					fiend_sound_volume-=10;
					if(fiend_sound_volume<0)
						fiend_sound_volume = 0;

					FSOUND_SetSFXMasterVolume(fiend_sound_volume);
				}
				else if(menu_row == 1)
				{
					if(vsync_is_on)
						vsync_is_on = 0;
					else
						vsync_is_on = 1;
				}
			}
			play_menu_sound("menu_move2",0);
	

			menu_left_down=1;
		}
		
		if(!key[KEY_RIGHT])menu_right_down=0;
		if(key[KEY_RIGHT] && !menu_right_down)
		{
			if(current_menu == MENU_OPTIONS)
			{
				if(menu_row == 0)
				{
					fiend_sound_volume+=10;
					if(fiend_sound_volume>255)
						fiend_sound_volume = 255;

					FSOUND_SetSFXMasterVolume(fiend_sound_volume);
				}
				else if(menu_row == 1)
				{
					if(vsync_is_on)
						vsync_is_on = 0;
					else
						vsync_is_on = 1;
				}
			}
			play_menu_sound("menu_move2",0);
	

			menu_right_down=1;
		}

		if(!key[KEY_UP])menu_up_down=0;
		if(key[KEY_UP] && !menu_up_down)
		{
			play_menu_sound("menu_move2",0);
	
			menu_row--;
			if(menu_row < 0)
				menu_row = max_rows[current_menu]-1;
			//if(menu_row < 0)
			//	menu_row = 0;
			
			menu_up_down = 1;
		}
	
		if(!key[KEY_DOWN])menu_down_down=0;
		if(key[KEY_DOWN] && !menu_down_down)
		{
			play_menu_sound("menu_move2",0);
		
			menu_row++;
			if(menu_row >= max_rows[current_menu])
				menu_row = 0;
			menu_down_down = 1;
		}
	
		if(!key[KEY_ENTER])menu_enter_down=0;
		if(key[KEY_ENTER] && !menu_enter_down)
		{
			play_menu_sound("menu_select",0);
	
			if(current_menu == MENU_MAIN)
			{
				if(menu_row == 0)
				{
					fading = 1;
					next_menu = MENU_WANNANEW;
				}
				else if(menu_row == 1)
				{
					fading = 1;
					next_menu = MENU_LOADGAME;
				}
				else if(menu_row == 2)
				{
					fading = 1;
					next_menu = MENU_OPTIONS;
				}
				else if(menu_row == 3)
				{
					fading = 1;
					next_menu = MENU_ADJUST;
				}
				else if(menu_row == 4)
				{
					fading = 1;
					next_menu = MENU_WANNAEXIT;
				}
			}
			else if(current_menu == MENU_LOADGAME)
			{
				if(menu_row == 0 && exists("save\\save1.sav"))
				{
					return_value = 1;
					fiend_load_saved_game = 1;
					quit_menu=1;
				}
				else if(menu_row == 1 && exists("save\\save2.sav"))
				{
					return_value = 2;
					fiend_load_saved_game = 1;
					quit_menu=1;
				}
				else if(menu_row == 2 && exists("save\\save3.sav"))
				{
					return_value = 3;
					fiend_load_saved_game = 1;
					quit_menu=1;
				}
				else if(menu_row == 3 && exists("save\\save4.sav"))
				{
					return_value = 4;
					fiend_load_saved_game = 1;
					quit_menu=1;
				}
				else if(menu_row == 4 && exists("save\\save5.sav"))
				{
					return_value = 5;
					fiend_load_saved_game = 1;
					quit_menu=1;
				}
				else if(menu_row == 5)
				{
					fading = 1;
					next_menu = MENU_MAIN;
				}

			}
			else if(current_menu == MENU_OPTIONS)
			{
				if(menu_row == 1)
				{
					if(vsync_is_on)
						vsync_is_on = 0;
					else
						vsync_is_on = 1;
				}
				else if(menu_row == 2)
				{
					fading = 1;
					next_menu = MENU_DEFINE;
				}
				else if(menu_row == 3)
				{
					fading = 1;
					next_menu = MENU_MAIN;
					save_the_config_file();
				}

			}
			else if(current_menu == MENU_DEFINE)
			{
				if(menu_row > -1 && menu_row<9)
				{
					define_key = menu_row;
					define_key_enter_off = 0;
				}
				else if(menu_row == 9)
				{
					fading = 1;
					next_menu = MENU_OPTIONS;
				}

			}
			else if(current_menu == MENU_WANNAEXIT)
			{
				if(menu_row == 0)
				{
					fading = 1;
					next_menu = MENU_MAIN;
				}
				else if(menu_row == 1)
				{
					quit_menu=1;
					return_value=-1;
					screen_is_black=1;
					sound_is_on=0;
				}
			}
			else if(current_menu == MENU_ADJUST)
			{
				fading = 1;
				next_menu = MENU_MAIN;
			}
			else if(current_menu == MENU_WANNANEW)
			{
				if(menu_row == 0)
				{
					fading = 1;
					next_menu = MENU_MAIN;
				}
				else if(menu_row == 1)
				{
					fiend_new_game = 1;
					return_value = 0;
					quit_menu =1;
				}
			}

		
			menu_enter_down = 1;
		}

		if(!key[KEY_ESC])menu_esc_down=0;
		if(key[KEY_ESC] && !menu_esc_down)
		{
			play_menu_sound("menu_select",0);
			
			if(current_menu == MENU_MAIN)
			{
				if(!menu_in_game)
				{
					fading = 1;
					next_menu = MENU_WANNAEXIT;
				}
				else
				{
					quit_menu=1;
				}
			}
			else if(current_menu == MENU_LOADGAME)
			{
					fading = 1;
					next_menu = MENU_MAIN;
			}
			else if(current_menu == MENU_OPTIONS)
			{
					save_the_config_file();
					fading = 1;
					next_menu = MENU_MAIN;
			}
			else if(current_menu == MENU_DEFINE)
			{
					fading = 1;
					next_menu = MENU_OPTIONS;
			}
			else if(current_menu == MENU_WANNAEXIT)
			{
					fading = 1;
					next_menu = MENU_MAIN;
			}
			else if(current_menu == MENU_ADJUST)
			{
					fading = 1;
					next_menu = MENU_MAIN;
			}
			else if(current_menu == MENU_WANNANEW)
			{
					fading = 1;
					next_menu = MENU_MAIN;
			}
		
			menu_esc_down = 1;
		}
	}
}



int fiend_main_menu(void)
{
	speed_counter = 0;
	
	while(!quit_menu)
	{
		while(speed_counter>0)
		{
			update_menu_logic();
		
			speed_counter--;
		}
		
		update_menu_gfx();
	}


	return return_value;
}

int fiend_menu(int in_game)
{
	int ans,i;

	menu_in_game = in_game;

	pause_fiend_music();
	
	bmp_back = load_bitmap("graphic\\menu\\menu_back.pcx",NULL);
	if(bmp_back==NULL){allegro_message("couldn't load graphic\\menu\\menu_back.pcx");return 1;}

	for(i=0;i<MAX_ROWS;i++)
	{
		bmp_text[i] = create_bitmap(480,40);

		text_alpha[i] = 160;
	}
	
	bmp_fade = create_bitmap(480,330);
	font_menu = load_datafile("graphic\\fonts\\menu.dat");
	if(font_menu==NULL){allegro_message("couldn't load font menu");return 1;}
	
	font_menu2 = load_datafile("graphic\\fonts\\arial.dat");
	if(font_menu2==NULL){allegro_message("couldn't load font arial");return 1;}
	
	current_menu = MENU_MAIN;
	menu_row =0;
	fading =0;
	alpha = 0;
	define_key =-1;
	define_key_enter_off =0;
	return_value=0;
	menu_esc_down = 1;
	quit_menu =0;
	fiend_load_saved_game =0;
	fiend_new_game =0;


	
	ans = fiend_main_menu();

	destroy_bitmap(bmp_back);
	for(i=0;i<MAX_ROWS;i++)
	{
		destroy_bitmap(bmp_text[i]);
	}
	destroy_bitmap(bmp_fade);
	unload_datafile(font_menu);
	unload_datafile(font_menu2);

	resume_fiend_music();

	return ans;
}
