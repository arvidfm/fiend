////////////////////////////////////////////////////
// This file contains the main fiend funcs like init
// and exit. 
//
// Code by Thomas Grip and Pierre Renaux
///////////////////////////////////////////////////

#include <ctype.h>


#include "fiend.h"
#include "draw.h"
#include "grafik4.h"
#include "console.h"
#include "console_funcs.h"

//JUst so we get no errors.......
//more of that lazyness....
int map_x;
int map_y;

int in_the_game=0;

//some other variables
int color_depth=16;

int sound_is_on=1;
int vsync_is_on=0;
int debug_is_on=0;
int lightning_is_on=1;
int cache_lights_is_on=0;

int fiend_sound_volume=256;
int fiend_sound_buffer_size=100;
int fiend_music_volume=160;
int fiend_sound_driver=0;

int fiend_gfx_driver=GFX_AUTODETECT;

int game_complete=0;
		


//some drawing stuff
BITMAP *virt;
BITMAP *temp_bitmap;

BITMAP *csl_bkgd = NULL;

BITMAP *mask;

MAP_DATA *map;

BITMAP *lightmap_data[MAX_LIGHT_NUM];


DATAFILE *font_avalon,*font_arial,*font_avalon2,*font_small1,*font_small2;

//the error code shown when exiting the program with failure!
char fiend_errorcode[60]="Unknown error";

int init_fiend2(void)
{
	BITMAP *bmp;

	clear(screen);

	bmp = load_bitmap("graphic/menu/menu_back.pcx",NULL);
	
	stretch_sprite(screen, bmp,80,60,480,200);

	destroy_bitmap(bmp);


	rect(screen,29+80,419,441+80,461,makecol(40,40,40));
	rect(screen,28+80,418,442+80,462,makecol(40,40,40));
	
	
	//init light drawing
    //csl_textout(2,"Init lightdrawing...");
	rectfill(screen,30+80,420,65+80,460,makecol(160,20,10));
	init_draw();

	//init the gfx lib...
	init_grafik4();

	
	//Load the graphics....
	//csl_textout(2,"Loading tile data...");
	rectfill(screen,30+80,420,100+80,460,makecol(160,20,10));
	if(load_tiles())return CSLMSG_QUIT;
	//csl_textout(2,"Loading npc data...");
	rectfill(screen,30+80,420,135+80,460,makecol(160,20,10));
	if(load_characters())return CSLMSG_QUIT;
	//csl_textout(2,"Loading enemy data...");
	rectfill(screen,30+80,420,170+80,460,makecol(160,20,10));
	if(load_enemys())return CSLMSG_QUIT;
	//csl_textout(2,"Loading object data...");
	rectfill(screen,30+80,420,205+80,460,makecol(160,20,10));
	if(load_objects())return CSLMSG_QUIT;
	//csl_textout(2,"Loading item data...");
	rectfill(screen,30+80,420,240+80,460,makecol(160,20,10));
	if(load_items())return CSLMSG_QUIT;
	
	if(sound_is_on)
	{
		//csl_textout(2,"Loading sound data...");
		rectfill(screen,30+80,420,275+80,460,makecol(160,20,10));
		if(load_sounds())return CSLMSG_QUIT;
	}

	
	//csl_textout(2,"Loading minor data...");
	rectfill(screen,30+80,420,310+80,460,makecol(160,20,10));
	//Load some minor data
	if(load_weapons())return CSLMSG_QUIT;
	if(load_particles())return CSLMSG_QUIT;
	if(load_flames())return CSLMSG_QUIT;
	if(load_inventory_gfx())return CSLMSG_QUIT;
	if(load_message_faces())return CSLMSG_QUIT;
	
	//csl_textout(2,"Intializing data...");
	rectfill(screen,30+80,420,345+80,460,makecol(160,20,10));
	//Init some data...
	init_npc_data();
	init_enemy_data();
	init_item_data();
	init_global_vars();
	init_global_triggers();
	init_fiend_note();
	
	
	//csl_textout(2,"Creating effect data...");
	rectfill(screen,30+80,420,380+80,460,makecol(160,20,10));
	//some effect calcs
	create_normal_lightmaps();
	create_shadows();
	create_wall_shadows();
	make_los_borders();
	
	//csl_textout(2,"Loading font data...");
	rectfill(screen,30+80,420,415+80,460,makecol(160,20,10));
	//Load the fonts
	font_avalon = load_datafile("graphic/fonts/avalon.dat");
	if(font_avalon==NULL){ 
		strcpy(fiend_errorcode,"couldn't load font avalon");
		return CSLMSG_QUIT;
    }
	
	font_arial = load_datafile("graphic/fonts/arial.dat");
	if(font_arial==NULL){
		strcpy(fiend_errorcode,"couldn't load font arial");
		return CSLMSG_QUIT;
    }
	
	font_avalon2 = load_datafile("graphic/fonts/avalon2.dat");
	if(font_avalon2==NULL){ 
		strcpy(fiend_errorcode,"couldn't load font avalon2");
		return CSLMSG_QUIT;
    }
	
	font_small2 = load_datafile("graphic/fonts/small2.dat");
	if(font_small2==NULL){ 
		strcpy(fiend_errorcode,"couldn't load font small2");
		return CSLMSG_QUIT;
    }
	

	//csl_textout(2,"Making map...");
    rectfill(screen,30+80,420,440+80,460,makecol(160,20,10));
	
	//Make a map...
	map = calloc(sizeof(MAP_DATA),1);
	new_map(map);

  
 return CSLMSG_CLOSE;
}

int init_fiend(void)
{		
    //init the graphic mode

	if(color_depth==16)
	{
		set_color_depth(16);
		if(set_gfx_mode(fiend_gfx_driver,640,480,0,0)!=0)
		{
			set_color_depth(15);
			if(set_gfx_mode(fiend_gfx_driver,640,480,0,0)!=0){
				strcpy(fiend_errorcode,"couldn't enter graphics mode");
				return 1;}
		}
	}
	
	if(sound_is_on)
	{
		FSOUND_SetDriver(fiend_sound_driver);

		//FSOUND_SetBufferSize(fiend_sound_buffer_size);
	
		if(!FSOUND_Init(44100, 32, 0))
		{
			strcpy(fiend_errorcode,"couldn't install sound");
			return 1;
		}
	
		FSOUND_SetSFXMasterVolume(fiend_sound_volume);
	}

	
	clear(screen);

	//bmp = load_pcx("graphic/menu/loading.pcx",NULL);

    csl_bkgd = create_bitmap(480,480);
	clear(csl_bkgd);
    //blit(bmp, csl_bkgd, 0, 0, 0, 0, 480, 480);

    //destroy_bitmap(bmp);

	font_small1 = load_datafile("graphic/fonts/small1.dat");
	if(font_small1==NULL)
    {
		strcpy(fiend_errorcode,"couldn't load font small1");
		return 1;
    }
	
	//make the virtual screen
	virt = create_bitmap(480,480);
	clear(virt);

	//make a temp bitmap...
	temp_bitmap = create_bitmap(200,200);
	clear(temp_bitmap);

	//make the light mask
	mask = create_bitmap_ex(8,480,480);
	clear(mask);

    // Initialise the console
    csl_set_draw_after_textout(1);
    csl_set_quick_quit_key(KEY_TILDE);
    csl_set_always_redraw(0);

    init_console(480,draw_csl_dbuff);

    set_csl_font_color(makecol(255,255,255), -1);
    csl_set_y(480);
    csl_set_rect(4, 4, CSL_DBUFF_W-4, CSL_DBUFF_H-4);
    csl_set_bkgd_type(CSL_BKGD_BITMAP, makecol(0,127,127), csl_bkgd);
    csl_set_cursor(CSL_CURSOR_DEFAULT, NULL); 

    
	
	// Start the init function
    //csl_set_show_command(0);
    //csl_set_move_speed(480,10);
    
	if(init_fiend2()==CSLMSG_QUIT)
		return 1;

    //csl_remove_func(cslfunc_init_fiend);

    csl_set_y(280);
    csl_set_move_speed(17,17);
    csl_set_show_command(1);
    csl_set_cursor(CSL_CURSOR_DEFAULT, NULL); 

    register_console_funcs_and_vars();
		
	//The some routines are used by both editor and game this varibale makes them become seperated
    in_the_game=1;

	clear(screen);	
	return 0;
}

void exit_fiend(void)
{	
	int i;

	for(i=0;i<map->num_of_lights;i++)//release the lightmaps!!!
		 destroy_bitmap(lightmap_data[i]);

	free(normal_light_data);
		
	release_npc();
	release_enemy_data();
	release_item_data();
	release_global_vars();
	release_global_triggers();
	release_weapons();
	release_particles();
	release_fiend_note();
	release_message_faces();
		
	free_sounds();
	release_npc();
	release_tiles();
	release_characters();
	release_items();
	release_objects();
	free(map);

    close_console();

    if(csl_bkgd)
		destroy_bitmap(csl_bkgd);


	FSOUND_Close();

	allegro_exit();
}

