////////////////////////////////////////////////////
// This file contains the main game and logic funcs
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <allegro.h>

#include "../fiend.h"
#include "../grafik4.h"
#include "../console.h"



char map_file[80] = "maps\\lauder_alive\\lauder.map";


//some variables
int game_ended=0;
int lights_flashes=0;


//Speed Control
volatile int speed_counter = 0;

int fiend_load_saved_game=0;
int fiend_new_game=0;
int fiend_show_intro=1;


//The speed counter function
void increment_speed_counter(void)
{
 if(!csl_started)
  speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);





//update the logic
void update_game_logic(void)
{
	if(!message_active)
	{
		update_tile_object_height();
		
		update_npc();
				
		check_triggers(1); 
	
		check_link_collison();
		update_auto_move();
		
	
		update_soundemitors();
		update_player();
		update_objects();
		update_door_objects();
		update_enemy();
		
		update_shells();
		update_bloodpools();
		update_missiles();
		update_flames();
		update_particles();
		update_beams();

		check_look_at_areas();

		update_tiles();
							
		update_sound();
		
		if(inventory_is_on)update_inventory_logic();
		
		if(fiend_note_is_on)update_note_logic();

		update_normal_light();

		if(RANDOM(0,100)>90)lights_flashes=0;
		if(RANDOM(0,100)>83 || lights_flashes==1)
			lights_flashes=1;

		update_effects();
	}

	update_pickup_message();
	update_engine_error();
	update_message();
	update_global_keys();

	fiend_playtime++;	   
}	






void the_game(void)
{	
	int ans;
	
	//----BEGIN SPEED CONTROL--------
	
	LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);

    if(install_int_ex(increment_speed_counter, BPS_TO_TIMER(60))<0)return;
	
	//----END SPEED CONTROL--------
	
	
	//The menu

	//load configs
	load_config_file();


	show_gripdesign();
	show_poem();
	
	ans = fiend_menu(0);

	if(ans == -1)game_ended=1;
	
	//--Begin shit that is going to be taken away (maybe)
	load_npc();
	load_enemy_data();
	load_item_data();
	load_global_vars();
	load_global_triggers();

	clear_player_data();

	
	
	if(fiend_new_game)
	{
		if(fiend_show_intro)
			show_intro_text();
				
		if(!load_edit_map(map, map_file))
		{
			allegro_message("Couldn't load map \"%s\".",map_file);
			return;
		}
		
		player.x = map->player_x;
		player.y = map->player_y;
		player.angle = map->player_angle;
	
		speed_counter =1;
	
		total_reset_npc_ai();
		reset_npc_data();
	
		total_reset_enemy_ai();
		reset_enemy_data();

		get_current_map_objects();
	
		check_triggers(0);	
	
	
	
		update_before_map();
	
		reset_shells();
		reset_missiles();
		reset_bloodpools();
		reset_flames();
		reset_particles();
		reset_effects();
		reset_beams();
	}
	else if(fiend_load_saved_game)
	{
		if(ans == 1)load_game("save\\save1.sav");
		else if(ans == 2)load_game("save\\save2.sav");
		else if(ans == 3)load_game("save\\save3.sav");
		else if(ans == 4)load_game("save\\save4.sav");
		else if(ans == 5)load_game("save\\save5.sav");
	}
	
	//--End that shit

	init_frame_speed();
	
	while(!game_ended)
	{
		while(speed_counter>0)
		{
			update_game_logic();
		
			speed_counter--;
		}
		
		draw_level();
	
	}


	if(game_complete) show_ending();

	release_map(map);
}	





void main(int argc, char *argv[])
{
	allegro_init();
	
	load_config_file();
	
	get_args(argc,argv);

	install_mouse();
    install_keyboard();
    install_timer();
    
	set_window_title("Fiend");

	if(init_fiend()==CSLMSG_QUIT){allegro_message(fiend_errorcode);return;}

	//fiend_menu(0);
	the_game();

	exit_fiend();
	
}
END_OF_MAIN();



