////////////////////////////////////////////////////
// This file contains the inventory code 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"

#define MENU_MAIN 0
#define MENU_ITEMS 1
#define MENU_WEAPONS 2
#define MENU_NOTES 3
#define MENU_ITEM_ACTION 4
#define MENU_WEAPON_ACTION 5
#define MENU_NOTE_ACTION 6
#define MENU_SAVEGAME 7

#define ITEM_ROWS 10

static int max_rows[] = {3,0,0,0,2,3,1,5};

extern int action_key_down;	
extern int pickup_key_down;	
extern int key_inventory;

int inventory_is_on=0;
int menu_is_open=0;
int menu_row =0;
static float current_color_add=1.5;
static float current_color=120;
int left_space;
int current_space;
int item_row=0;
int ammo_row=0;
int weapon_row=0;
int note_row=0;
static int action_item;

int current_menu =0;

static BITMAP *inv_weapons;
static BITMAP *inv_items;
static BITMAP *inv_notes;

int load_inventory_gfx(void)
{
	inv_items = load_bitmap("graphic/menu/inv_items.bmp",NULL);
	if(inv_items==NULL){strcpy(fiend_errorcode,"couldn't load inv_items.bmp");return 1;}
	inv_weapons = load_bitmap("graphic/menu/inv_weapons.bmp",NULL);
	if(inv_weapons==NULL){strcpy(fiend_errorcode,"couldn't load inv_wepons.bmp");return 1;}
	inv_notes = load_bitmap("graphic/menu/inv_notes.bmp",NULL);
	if(inv_notes==NULL){strcpy(fiend_errorcode,"couldn't load inv_notes.bmp");return 1;}
	
	return 0;
}



static void draw_folder(BITMAP *dest,BITMAP *src,int x,int y,int chosen,int centre)
{
	if(chosen==1)
	{
		set_trans_blender(221,111,0,0);
		draw_lit_sprite(dest,src,x-src->w/2,y,current_color-100);
	}
	else if(chosen==-1)
	{
		set_trans_blender(0,0,0,0);
		draw_lit_sprite(dest,src,x-src->w/2,y,128);
	}
	else
	{
		draw_sprite(dest,src,x-src->w/2,y);
	}
	
	
}

 

static void print_big_text(BITMAP *dest,char *text,int x,int y,int chosen,int centre)
{
	if(centre)
	{
		if(chosen==1)
		{
			textprintf_centre_ex(virt,font_avalon->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x,y,makecol(current_color,current_color,0),-1,text);
		}
		else if(chosen==-1)
		{
			textprintf_centre_ex(virt,font_avalon->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x,y,makecol(140,140,140),-1,text);
		}
		else
		{
			textprintf_centre_ex(virt,font_avalon->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x,y,makecol(255,255,255),-1,text);
		}
	}
	else
	{
		if(chosen==1)
		{
			textprintf_centre_ex(virt,font_avalon->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x,y,makecol(current_color,current_color,0),-1,text);
		}
		else if(chosen==-1)
		{
			textprintf_centre_ex(virt,font_avalon->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x,y,makecol(140,140,140),-1,text);
		}

		else
		{
			textprintf_centre_ex(virt,font_avalon->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_avalon->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_ex(virt,font_avalon->dat,x,y,makecol(255,255,255),-1,text);
		}
	}
}


static void print_small_text(BITMAP *dest,char *text,int x,int y,int chosen,int centre)
{
	if(centre)
	{
		if(chosen)
		{
			textprintf_centre_ex(virt,font_small2->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_small2->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_small2->dat,x,y,makecol(current_color,current_color,0),-1,text);
		}
		else
		{
			textprintf_centre_ex(virt,font_small2->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_small2->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_centre_ex(virt,font_small2->dat,x,y,makecol(255,255,255),-1,text);
		}
	}
	else
	{
		if(chosen)
		{
			textprintf_ex(virt,font_small2->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_ex(virt,font_small2->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_ex(virt,font_small2->dat,x,y,makecol(current_color*221,current_color*111,0),-1,text);
		}
		else
		{
			textprintf_ex(virt,font_small2->dat,x+1,y+1,makecol(0,0,0),-1,text);
			textprintf_ex(virt,font_small2->dat,x-1,y-1,makecol(0,0,0),-1,text);
			textprintf_ex(virt,font_small2->dat,x,y,makecol(255,255,255),-1,text);
		}
	}
}


static int get_weapon_ammo(int num)
{
	int i;

	for(i=0;i<player.num_of_ammo;i++)
		if( strcmp(item_info[item_data[num].type].name, item_info[item_data[player.ammo_space[i].item].type].s_string)==0)
		{
			return player.ammo_space[i].num;
		}
	
	return 0;
}


void update_inventory_graphic(void)
{
	int i,temp;
	char string[30];
	char string2[30];
	int s,m,h;

	FILE *f;

	SAVEDATA temp_save;
	
	//text_mode(-1);



	print_small_text(virt,"Condition:",190,70,0,0);

	if(player.energy>50)
	{
		textprintf_ex(virt,font_small2->dat,254,70,makecol((current_color/255)*40,(current_color/255)*60+190,(current_color/255)*40),-1,"Fine");
	}
	else if(player.energy>20)
	{
		textprintf_ex(virt,font_small2->dat,254,70,makecol((current_color/255)*60+180,(current_color/255)*60+80,(current_color/255)*20),-1,"Caution");
	}
	else
	{
		textprintf_ex(virt,font_small2->dat,254,70,makecol((current_color/255)*60+190,(current_color/255)*40,(current_color/255)*40),-1,"Danger");
	}
	///////The Main Menu ///////////////
	if(current_menu==MENU_MAIN)
	{
		if(menu_row==0)draw_folder(virt,inv_items,240,120,1,1);
		else	draw_folder(virt,inv_items,240,120,0,1);
		if(menu_row==1)draw_folder(virt,inv_weapons,240,150,1,1);
		else	draw_folder(virt,inv_weapons,240,150,0,1);
		if(menu_row==2)draw_folder(virt,inv_notes,240,180,1,1);
		else	draw_folder(virt,inv_notes,240,180,0,1);
	}
	
	//////The Item Menus //////////////
	if(current_menu==MENU_ITEMS)
	{
		draw_folder(virt,inv_items,240,90,-1,1);
		for(i=menu_row-ITEM_ROWS/2;i<menu_row+ITEM_ROWS/2;i++)
			if(i>=0 && i<player.num_of_items)
			{
				sprintf(string,"%s",item_data[player.item_space[i].item].name);

				temp = i-menu_row;
				if(i==menu_row)print_small_text(virt,string,240,temp*10+100+ITEM_ROWS/2*10+10,1,1);
				else print_small_text(virt,string,240,temp*10+100+ITEM_ROWS/2*10+10,0,1);
			}
	}
	if(current_menu==MENU_ITEM_ACTION)
	{
		draw_folder(virt,inv_items,240,90,-1,1);
	
		print_big_text(virt,item_data[player.item_space[action_item].item].name,240,110,-1,1);
		if(menu_row==0)print_big_text(virt,"Use",240,130,1,1);
		else	print_big_text(virt,"Use",240,130,0,1);
		if(menu_row==1)print_big_text(virt,"Check",240,150,1,1);
		else	print_big_text(virt,"Check",240,150,0,1);
		//if(menu_row==2)print_big_text(virt,"Drop",240,170,1,1);
		//else	print_big_text(virt,"Drop",240,170,0,1);
	}

	//////The Weapon Menus //////////////
	if(current_menu==MENU_WEAPONS)
	{
		draw_folder(virt,inv_weapons,240,90,-1,1);
		for(i=menu_row-ITEM_ROWS/2;i<menu_row+ITEM_ROWS/2;i++)
			if(i>=0 && i<player.num_of_weapons)
			{
				sprintf(string,"%s",item_data[player.weapon_space[i].item].name);

				temp = i-menu_row;

				if(i==menu_row)print_small_text(virt,string,240,temp*10+100+ITEM_ROWS/2*10+10,1,1);
				else print_small_text(virt,string,240,temp*10+100+ITEM_ROWS/2*10+10,0,1);

				if(i == player.active_weapon)
					print_small_text(virt,"(equipted)",240+text_length(font_small2->dat,string)/2+40,(temp*10+100+ITEM_ROWS/2*10+10),-1,1);
			}
	}
	if(current_menu==MENU_WEAPON_ACTION)
	{
		
		draw_folder(virt,inv_weapons,240,90,-1,1);
	
		temp = player.weapon_space[action_item].item;

		sprintf(string,"%s %d/%d",item_data[temp].name,item_data[temp].value, get_weapon_ammo(temp));
		
		print_big_text(virt,string,240,110,-1,1);
		
		if(player.active_weapon == action_item)	strcpy(string,"Unequip");
		else strcpy(string,"Equip");
		if(menu_row==0)print_big_text(virt,string,240,130,1,1);
		else	print_big_text(virt,string,240,130,0,1);
		if(menu_row==1)print_big_text(virt,"Check",240,150,1,1);
		else	print_big_text(virt,"Check",240,150,0,1);
		if(menu_row==2)print_big_text(virt,"Reload",240,170,1,1);
		else	print_big_text(virt,"Reload",240,170,0,1);
		//if(menu_row==3)print_big_text(virt,"Drop",240,190,1,1);
		//else	print_big_text(virt,"Drop",240,190,0,1);
	}
	
	//////The Note Menus //////////////
	if(current_menu==MENU_NOTES)
	{
		draw_folder(virt,inv_notes,240,90,-1,1);
		for(i=menu_row-ITEM_ROWS/2;i<menu_row+ITEM_ROWS/2;i++)
			if(i>=0 && i<player.num_of_notes)
			{
				sprintf(string,"%s",item_data[player.note_space[i].item].name);

				temp = i-menu_row;
				if(i==menu_row)print_small_text(virt,string,240,temp*10+100+ITEM_ROWS/2*10+10,1,1);
				else print_small_text(virt,string,240,temp*10+100+ITEM_ROWS/2*10+10,0,1);

				if(item_data[player.note_space[i].item].sp1 == 0)
					print_small_text(virt,"(unread)",240+text_length(font_small2->dat,string)/2+40,(temp*10+100+ITEM_ROWS/2*10+10),-1,1);
		
			}
	}
	if(current_menu==MENU_NOTE_ACTION)
	{
		draw_folder(virt,inv_notes,240,90,-1,1);
	
		print_big_text(virt,item_data[player.note_space[action_item].item].name,240,110,-1,1);
		if(menu_row==0)print_big_text(virt,"Read",240,130,1,1);
		else	print_big_text(virt,"Read",240,130,0,1);
		//if(menu_row==1)print_big_text(virt,"Drop",240,150,1,1);
		//else	print_big_text(virt,"Drop",240,150,0,1);
	}
	if(current_menu==MENU_SAVEGAME)
	{
		
		for(i=0;i<5;i++)
		{
			sprintf(string2,"save/save%d.sav",i+1);
		
			if(exists(string2))
			{
				f = fopen(string2, "rb");
	
				fread(&temp_save, sizeof(SAVEDATA),1,f);

				fclose(f);
				
				s = (temp_save.play_time/60)%60;
				m = (temp_save.play_time/(60*60))%60;
				h = temp_save.play_time/(60*60*60);
								
				sprintf(string,"%s %d:%d:%d",temp_save.name,h,m,s);
			}
			else
			{
				sprintf(string,"[empty]");
			}

			temp = i-menu_row;
			if(i==menu_row)print_small_text(virt,string,240,temp*10+100+ITEM_ROWS/2*10+10,1,1);
			else print_small_text(virt,string,240,temp*10+100+ITEM_ROWS/2*10+10,0,1);
		}

	}
	

	
	//text_mode(0);
	//textprintf(screen,font,0,30,makecol(255,255,255),"%d : %d  ", current_menu ,menu_row);
}




////// ITEM MENU FUNCTIONS //////////////////
//use an item in the menu
static void use_item(int num)
{
	float temp_x,temp_y;
	int i;

	if(strcmp(item_data[player.item_space[num].item].name,"Medicine_flask")==0)
	{
		player.energy = 100;
		make_new_message("You drink the contens of the flask and immidiently feel much better.",NULL);
		
		//Delete the item
		for(i=num;i<player.num_of_items-1;i++)
			{
				player.item_space[i].item = player.item_space[i+1].item;
				player.item_space[i].num = player.item_space[i+1].num;
				player.item_space[i].active = player.item_space[i+1].active;
			}
				player.num_of_items--;
			
	}
	else
	{
		xyplus(PLAYER_USE_LENGTH, player.angle, &temp_x, &temp_y);

		player_item_x = player.x +temp_x;
		player_item_y = player.y +temp_y;
	}
						
	player_has_used_item=1;	
	player_item_num = player.item_space[num].item;
}

//check (look at) an item in the menu
static void check_item(int num)
{
	make_new_message(item_data[player.item_space[num].item].desc,NULL);
}


//drop an item in the menu. 
static void drop_item(int num)
{
	int temp = player.item_space[num].item;
	int i;

	item_data[temp].x = player.x;
	item_data[temp].y = player.y;
	item_data[temp].angle = 0;
	item_data[temp].picked_up = 0;
	strcpy(item_data[temp].map_name,map->name);

	for(i=num;i<player.num_of_items-1;i++)
	{
		player.item_space[i].item = player.item_space[i+1].item;
		player.item_space[i].num = player.item_space[i+1].num;
		player.item_space[i].active = player.item_space[i+1].active;
	}

	player.num_of_items--;

}

////// WEAPON MENU FUNCTIONS //////////////////

//use a weapon in the menu
static void equipt_weapon(int num)
{
	int temp = player.weapon_space[num].item;
	
	if(player.active_weapon != num)
	{
		player.active_weapon = num;
	}
	else
	{
		player.active_weapon = -1;
	}
}

//reload a weapon in the menu
static void reload_weapon(int num)
{
	int temp,i;
	int type = item_data[player.weapon_space[num].item].type;
	int ammo;

	int weapon_num= get_weapon_num(item_info[type].s_string);
	
	
	temp=-1;
	for(i=0;i<player.num_of_ammo;i++)
		if(strcmp( item_info[item_data[player.ammo_space[i].item].type].s_string, item_info[type].name)==0)
		{
			temp = i;
			break;
		}
	
	if(temp==-1)return;
		
	ammo = player.ammo_space[temp].num;
	player.ammo_space[temp].num = 0;

	item_data[player.weapon_space[num].item].value += ammo;

	if(item_data[player.weapon_space[num].item].value>weapon_info[weapon_num].max_ammo)
	{
		player.ammo_space[temp].num = item_data[player.weapon_space[num].item].value - weapon_info[weapon_num].max_ammo;	
		item_data[player.weapon_space[num].item].value = weapon_info[weapon_num].max_ammo; 	
	}
}

//check (look at) a weapon in the menu
static void check_weapon(int num)
{
	make_new_message(item_data[player.weapon_space[num].item].desc,NULL);
}

//drop a weapon in the menu. 
static void drop_weapon(int num)
{
	int temp = player.weapon_space[num].item;
	int i;

	item_data[temp].x = player.x;
	item_data[temp].y = player.y;
	item_data[temp].angle = 0;
	item_data[temp].picked_up = 0;
	strcpy(item_data[temp].map_name,map->name);

	if(player.active_weapon == num)	player.active_weapon = -1;

	for(i=num;i<player.num_of_weapons-1;i++)
	{
		player.weapon_space[i].item = player.weapon_space[i+1].item;
		player.weapon_space[i].num = player.weapon_space[i+1].num;
		player.weapon_space[i].active = player.weapon_space[i+1].active;
	}

	player.num_of_weapons--;
}



////// NOTE MENU FUNCTIONS //////////////////

//use a weapon in the menu
static void read_note(int num)
{
	int temp = player.note_space[num].item;

	read_fiend_note(item_data[temp].string);
	item_data[temp].sp1 = 1;
}

//drop an item in the menu. 
static void drop_note(int num)
{
	int temp = player.note_space[num].item;
	int i;

	item_data[temp].x = player.x;
	item_data[temp].y = player.y;
	item_data[temp].angle = 0;
	item_data[temp].picked_up = 0;
	strcpy(item_data[temp].map_name,map->name);

	for(i=num;i<player.num_of_notes-1;i++)
	{
		player.note_space[i].item = player.note_space[i+1].item;
		player.note_space[i].num = player.note_space[i+1].num;
		player.note_space[i].active = player.note_space[i+1].active;
	}

	player.num_of_notes--;

}








void update_inventory_logic(void)
{
	static int right_down=1;
	static int left_down=1;
	static int up_down=1;
	static int down_down=1;
	static int pickup_down=1;

	char string[30];
	
	
	//some init stuff
	max_rows[MENU_ITEMS] = player.num_of_items;
	max_rows[MENU_WEAPONS] = player.num_of_weapons;
	max_rows[MENU_NOTES] = player.num_of_notes;
	
	
	//key presses!
	if(!key[key_forward])up_down=0;
	if(key[key_forward] && !up_down)
	{
		play_fiend_sound("menu_move",0,0,0,0,200);
	
		menu_row--;
		if(menu_row < 0)
			menu_row = max_rows[current_menu]-1;
		if(menu_row < 0)
			menu_row = 0;
			
		up_down = 1;
	}
	
	if(!key[key_backward])down_down=0;
	if(key[key_backward] && !down_down)
	{
		play_fiend_sound("menu_move",0,0,0,0,200);
	
		menu_row++;
		if(menu_row >= max_rows[current_menu])
			menu_row = 0;
		down_down = 1;
	}


	////The selct input!!
	if(!key[key_action])action_key_down=0;
	if(key[key_action] && !action_key_down)
	{
		play_fiend_sound("menu_forward",0,0,0,0,200);
	
		//The Main menu
		if(current_menu==MENU_MAIN)
		{
			if(menu_row==0)
			{
				current_menu=MENU_ITEMS;
				menu_row=player.num_of_items-1;
			}
			else if(menu_row==1)
			{
				current_menu=MENU_WEAPONS;
				
				if(player.active_weapon==-1)menu_row=player.num_of_weapons-1;
				else menu_row = player.active_weapon;
				
			}
			else if(menu_row==2)
			{
				current_menu=MENU_NOTES;
				menu_row=player.num_of_notes-1;
			}
		}
		else if(current_menu==MENU_ITEMS && menu_row>-1 && player.item_space[menu_row].item>-1)
		{
			current_menu=MENU_ITEM_ACTION;
			action_item = menu_row;
			menu_row =0;
		}
		else if(current_menu==MENU_WEAPONS && menu_row>-1 && player.weapon_space[menu_row].item>-1)
		{
			current_menu=MENU_WEAPON_ACTION;
			action_item = menu_row;
			menu_row = 0;
		}
		else if(current_menu==MENU_NOTES && menu_row>-1 && player.note_space[menu_row].item>-1)
		{
			current_menu=MENU_NOTE_ACTION;
			action_item = menu_row;
			menu_row =0;
		}
		else if(current_menu==MENU_ITEM_ACTION)
		{
			if(menu_row==0){
				use_item(action_item);
				inventory_is_on=0;
				player.active=1;
				current_menu=0;
				menu_row=0;}
			else if(menu_row==1)	{
				check_item(action_item);}
			else if(menu_row==2)	{
				drop_item(action_item);
				current_menu=MENU_ITEMS;
				menu_row=player.num_of_items-1;}
		}
		else if(current_menu==MENU_WEAPON_ACTION)
		{
			if(menu_row==0){
				equipt_weapon(action_item);}
			else if(menu_row==1)	{
				check_weapon(action_item);}
			else if(menu_row==2)	{
				reload_weapon(action_item);}
			else if(menu_row==3)	{
				drop_weapon(action_item);
				current_menu=MENU_WEAPONS;
				menu_row=player.num_of_weapons-1;}
		}
		else if(current_menu==MENU_NOTE_ACTION)
		{
			if(menu_row==0){
				read_note(action_item);}
			else if(menu_row==1)	{
				drop_note(action_item);
				current_menu=MENU_NOTES;
				menu_row=player.num_of_notes-1;}
		}
		else if(current_menu==MENU_SAVEGAME)
		{
			inventory_is_on=0;
			player.active=1;
						
			sprintf(string,"save/save%d.sav",menu_row+1);
			save_game(string,map->name);
			current_menu=0;
			menu_row=0;
			make_new_message("You type your progress using the typewriter.",NULL);
		}


		action_key_down = 1;
	}

	////The cancel input!!
	if(!key[key_pickup])pickup_key_down=0;
	if(key[key_pickup] && !pickup_key_down)
	{
		play_fiend_sound("menu_back",0,0,0,0,200);
	
		if(current_menu==MENU_ITEMS)
		{
			current_menu = 0;
			menu_row=0;
		}
		else if(current_menu==MENU_WEAPONS)
		{
			current_menu = 0;
			menu_row=1;
		}
		else if(current_menu==MENU_NOTES)
		{
			current_menu = 0;
			menu_row=2;
		}
		else if(current_menu==MENU_ITEM_ACTION)
		{
			current_menu = MENU_ITEMS;
			menu_row=action_item;
		}
		else if(current_menu==MENU_WEAPON_ACTION)
		{
			current_menu = MENU_WEAPONS;
			menu_row=action_item;
		}
		else if(current_menu== MENU_NOTE_ACTION)
		{
			current_menu = MENU_NOTES;
			menu_row=action_item;
		}
		else if(current_menu== MENU_SAVEGAME)
		{
			inventory_is_on=0;
			player.active=1;
			current_menu=0;
			menu_row=0;
		}
		else 
		{
			inventory_is_on=0;
			player.active=1;
			current_menu=0;
			menu_row=0;
		}
		
		pickup_key_down = 1;
	}




	//misc stuff
	current_color+=current_color_add;
	
	if(current_color>255)
	{
		current_color=255;
		current_color_add = -current_color_add;
	}
	if(current_color<120)
	{
		current_color=120;
		current_color_add = -current_color_add;
	}
}
