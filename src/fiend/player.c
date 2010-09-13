////////////////////////////////////////////////////
// This file contains player input, movement,
// animation.
// also has things as picking up items.   
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"
#include "../console.h"


//some gloabals
int key_forward = KEY_UP;
int key_backward = KEY_DOWN;
int key_left = KEY_LEFT;
int key_right = KEY_RIGHT;
int key_attack = KEY_LCONTROL;
int key_action = KEY_ALT;
int key_pickup = KEY_Z;
int key_inventory = KEY_SPACE;
int key_strafe = KEY_X;
int key_useitem = KEY_LSHIFT;
int key_reload = KEY_A;
int key_number[8] = {KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,KEY_6,KEY_7,KEY_8};




//Gloabal vars that can trigger events
int gun_fired=0;

extern int fiend_load_saved_game;
extern int fiend_new_game;


extern int menu_is_open;
extern int current_menu;
extern int menu_row;

//VArs for checking if the player has "used" anthing
int player_has_used;
int player_use_x;
int player_use_y;

int player_has_used_item;
int player_item_x;
int player_item_y;
int player_item_num;


//some variables used between
int auto_move_angle;
int auto_move_speed;
int auto_move_counter;
int auto_move_active=0;

static int walk_key_pressed=0;
static int walk_forward_pressed=0;

int pushing_an_object=0;
int have_pushed_an_object=0;
int the_push_voice=-1;


//vars for checking if a button is pushed..
int attack_key_down=0;	
int action_key_down=0;	
int pickup_key_down=0;	
int forward_key_down=0;	
int inventory_key_down=0;	
int useitem_key_down=0;	
int reload_key_down=0;
int num_key_down[10] = {0,0,0,0,0,0,0,0,0,0};	
int esc_down=0;


PLAYER_INFO player;

void clear_player_data(void)
{
	int i;
	
	player.x = 100; //x
	player.y = 100; //y
	player.angle = 0;//angle
	player.action = 0;//action
	player.frame = 0; //frame
	player.nextframe = 0;//nextframe
	player.active = 1;//active 

	player.dead = 0;//dead
	player.was_dead = 0;//was_dead
	
	player.hit_speed = 0;//hit speed
	player.hit_angle = 0;//hit angle
	
	player.energy = 100; //energy

	player.active_weapon = -1; //active weapon

	player.weapon_drawn = 0; //active weapon

	player.reloading = 0;//reloading

	player.running = 0;//running

	player.animation = 0;//animation

	player.last_dx = 0;//last_dx
	player.last_dy = 0;//last_dy

	player.can_shoot = 0;//can shoot
	
	player.num_of_items=0;
	for(i=0;i<MAX_ITEM_SPACES;i++)
	{
		player.item_space[i].item=-1;
		player.item_space[i].active=0;
	}
	player.num_of_ammo=0;
	for(i=0;i<MAX_AMMO_SPACES;i++)
	{
		player.ammo_space[i].item=-1;
		player.ammo_space[i].active=0;
	}
	player.num_of_weapons=0;
	for(i=0;i<MAX_WEAPON_SPACES;i++)
	{
		player.weapon_space[i].item=-1;
		player.weapon_space[i].active=0;
	}
	player.num_of_notes=0;
	for(i=0;i<MAX_NOTE_SPACES;i++)
	{
		player.note_space[i].item=-1;
		player.note_space[i].active=0;
	}
	
	fiend_playtime=0;
}



void reload_weapon(void)
{
	int temp;
	int weapon_num;
	
	if(player.active_weapon<0)return;

	weapon_num= get_weapon_num(item_info[item_data[player.active_weapon].type].s_string);
						
	if(item_data[player.active_weapon].value<weapon_info[weapon_num].max_ammo)
	{
		temp = item_data[player.active_weapon].sp1;
		item_data[player.active_weapon].sp1=0;	
							
		item_data[player.active_weapon].value+=temp;
		if(item_data[player.active_weapon].value>weapon_info[weapon_num].max_ammo)
		{
			item_data[player.active_weapon].sp1=item_data[player.active_weapon].value - weapon_info[weapon_num].max_ammo;	
			item_data[player.active_weapon].value=weapon_info[weapon_num].max_ammo;
		}
	


	}

}

//////////////////////////////////////////////////
////////// Check Push functions //////////////////
/////////////////////////////////////////////////

void check_push_object_x(float x, float y, float tmp)
{
	int i; 
	float temp_x;
	for(i=0;i<map->num_of_objects;i++)
	{
		if(map->object[i].active)
			if(map->object[i].pushable)
				if(check_angle_collision(x, y, char_info[0].w, char_info[0].h, 0, map->object[i].x, map->object[i].y, object_info[map->object[i].type].w,object_info[map->object[i].type].h,  map->object[i].angle))
				{
					
					if(player.last_dx==0)
					{
						if(tmp>0)temp_x = map->object[i].x+1;
						else temp_x = map->object[i].x-1;

						if(!check_object_object_collision(temp_x, map->object[i].y,i))
						{
							map->object[i].x = temp_x;
							if(pushing_an_object<4)pushing_an_object=1;
						}
						
						
					}
						
				}
	}
	
}


void check_push_object_y(float x, float y,float tmp)
{
	int i;
	float temp_y;
	for(i=0;i<map->num_of_objects;i++)
	{
		if(map->object[i].active)
			if(map->object[i].pushable)
				if(check_angle_collision(x, y, char_info[0].w, char_info[0].h, 0, map->object[i].x, map->object[i].y, object_info[map->object[i].type].w,object_info[map->object[i].type].h,  ((int)(map->object[i].angle*((float)object_info[map->object[i].type].angles/360) )) *  ((float)360/object_info[map->object[i].type].angles)))
				{
					
					if(player.last_dy==0)
					{
						if(tmp>0)temp_y = map->object[i].y+1;
						else temp_y = map->object[i].y-1;

						if(!check_object_object_collision(map->object[i].x,temp_y,i))
						{
							map->object[i].y = temp_y;
							if(pushing_an_object<4)pushing_an_object=1;
						}
						
						
					}
					
				}
	}
	
}




/////////////////////////////////////////////////////
//Check if the player collides with enmys and stuff///
/////////////////////////////////////////////////////

int player_object_collision(float x,float y)
{
	int i,j;


	for(i=0;i<map->num_of_objects;i++)
	{
		if(map->object[i].active)
			if(object_info[map->object[i].type].solid>0 && object_info[map->object[i].type].animation[map->object[i].action].solid>0)
				if(check_angle_collision(x, y, char_info[0].w, char_info[0].h, 0, map->object[i].x, map->object[i].y, object_info[map->object[i].type].w,object_info[map->object[i].type].h,  map->object[i].angle))
					return 1;
	}
	
	for(j=0;j<current_map_enemy_num;j++)
	{
		i = current_map_enemy[j];
		if(!enemy_data[i].dead && enemy_data[i].active && enemy_data[i].used && !enemy_info[enemy_data[i].type].under_player)
			if(check_collision(x-char_info[0].w/2, y-char_info[0].h/2, char_info[0].w, char_info[0].h, enemy_data[i].x-enemy_info[enemy_data[i].type].w/2, enemy_data[i].y-enemy_info[enemy_data[i].type].h/2,enemy_info[enemy_data[i].type].w,enemy_info[enemy_data[i].type].h)) 
					return 1;
	}

	for(j=0;j<current_map_npc_num;j++)
	{
		i = current_map_npc[j];
		if(npc_data[i].active && npc_data[i].used && !npc_data[i].dead)
			if(check_collision(x-char_info[0].w/2, y-char_info[0].h/2, char_info[0].w, char_info[0].h, npc_data[i].x-char_info[npc_data[i].type].w/2, npc_data[i].y-char_info[npc_data[i].type].h/2,char_info[npc_data[i].type].w,char_info[npc_data[i].type].h)) 
					return 1;
	}

	
	return 0;
}




//change the player action to name
void change_player_action(char *name)
{
	if(player.action!=char_action(&char_info[0],name))
	{
		player.action = char_action(&char_info[0],name);
		player.frame =0;
		player.nextframe =0;
	}
}





///////////////////////////////////////////////
/////// ITEM FUNCTION ////////////////////////
/////////////////////////////////////////////
void pick_up_item(void)
{
	int i,j,type,temp;
	int	item_space=-1;
	int pickup_x;
	int pickup_y;
	float temp_x,temp_y;
	char mess_string[80];

	int got_picked_up=0;
	
	xyplus(PLAYER_USE_LENGTH, player.angle, &temp_x, &temp_y);

	pickup_x = player.x +temp_x;
	pickup_y = player.y +temp_y;

		
	for(i=0;i<MAX_ITEM_DATA;i++)
	{
		if(!item_data[i].picked_up && item_data[i].active && item_data[i].used)
			if(strcmp(map->name, item_data[i].map_name)==0)
				if(check_collision(pickup_x -PLAYER_PICKUP_W/2, pickup_y - PLAYER_PICKUP_H/2, PLAYER_PICKUP_W, PLAYER_PICKUP_H,
					item_data[i].x - item_info[item_data[i].type].w/2, item_data[i].y - item_info[item_data[i].type].h/2, item_info[item_data[i].type].w,item_info[item_data[i].type].h))
				{
					type = item_info[item_data[i].type].type;

					
					////// you pick up a normal item ////////
					if(type == 0)
					{
						player.item_space[player.num_of_items].item = i;
						player.item_space[player.num_of_items].active = 1;
	
						player.num_of_items++;

					}
					
					////// you pick up a weapon ////////
					if(type == 1)
					{
						player.weapon_space[player.num_of_weapons].item = i;
						item_data[i].active = 1;
	
						player.num_of_weapons++;
					}
					
					////// you pick up a ammo ////////
					if(type == 2)
					{
						temp=-1;
						for(j=0;j<player.num_of_ammo;j++)
							if(strcmp(item_info[item_data[player.ammo_space[j].item].type].s_string,item_info[item_data[i].type].s_string)==0)
							{
								temp=j;
								break;
							}
						
						//I the typ of ammo don't exist in the inventory make anew space
						if(temp==-1)
						{
							player.ammo_space[player.num_of_ammo].item = i;
							player.ammo_space[player.num_of_ammo].num = item_data[i].value;
							player.ammo_space[player.num_of_ammo].active = 1;
	
							player.num_of_ammo++;
						}
						else //make the type of ammo ammount larger.
						{
							player.ammo_space[temp].num+=item_data[i].value;
						}

					}
					
					////// you pick up a note ////////
					if(type == 3)
					{
						player.note_space[player.num_of_notes].item = i;
						player.note_space[player.num_of_notes].active = 1;
						player.note_space[player.num_of_notes].num = 0;	
						player.num_of_notes++;
					}
					
					item_data[i].picked_up=1;

					//if(item_info[item_data[i].type].type!=0)
					//else
					//		sprintf(mess_string, "You picked up %s.", item_info[item_data[i].type].name);
					
					sprintf(mess_string, "You picked up %s.", item_data[i].name);
					
						
				
					make_new_message(mess_string,NULL);
					got_picked_up = 1;
					break;
				}

	}


	if(!got_picked_up)
	{
		i = RANDOM(0, 4);

		if(i==0)
			make_new_message("Nothing to pick up here.",NULL);
		else if(i==1)
			make_new_message("Can't pick up anything.",NULL);
		else if(i==2)
			make_new_message("Nothing interesting here.",NULL);
		else if(i==3)
			make_new_message("There is nothing useful here.",NULL);
		else if(i==4)
			make_new_message("Nothing useful here.",NULL);
	}

			
}

///////////////////////////////////////////////////
////// DEATH /////////////////////////
////////////////////////////////////////////////////

void player_death(void)
{
	int end=0;
	int time=0;
	
	float alpha=0;
	int ans;
	
	
	
	clear(screen);

	stop_all_sounds();
	stop_fiend_music();

	play_fiend_sound("player_die1",0,0,0,0,200);

	speed_counter = 0;

	while(!end)
	{
		
		while(speed_counter>0)
		{
			
			time++;
			
			if(time<100)
			{
				

			}
			else if(time<400)
			{
				alpha+=1;				
			}
			else if(time<600)
			{
				alpha-=3;
			}
			else
			{
				end = 1;
			}
			
			if(alpha>255)alpha=255;
			if(alpha<0)alpha=0;
			
	
			speed_counter--;
		}
		
		clear(virt);
		textout_centre_ex(virt,font_avalon2->dat,"Y o u   d i e d", 220,240,makecol((alpha/255)*200,(alpha/255)*30,(alpha/255)*30), -1);
	
		blit(virt,screen,0,0,80,0,480,480);
	}

	ans = fiend_menu(0);

	if(ans ==-1) game_ended = 1;
		
	if(fiend_new_game)
	{
		show_intro_text();
					
		if(!load_edit_map(map, "maps/lauder_alive/lauder.map"))
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
	
		reset_saved_vars();
	
		update_before_map();

		load_item_data();
				
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
		if(ans == 1)load_game("save/save1.sav");
		else if(ans == 2)load_game("save/save2.sav");
		else if(ans == 3)load_game("save/save3.sav");
		else if(ans == 4)load_game("save/save4.sav");
		else if(ans == 5)load_game("save/save5.sav");
	}
	

	speed_counter =0;
}




///////////////////////////////////////////////////
////// GLOBAL KEYS UPDATE /////////////////////////
////////////////////////////////////////////////////

void update_global_keys(void)
{
	char c;
	static int F12_down=0;
	static int F6_down=0;
	static int F7_down=0;

	char temp_pic_file[20];
	int pic_count=0;
	int ans;
	
	if(key[KEY_TILDE])
    {
		if(debug_is_on)
		{
			csl_start(NULL,0);
			return;
		}
    }
	
	if(!key[KEY_ESC] && esc_down)esc_down =0;
	if(key[KEY_ESC] && !esc_down)
	{
		ans = fiend_menu(1);

		if(ans ==-1) game_ended = 1;
		
		if(fiend_new_game)
		{
			show_intro_text();
					
			if(!load_edit_map(map, "maps/lauder_alive/lauder.map"))
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
	
			reset_saved_vars();
	
			update_before_map();

			load_item_data();
				
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
			if(ans == 1)load_game("save/save1.sav");
			else if(ans == 2)load_game("save/save2.sav");
			else if(ans == 3)load_game("save/save3.sav");
			else if(ans == 4)load_game("save/save4.sav");
			else if(ans == 5)load_game("save/save5.sav");
		}
	

		speed_counter =0;

		esc_down=1;
	}

	
	if(!key[KEY_F12])F12_down=0;
	if(key[KEY_F12] && !F12_down)
		{
			do
			{
				sprintf(temp_pic_file,"screen%d.pcx",pic_count);
				pic_count++;
			}while(exists(temp_pic_file)!=0);
						
			save_pcx(temp_pic_file,virt,NULL);
			F12_down=1;
		}
		
	/*if(!key[KEY_F6])F6_down=0;
	if(key[KEY_F6] && !F6_down)
		{
			if(!save_game("save.f","Quick Save"))allegro_message("couldn't save savegame");
			make_engine_error("Saved current game to \"save.f\"");
			F6_down = 1;
		}
	
	if(!key[KEY_F7])F7_down=0;
	if(key[KEY_F7] && !F7_down)
		{
			if(!load_game("save.f"))allegro_message("couldn't load savegame");
			make_engine_error("Loaded game from \"save.f\"");
			F7_down = 1;
		}*/

}

///////////////////////////////////////////////////
////// MAIN PLAYER UPDATE /////////////////////////
////////////////////////////////////////////////////

void update_player(void)
{
	char walk_action[20] ="moving";
	char temp_string[30] = "";
	
	float temp_x, temp_y;
	float angle;
		
	static int temp=0;
	float speed = char_info[0].speed;
	float turn_speed = char_info[0].speed*2;

	
	int tile_set;
	int tile_num;

	int i;
	
	static int time_between_forward=100;
	
	static int weapon_has_shot=0;
	static int release_shell=0;

    if(csl_started)
     return;

	player.last_dx =0;
	player.last_dy =0;

	walk_forward_pressed =0;

	
	/////////////////////////////////////////////
	//-------BEGIN PLAYER FIRST TESTS------------
	////////////////////////////////////////////
	
	//--The Hitback--
	/*if(player.hit_speed>0)
	{
		player.active = 0;

		xyplus(player.hit_speed, player.hit_angle, &temp_x, &temp_y);
		
		if(!check_tile_collision(player.x + temp_x-char_info[0].w/2, player.y-char_info[0].h/2,char_info[0].w, char_info[0].h)&& !player_object_collision(player.x + temp_x, player.y) )
		{
			player.x+=temp_x;
			player.last_dx =temp_x;
		}
		
		if(!check_tile_collision(player.x-char_info[0].w/2, player.y + temp_y -char_info[0].h/2  ,char_info[0].w, char_info[0].h) && !player_object_collision(player.x , player.y+temp_y))
		{
			player.y+=temp_y;
			player.last_dy =temp_y;
		}

		if(!player.dead)change_player_action("hit");
		
		player.hit_speed-=0.03;
		
		if(player.hit_speed<0)
			player.hit_speed=0;
	}
	else
	{
		player.active = 1;
	}*/

	//--The Death--
	if(player.energy<0 && !player.dead)
	{
		if( player.energy<-char_info[0].energy*2 )
		{
			player.was_dead=1;
			player.dead=1;
			
			play_fiend_sound("splash",player.x,player.y, 1,0,180);
			make_flesh_explosion(player.x,player.y,1,player.hit_angle);
			
		}
		else
		{
			change_player_action("dead");
			player.dead=1;
			play_fiend_sound(char_info[0].sound_death,0,0,0,0,230);
			player_death();
		}
	}

	
	if(player.dead && player.hit_speed==0 && !player.was_dead)
	{
		player.was_dead=1;
		make_bloodpool(player.x, player.y, (int)((float)char_info[0].w *0.4),makecol(200,0,0));
	}	

	if(player.active_weapon==-1)
	{
		player.weapon_drawn =0;
	}
	


	/////////////////////////////////////////////
	//-------END PLAYER FIRST TESTS------------
	////////////////////////////////////////////
	
	
	/////////////////////////////////////////////
	//-------BEGIN PLAYER INPUT CODE------------
	////////////////////////////////////////////
	
	////// Non player inputs //////////////
	

	if(!key[key_inventory])inventory_key_down=0;
	if(key[key_inventory] && !inventory_key_down)
		{
			if(fiend_note_is_on)
			{
				play_fiend_sound("menu_back",0,0,0,0,200);
				inventory_is_on=1;
				fiend_note_is_on=0;
				clear_note_data();
			}		
			else if(inventory_is_on)
			{
				play_fiend_sound("menu_back",0,0,0,0,200);
				menu_is_open=0;
				inventory_is_on=0;
				player.active=1;
				current_menu=0;
				menu_row=0;
			}
			else if(!inventory_is_on && !player.dead)
			{
				play_fiend_sound("menu_forward",0,0,0,0,200);
				inventory_is_on=1;
				//player.active=0;
			}
			inventory_key_down=1;
		}
	

	
	// ///// Player inputs //////////////
	if(player.active && !inventory_is_on && !player.dead && !player.animation && !fiend_note_is_on)
	{
	
		
		/////// ------- SOME FIRST CHECKS -------- //////
		
		//Determin the players speed

		if(player.energy<20)
			speed*=0.5;
		else if(player.energy<50)
			speed*=0.7;

		if(player.active_weapon!=-1)//MAke the wepaons weigght slow down the player
		{
			if(player.weapon_drawn)
				speed*=1-((float)weapon_info[get_weapon_num(item_info[item_data[player.active_weapon].type].s_string)].weight/100);
		}
		if(player.running && player.can_shoot>weapon_info[temp].shot_length && !player.weapon_drawn) 
		{
			//if(player.active_weapon==-1)
				speed*=1.85;
			//else
			//	speed*=1.4;
		
			strcpy(walk_action,"running");

			change_player_action("running");
		}
		

		/////// ------- ACTION/USE INPUT -------- //////
		
		player_has_used=0;	
		player_has_used_item=0;	

		gun_fired=0;

		
		//set the current weapon to temp...
		temp = get_weapon_num(item_info[item_data[player.weapon_space[player.active_weapon].item].type].s_string);
		
	
		// ---- action key begin: -------//

		if(key[key_action] && !action_key_down)
		{
			xyplus(PLAYER_USE_LENGTH, player.angle, &temp_x, &temp_y);

			player_use_x = player.x +temp_x;
			player_use_y = player.y +temp_y;
						
			player_has_used=1;	
			action_key_down=1;
		}

		if(!key[key_action] && action_key_down)
		{
			action_key_down=0;
		}

		// -----action key end ---------//

		
		
		
		// ---- attack key begin: -------//

		if(player.weapon_drawn)
		{
			if(key[key_attack])
			{
			
				if(player.active_weapon!=-1 && player.weapon_drawn)
				{
					if(player.can_shoot>=weapon_info[temp].shot_length && item_data[player.weapon_space[player.active_weapon].item].value>0)
					{
						if(!weapon_info[temp].silent)
							gun_fired=1;
					
						//Show a lightmap and flame
						compute_towerxy(player.angle,weapon_info[temp].flame_x,weapon_info[temp].flame_y,&temp_x, &temp_y);

						make_normal_light(player.x + temp_x, player.y + temp_y, weapon_info[temp].light_c, weapon_info[temp].light_r, weapon_info[temp].flame_length);
					
						make_flame(weapon_info[temp].flame_num,player.x+temp_x, player.y+temp_y,weapon_info[temp].flame_length);					
					
						//change the player action
						change_player_action(weapon_info[temp].player_action);

						//Play the weapon sound
						play_fiend_sound(weapon_info[temp].sound,0,0,0,0,230);

						//Make a missile
						for(i=0;i<weapon_info[temp].num_of_missiles;i++)
						{
							//angle = ((float)i/weapon_info[temp].num_of_missiles)*(float)weapon_info[temp].missile_angle;
						
							//angle = (weapon_info[temp].missile_angle/2)-angle;

							angle = RANDOM(-weapon_info[temp].missile_angle, weapon_info[temp].missile_angle);

							compute_towerxy( player.angle ,weapon_info[temp].missile_x,weapon_info[temp].missile_y,&temp_x, &temp_y);
							make_new_missile(player.x+temp_x, player.y+temp_y, add_angle(player.angle,angle), temp);
							//allegro_message("Working");
						}

						//dec the ammo
						item_data[player.weapon_space[player.active_weapon].item].value--;
										
						player.can_shoot=0;

						weapon_has_shot=1;
						release_shell = weapon_info[temp].shell_after_shot_length;
					}
				}
			}
		}
		else
		{
			if(key[key_attack] && !attack_key_down)
			{
				pick_up_item();
								
				attack_key_down = 1;
			}
			if(!key[key_attack])attack_key_down=0;
		}

		if(weapon_has_shot && release_shell<1)
		{	
			//shoot a shell
			compute_towerxy(player.angle,weapon_info[temp].shell_x,weapon_info[temp].shell_y,&temp_x, &temp_y);

			make_shell(weapon_info[temp].shell_name,player.x+temp_x, player.y+temp_y, (float)(RANDOM(8,12))/10, add_angle(player.angle, 90), 1,(float)(RANDOM(16,19))/10,(float)(RANDOM(14,20))/10,190);
			weapon_has_shot=0;		
		}
		else
		{
			release_shell--;
		}
		
		if(player.active_weapon!=-1 && player.weapon_drawn)
		{
			player.can_shoot++;
			if(player.can_shoot>10000)player.can_shoot=10000;
		}
		else
		{
			player.can_shoot=10000;
		}

		
		// ----- attack key end ---------//

		
		// ---- drawweapon key begin: -------//
		if(!key[key_pickup] && pickup_key_down)
		{
			pickup_key_down=0;
		}

		if(key[key_pickup] && !pickup_key_down)
		{
			if(player.active_weapon!=-1 && !weapon_has_shot)
			{
				if(player.weapon_drawn==0)
					player.weapon_drawn=1;
				else
					player.weapon_drawn=0;
			}
			
			pickup_key_down=1;
		}
		// -----drawweapon key end ---------//

		
		
		/////// ------- MOVEMENT INPUT -------- //////

		//CHeck if a weapon is drawn, if so change walk action
		if(player.active_weapon!=-1 && player.weapon_drawn)
		{
			strcpy(walk_action,"moving_");
			strcat(walk_action, weapon_info[temp].name);
		}
		
		// -----left key begin ---------//
		
		if(key[key_left])
		{
			if(!key[key_strafe])
			{
				player.angle = add_angle(player.angle, -turn_speed);
			}
			else if(!walk_forward_pressed  && !player.running && player.can_shoot>weapon_info[temp].shot_length)
			{
				xyplus(speed, add_angle(player.angle,-90), &temp_x, &temp_y);
				
				if(!check_tile_collision(player.x + temp_x-char_info[0].w/2, player.y-char_info[0].h/2,char_info[0].w, char_info[0].h)&& !player_object_collision(player.x + temp_x, player.y) )
				{
					player.x+=temp_x;
					player.last_dx =temp_x;
				}
		
				if(!check_tile_collision(player.x-char_info[0].w/2, player.y + temp_y -char_info[0].h/2  ,char_info[0].w, char_info[0].h) && !player_object_collision(player.x , player.y+temp_y))
				{
					player.y+=temp_y;
					player.last_dy =temp_y;
				}
				walk_forward_pressed =1;

			}

			walk_key_pressed =1;
		
			change_player_action(walk_action);
		}
		// -----left key end ---------//
		
		
		
		
		
		
		
		
		
		// -----right key begin ---------//
		
		if(key[key_right]  )
		{
			
			if(!key[key_strafe])
			{
				player.angle = add_angle(player.angle, turn_speed);
			}
			else if(!walk_forward_pressed && !player.running && player.can_shoot>weapon_info[temp].shot_length)
			{
				xyplus(speed, add_angle(player.angle,90), &temp_x, &temp_y);
				
				if(!check_tile_collision(player.x + temp_x-char_info[0].w/2, player.y-char_info[0].h/2,char_info[0].w, char_info[0].h)&& !player_object_collision(player.x + temp_x, player.y) )
				{
					player.x+=temp_x;
					player.last_dx =temp_x;
				}
		
				if(!check_tile_collision(player.x-char_info[0].w/2, player.y + temp_y -char_info[0].h/2  ,char_info[0].w, char_info[0].h) && !player_object_collision(player.x , player.y+temp_y))
				{
					player.y+=temp_y;
					player.last_dy =temp_y;
				}
				walk_forward_pressed =1;

			}

			walk_key_pressed =1;
		
			change_player_action(walk_action);

		}
		// -----action key end ---------//
		
		
		
		
		
		
		
		
		
		
		if(player.can_shoot>weapon_info[temp].shot_length)
		{

		// -----forward key begin ---------//
		
		//clear a flag.... 
		//if(pushing_an_object>-5)
		pushing_an_object=0;


		

		
		
		//used for player.running
		if(!key[key_forward])
		{
			time_between_forward++;
			if(time_between_forward>200)time_between_forward=200;
			forward_key_down=0;
			player.running=0;
		}
				
		if(key[key_forward] && !walk_forward_pressed)
		{
			xyplus(speed, player.angle, &temp_x, &temp_y);
				
			if(!check_tile_collision(player.x + temp_x-char_info[0].w/2, player.y-char_info[0].h/2,char_info[0].w, char_info[0].h) && !player_object_collision(player.x + temp_x, player.y))
			{
			player.x+=temp_x;
			player.last_dx =temp_x;
			}
			else //check if you are pushing an object
			{
				check_push_object_x(player.x + temp_x, player.y,temp_x);	
			}

			if(!check_tile_collision(player.x-char_info[0].w/2, player.y + temp_y -char_info[0].h/2  ,char_info[0].w, char_info[0].h) && !player_object_collision(player.x , player.y+temp_y))
			{
			player.y+=temp_y;
			player.last_dy =temp_y;
			}
			else //check if you are pushing an object
			{
				check_push_object_y(player.x, player.y+temp_y,temp_y);	
			}

				
			
			walk_key_pressed =1;
			walk_forward_pressed =1;
		
			change_player_action(walk_action);

			
			//check if you will run....
			if(!forward_key_down && time_between_forward<7)
				//if(player.active_weapon==-1)
					player.running=1;
			
			time_between_forward=0;
			forward_key_down=1;

					
		}
		// -----forward key end ---------//

		
		
		
		
		
		
		
		
		// -----backward key begin ---------//
		
		if(key[key_backward] && !walk_forward_pressed)
		{
			speed*=0.6;
			xyplus(-speed, player.angle, &temp_x, &temp_y);
				
			if(!check_tile_collision(player.x + temp_x-char_info[0].w/2, player.y-char_info[0].h/2,char_info[0].w, char_info[0].h)&& !player_object_collision(player.x + temp_x, player.y) )
			{
				player.x+=temp_x;
				player.last_dx =temp_x;
			}
		
			if(!check_tile_collision(player.x-char_info[0].w/2, player.y + temp_y -char_info[0].h/2  ,char_info[0].w, char_info[0].h) && !player_object_collision(player.x , player.y+temp_y))
			{
				player.y+=temp_y;
				player.last_dy =temp_y;
			}


			walk_key_pressed =1;
			walk_forward_pressed =1;

		
			change_player_action(walk_action);
		
		}
		// -----backward key end ---------//

	
	}
	

		// ---- reload key begin: -------//
		if(key[key_reload] && !reload_key_down )
		{
			if(player.active_weapon!=-1 && item_data[player.active_weapon].sp1>0 && item_data[player.active_weapon].value<weapon_info[temp].max_ammo)
			{
				change_player_action("reload");

				play_fiend_sound("player_reload",0,0,0,0,230);

				player.reloading=1;
				player.animation=1;

			}
			
			reload_key_down=1;
		}

		if(!key[key_reload] && reload_key_down)
		{
			reload_key_down=0;
		}
		// ---- reload key end -------//

		
	}

  

	
	///////////////////////////////////////////
	//-------END PLAYER INPUT CODE------------
	//////////////////////////////////////////
	
	
	
	/*
	
	//// ---- begin pushing sound -------
		if(pushing_an_object>0 && the_push_voice<0)
			the_push_voice=play_fiend_sound("pushing", 0,0,0,0,200);

		if(voice_get_position(the_push_voice)==-1)
		{
			stop_sound_num(the_push_voice);
			the_push_voice=-1;
		
		}

	//// ------ end pushing sound --------

		
	*/
	


	////////////////////////////////////////////
	//------ UPDATE THE PLAYER ------------///
	//////////////////////////////////////////

	//temp = get_weapon_num(item_info[item_data[player.active_weapon].type].s_string);
	temp = get_weapon_num(item_info[item_data[player.weapon_space[player.active_weapon].item].type].s_string);
		

	//if the player is not doinf anything special set to still animation
	if(!player.animation && !walk_key_pressed && player.hit_speed==0 && !player.dead && player.can_shoot>weapon_info[temp].shot_length)
	{
		if(player.active_weapon<0 || !player.weapon_drawn)
		{
			change_player_action("still_none");
		}
		else
		{
			strcpy(temp_string,"still_");
			strcat(temp_string,weapon_info[temp].name);

			change_player_action(temp_string);
			
		}
	}

	player.nextframe++;
	if(player.nextframe>=ANIM_SPEED)
	{
		//update frame	
		player.nextframe =0;
		player.frame++;
		if(char_info[0].animation[player.action].frame[player.frame] ==255)
		{
			player.frame = char_info[0].animation[player.action].frame[player.frame + 1 ];

			//if reloading stop it!
			if(player.reloading)
			{
				player.reloading=0;
				reload_weapon();
			}
			if(player.animation!=0)
			{
				
				if(player.animation>-1)player.animation--;
				
				if(!player.dead && player.animation==0)
				{
					if(player.active_weapon<0 || !player.weapon_drawn)
					{
						change_player_action("still_none");
					}
					else
					{
						strcpy(temp_string,"still_");
						strcat(temp_string,weapon_info[temp].name);

						change_player_action(temp_string);
					}	
				}
			}
		
		
		}
		

		//footstep sound
		tile_set = (map->layer1+ ((int)player.x/TILE_SIZE) +( ((int)player.y/TILE_SIZE) * map->w))->tile_set;
		tile_num = (map->layer1+ ((int)player.x/TILE_SIZE) +( ((int)player.y/TILE_SIZE) * map->w))->tile_num;
		
		if(strncasecmp(char_info[0].animation[player.action].name, "moving",6)==0 && (player.frame==char_info[0].walk_step1 || player.frame==char_info[0].walk_step2))
			play_fiend_sound(tile_info[tile_set].tile[tile_num].sound,0,0,0,0,180);


		if(strcmp(char_info[0].animation[player.action].name, "running")==0 && (player.frame==char_info[0].run_step1 || player.frame==char_info[0].run_step2))
			play_fiend_sound(tile_info[tile_set].tile[tile_num].sound,0,0,0,0,180);

		
	}

	walk_key_pressed=0;
	


	map_x = player.x - 240;
	map_y = player.y - 240;
	
	
}



//////////////////////////////////////////////////////
///////// AUTOMOVE STUFF //////////////////////////////
///////////////////////////////////////////////////////

//THis auto moves the player x,y with speed speed...
//NOTE that the player controlls are not turned of....
void auto_move_player(int x, int y,float speed)
{
	auto_move_speed = speed;
	auto_move_angle = compute_angle(x,y,player.x,player.y);
	auto_move_counter = distance(player.x, player.y, x,y)/speed;
	auto_move_active =1;
	player.active = 0;

}

//Stops an auto move
void stop_auto_move(void)
{
	auto_move_active=0;
}

//resumes a stopped auto move
void resume_auto_move(void)
{
	auto_move_active=1;
}

//Here the auto movement gets updated
void update_auto_move(void)
{	
	float temp_x;
	float temp_y;
	int dist;

	
	//check if auto move is active
	if(auto_move_active)
	{
		//if the angle is not right turn the player.....
		if(auto_move_angle!=player.angle)
		{
			//if The angle is close to the "goal" angle than just the set the right angle
			//else tyrn him with speed
			if( compute_angle_distance(player.angle, auto_move_angle)<=char_info[0].speed*4)
				player.angle = auto_move_angle;
			else
			{
				dist = compute_angle_distance(auto_move_angle, player.angle);
		
				if(  (add_angle(player.angle, dist)<auto_move_angle+2) && (add_angle(player.angle, dist)>auto_move_angle-2) ) 
					player.angle= add_angle(player.angle, char_info[0].speed*2);
				else
					player.angle= add_angle(player.angle, -char_info[0].speed*2);

			}

			
			//make the player become animated
			walk_key_pressed =1;
			change_player_action("moving");

			
		}//if you have more to move
		else if(auto_move_counter>0)
		{
			//check for collision....if no collision -move
			xyplus(auto_move_speed, player.angle, &temp_x, &temp_y);	
			
			if(!check_tile_collision(player.x + temp_x-char_info[0].w/2, player.y-char_info[0].h/2,char_info[0].w, char_info[0].h) )
			{
			player.x+=temp_x;
			player.last_dx =temp_x;
			}
		
			if(!check_tile_collision(player.x-char_info[0].w/2, player.y + temp_y -char_info[0].h/2  ,char_info[0].w, char_info[0].h) )
			{
			player.y+=temp_y;
			player.last_dy =temp_y;
			}
			
			//animate the player
			walk_key_pressed =1;
			change_player_action("moving");

			//one move less to make....
			auto_move_counter--;
		
		}
		else//make auto move not active
		{
			auto_move_active=0;
			player.active = 1;
		}
	}


}

