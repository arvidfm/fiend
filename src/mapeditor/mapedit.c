#include <stdio.h>
#include <allegro.h>
#include "../fiend.h"
#include "../grafik4.h"


char map_file[80];


//Some Varibales
int map_x = 0;
int map_y = 0;

int current_tileset=0;
int current_tile=0;
int current_shadow=-1;
int current_layer=1;

int current_object=0;

int current_editmode=0;

int x_grid=1;
int y_grid=1;

DATAFILE *misc;


BITMAP *the_buffer;

//File
int file_exit_proc(void);
int file_new_proc(void);
int file_load_map(void);
int file_save_proc(void);

//Edit
int edit_change_mapname_proc(void);
int edit_change_mapnsize_proc(void);
int edit_clear_map_cord(void);

int edit_triggers(void);
int edit_global_triggers(void);
int edit_local_vars(void);
int edit_global_vars(void);


//Options
int options_light_proc(void);
int options_grid_proc(void);


//Misc
int misc_about_proc(void);
int misc_credits_proc(void);
int misc_minimap_proc(void);
int edit_global_enemies_proc(void);
int edit_global_items_proc(void);
int edit_global_npcs_proc(void);

int d_save_brush_button_proc(int msg,DIALOG *d,int c);
int d_load_brush_button_proc(int msg,DIALOG *d,int c);
int d_reset_brush_button_proc(int msg,DIALOG *d,int c);


//Draw
int d_drawmap_proc(int,DIALOG*,int);

//Dialog
int update_the_screen(void);

int d_print_mapname_proc(int msg,DIALOG *d,int c);
int d_print_mapsize_proc(int msg,DIALOG *d,int c);
int d_print_tileinfo_proc(int msg,DIALOG *d,int c);
int d_print_editmode_proc(int msg,DIALOG *d,int c);
int d_print_mousepos_proc(int msg,DIALOG *d,int c);

void init_brushes(void);

int key_up_proc(void);
int key_down_proc(void);
int key_right_proc(void);
int key_left_proc(void);

int d_shadow_list_proc(int msg, DIALOG *d, int c);

char *tileset_list(int index, int *list_size);
int d_tileset_list_proc(int msg, DIALOG *d, int c);

int d_tile_list_proc(int msg, DIALOG *d, int c);

int d_layer_radio_proc(int msg,DIALOG *d,int c);



char *character_list(int index, int *list_size);
int d_character_list_proc(int msg, DIALOG *d, int c);
char *image_list(int index, int *list_size);
int d_image_list_proc(int msg, DIALOG *d, int c);


char *object_list(int index, int *list_size);
int d_object_list_proc(int msg, DIALOG *d, int c);


char *npc_list(int index, int *list_size);
int d_npc_list_proc(int msg, DIALOG *d, int c);

char *enemy_list(int index, int *list_size);
int d_enemy_list_proc(int msg, DIALOG *d, int c);

char *item_list(int index, int *list_size);
int d_item_list_proc(int msg, DIALOG *d, int c);

char *brush_list(int index, int *list_size);
int d_brush_list_proc(int msg, DIALOG *d, int c);

int editmode_tiles(void);
int editmode_lights(void);
int editmode_player(void);
int editmode_links(void);
int editmode_npcs(void);
int editmode_objects(void);
int editmode_soundemitors(void);
int editmode_areas(void);
int editmode_enemy(void);
int editmode_items(void);
int editmode_pathnodes(void);
int editmode_brushes(void);
int editmode_shadows(void);
int editmode_look_at_area(void);


MENU editmode_menu[]=
{
 //text     proc    menu-child     flags      dp
{"Tiles",  editmode_tiles,    NULL,     D_SELECTED,         NULL},
{"Objects", editmode_objects,  NULL,     0,         NULL},
{"Items",  editmode_items,  NULL,       0,         NULL},
{"NPC:s",  editmode_npcs,  NULL,  0,         NULL},
{"Lights",  editmode_lights,  NULL,      0,         NULL},
{"Player Pos",  editmode_player,  NULL,      0,         NULL},
{"Links",  editmode_links,  NULL,      0,         NULL},
{"Areas",  editmode_areas,  NULL,  0,         NULL},
{"Look At Areas",  editmode_look_at_area,  NULL,  0,         NULL},
{"SoundEmitors",  editmode_soundemitors,  NULL,  0,         NULL},
{"Enemies",  editmode_enemy,  NULL, 0,         NULL},
{"Path Nodes",  editmode_pathnodes,  NULL, 0,         NULL},
{"Brushes",  editmode_brushes,  NULL, 0,         NULL},
{"Shadows",  editmode_shadows,  NULL, 0,         NULL},
{"Enemy path",  NULL,  NULL,  D_DISABLED,         NULL},
{"NPC path",  NULL,  NULL,  D_DISABLED,         NULL},
{NULL,      NULL,   NULL,     0,         NULL}
};



MENU file_menu[]=
{
 //text     proc    menu-child     flags      dp
{ "&New map",   file_new_proc,   NULL,     0,         NULL},
{ "&Load map",  file_load_map,   NULL,     0,         NULL},
{ "&Save map",  file_save_proc,   NULL,     0,         NULL},
{ "",           NULL,   NULL,     0,         NULL},
{ "&Exit",      file_exit_proc,   NULL,     0,    NULL},
{ NULL,      NULL,   NULL,          0,         NULL}
};




MENU edit_menu[]=
{
 //text     proc    menu-child     flags      dp
{"Change map &size", edit_change_mapnsize_proc,   NULL,     0,         NULL},
{"Change map &name", edit_change_mapname_proc,   NULL,     0,         NULL},
{"Clear map &cords", edit_clear_map_cord,   NULL,     0,         NULL},
{"",  NULL,   NULL,     0,         NULL},
{"Edit triggers", edit_triggers,   NULL,     0,         NULL},
{"Edit global triggers", edit_global_triggers,   NULL,     0,         NULL},
{"Edit local vars", edit_local_vars,   NULL,     0,         NULL},
{"Edit global vars", edit_global_vars,   NULL,     0,         NULL},
{"",  NULL,   NULL,     0,         NULL},
{"&Update Screen (F5)",update_the_screen,   NULL,     0,         NULL},
{  NULL,      NULL,   NULL,          0,         NULL}
};

MENU options_menu[]=
{
 //text     proc    menu-child     flags      dp
{"&Grid Size",  options_grid_proc,   NULL,     0,         NULL},
{"&Map Options",  options_light_proc,   NULL,     0,         NULL},
{"&Edit Mode...",  NULL,  editmode_menu,     0,         NULL},
{NULL,      NULL,   NULL,          0,         NULL}
};



MENU misc_menu[]=
{
 //text     proc    menu-child     flags      dp
{"&About",  misc_about_proc,   NULL,     0,         NULL},
{"&Credits",  misc_credits_proc,   NULL,     0,         NULL},
{"",  NULL,   NULL,     0,         NULL},
{"&Minimap",  misc_minimap_proc,   NULL,     0,         NULL},
{"",  NULL,   NULL,     0,         NULL},
{"&Edit global enemies",edit_global_enemies_proc,   NULL,     0,   NULL},
{"&Edit global npcs",edit_global_npcs_proc,   NULL,     0,   NULL},
{"&Edit global items",edit_global_items_proc,   NULL,     0,   NULL},
{NULL,      NULL,   NULL,          0,         NULL}
};





MENU main_menu[]=
{
 //text     proc    menu-child     flags      dp
{ "&File",   NULL,   file_menu,     0,         NULL},
{ "&Edit",   NULL,   edit_menu,  0,         NULL},
{ "&Options",  NULL, options_menu,          0,         NULL},
{ "&Misc",   NULL,   misc_menu,          0,         NULL},
{ NULL,      NULL,   NULL,          0,         NULL}
};










DIALOG main_dialog[] =
{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)  (dp) */
   //The standard stuff
	{ d_clear_proc,       0,    0,    0,    0,    FG_COLOR,  BG_COLOR,    0,    0,       0,    0,    NULL },
   { d_drawmap_proc,     10,   40,    480, 480,   FG_COLOR,  BG_COLOR,    0,    0,       0,    0,    "HEJ"},
   { d_menu_proc,        0,   0,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,       0,    0,    main_menu},
   
   //upadate screen on f5
	{ d_keyboard_proc,    0,   0,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  KEY_F5,    0,    update_the_screen},
   
   //print the map name
	{ d_print_mapname_proc,230, 2,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    NULL},
   
   //scroll the mapon arrow keys
	{ d_keyboard_proc,    0,   0,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  KEY_UP,    0,    key_up_proc},
   { d_keyboard_proc,    0,   0,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  KEY_DOWN,    0,    key_down_proc},
   { d_keyboard_proc,    0,   0,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  KEY_RIGHT,    0,    key_right_proc},
   { d_keyboard_proc,    0,   0,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  KEY_LEFT,    0,    key_left_proc},
   

	//Choose the tileset
	{ d_text_proc,    505,   18,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Tileset:"},
    { d_tileset_list_proc,    500,   26,     287,     62,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    tileset_list},
   
	//Choose the tile
	{ d_tile_list_proc,    500,   90,     128,   256,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    NULL},
   
	//Choose the tilelayer
	{ d_box_proc,    10,   525,   300, 70,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    NULL},
    { d_text_proc,    12,   530,   0, 0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "TileLayer:"},
    { d_layer_radio_proc,  105,   530,   8, 8,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "1"},
    { d_layer_radio_proc,  135,   530,   8, 8,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "2"},
    { d_layer_radio_proc,  165,   530,   8, 8,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "3"},
    
	//print map w and h
	{ d_print_mapsize_proc,230, 15,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    NULL},
   	
	//print tile info
	{ d_print_tileinfo_proc,500, 350,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    NULL},
   	
	{ d_print_editmode_proc,12, 540,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    NULL},

   	//Choose Object
	{ d_text_proc,    640, 360,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Object:"},
    { d_object_list_proc,    635,   368,     153,     146,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    object_list},
      
	//Choose NPC
	{ d_text_proc,    640,   90,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "NPC:"},
    { d_npc_list_proc,    635,  98,     153,     50,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    npc_list},
    
	//Choose enemy
	{ d_text_proc,    640,   161,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Enemy:"},
    { d_enemy_list_proc,    635,  169,     153,     50,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    enemy_list},
    
	//Choose item
	{ d_text_proc,    640,   224,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Item:"},
    { d_item_list_proc,    635,  232,     153,     113, FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    item_list},
    
	//Choose brush
	{ d_text_proc,    505,   360,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Brush:"},
    { d_brush_list_proc,   500,  368,     128,     146, FG_COLOR,  BG_COLOR,    0,  D_EXIT,  0,    0,    brush_list},
    
	{  d_save_brush_button_proc,500, 517,  40,   14,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,         "Save"},
	{  d_load_brush_button_proc,542, 517,  40,   14,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,         "Load"},
	{  d_reset_brush_button_proc,584, 517,  43,   14,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,         "Reset"},
		

	{ d_print_mousepos_proc,12, 549,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    NULL},
	
	//Choose the shadow
	{ d_shadow_list_proc, 315,  532, 480, 64,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    NULL},
    { d_text_proc,    315,   523,     0,     0,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Shadow:"},
    {NULL,                0,    0,    0,    0,    0,    0,    0,    0,       0,    0,    NULL }
};






//redraws the whole screen
int update_the_screen(void)
{
 broadcast_dialog_message(MSG_DRAW, 0);
 return D_REDRAW;
}


int init_mapeditor(void)
{
	
	///Init some allegro stuff,
	allegro_init();
	install_mouse();
    install_keyboard();
    install_timer();
    
	set_window_title("Fiend Mapedit");

	//init the graphic mode
	set_color_depth(16);
    if(set_gfx_mode(GFX_DIRECTX,800,600,0,0)!=0)
	{
	   set_color_depth(15);
       if(set_gfx_mode(GFX_DIRECTX,800,600,0,0)!=0)
         return 1;
	}
	
	if(!FSOUND_Init(44100, 32, 0))
	{
		return 1;
	}
		
    init_grafik4();
	
	//Load the graphics....
	if(load_tiles())return 1;
	if(load_characters())return 1;
	if(load_objects())return 1;
	if(load_sounds())return 1;
	if(load_enemys())return 1;
	if(load_items())return 1;
	if(load_message_faces())return 1;

	
	init_npc_data();
	init_enemy_data();
	init_item_data();
	init_global_vars();
	init_global_triggers();

	init_brushes();

	load_npc();
	load_enemy_data();
	load_item_data();
	load_global_vars();
	load_global_triggers();
	
	create_wall_shadows();
		
	misc =load_datafile("graphic/misc.dat");
	
	//Make a map...
	map = calloc(sizeof(MAP_DATA),1);
	new_map(map);

	//Set some Gui options
	gui_fg_color = makecol(0,0,0);
	gui_bg_color = makecol(255,255,255);

	sort_objects();
	sort_tiles();

	the_buffer = create_bitmap(200,400);
	
    return 0;
}

void exit_mapeditor(void)
{	
	int i;

	for(i=0;i<map->num_of_lights;i++)//release the lightmaps!!!
		 destroy_bitmap(lightmap_data[i]);
	
	free_sounds();
	unload_datafile(misc);
	
	release_enemy_data();
	release_item_data();
	release_npc();
	release_global_vars();
	release_global_triggers();

	release_message_faces();
	release_items();
	release_enemys();
	release_characters();
	release_objects();
	release_tiles();
	release_map(map);
}



int main(int argc, char *argv[])
{
	int ret;
	
    //Init...
	if(init_mapeditor())
	{
		allegro_message(fiend_errorcode);
		return 1;
	}
    
	//begin
	ret=do_dialog(main_dialog, -1);	

	//exit.....
	exit_mapeditor();
	return 0;
}
END_OF_MAIN();


