#include <allegro.h>
#include <fmod.h>
#include <math.h>

#include "map.h"
#include "tile.h"
#include "lightmap.h"
#include "character.h"
#include "object.h"
#include "fiend/player.h"
#include "fiend/collision.h"
#include "NPC.h"
#include "sound.h"
#include "enemy.h"
#include "item.h"
#include "trigger.h"
#include "bmp_array.h"
#include "fiend/inventory.h"
#include "fiend/weapon.h"
#include "fiend/missile.h"
#include "fiend/particle.h"
#include "fiend/npc_update.h"
#include "fiend/enemy_update.h"
#include "fiend/ai.h"
#include "fiend/effect.h"
#include "fiend/los.h"
#include "fiend/astar.h"
#include "fiend/savegame.h"
#include "fiend/menu.h"
#include "fiend/notes.h"
#include "fiend/intro.h"


//-----Some defines------
#ifndef FIEND_DEFINES
#define FIEND_DEFINES

#ifdef WIN32
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

#define FIEND_VERSION      0
#define FIEND_SUB_VERSION  1
#define FIEND_WIP_VERSION  0

//mapeditor
#define FG_COLOR 0
#define BG_COLOR 0xffffff


//the game
#define TILE_SIZE 32


#define MAX_LIGHT_NUM 70
#define MAX_SCRIPT_NUM 30
#define MAX_SCRIPT_LENGTH 100
#define MAX_AREA_NUM 60
#define MAX_LOOK_AT_AREA_NUM 60
#define MAX_LINK_NUM 20
#define MAX_OBJECT_NUM 200
#define MAX_SOUNDEMITOR_NUM 40


#define MAX_LAYER_H 100
#define MAX_LAYER_W 100

#define ANIM_SPEED 3

#endif


//-----Extern STuff----


// Info sfuff
extern TILESET_INFO *tile_info;
extern RLE_ARRAY *tile_data[100];
extern CHARACTER_INFO *char_info;
extern PLAYER_INFO player;



//Some Data
extern BITMAP *virt;
extern BITMAP *temp_bitmap;
extern BITMAP *mask;
extern DATAFILE *font_avalon;
extern DATAFILE *font_arial;
extern DATAFILE *font_avalon2;
extern DATAFILE *font_small1;
extern DATAFILE *font_small2;




//-----Globals---------

extern int current_map_npc[];
extern int current_map_enemy[];
extern int current_map_item[];

extern int current_map_npc_num;
extern int current_map_enemy_num;
extern int current_map_item_num;

extern long int fiend_playtime;

extern int game_complete;
		


//Gloabal vars that can trigger events
int gun_fired;
int npc_damaged;

int outside_lightlevel;


//Object Globals
extern int num_of_tilesets;
extern int num_of_characters;
extern int num_of_soundemitors;
extern int num_of_areas;

extern volatile int speed_counter;

extern int map_x;
extern int map_y;

extern MAP_DATA *map;
extern BITMAP *lightmap_data[MAX_LIGHT_NUM];
extern BITMAP *lightmap_cache;

extern char map_file[80];
extern char global_var_filename[80];
extern char global_trigger_filename[80];
extern char npc_filename[80];
extern char item_filename[80];
extern char enemy_filename[80];

extern int game_ended;

extern int color_depth;


extern int in_the_game;

extern int message_active;


extern char fiend_errorcode[];

extern int los_buffer[18][18];


//confidg stuff
extern int vsync_is_on;
extern int sound_is_on;
extern int debug_is_on;
extern int lightning_is_on;
extern int cache_lights_is_on;

int fiend_sound_volume;
int fiend_sound_buffer_size;
int fiend_music_volume;
int fiend_sound_driver;

int fiend_gfx_driver;

int fiend_show_intro;

extern int key_forward;
extern int key_backward;
extern int key_left;
extern int key_right;
extern int key_attack;
extern int key_action;
extern int key_pickup;
extern int key_inventory;
extern int key_strafe;



//------Routines--------

//MapIO routines 

int save_map(MAP_DATA*, char*); //saves the map 
int load_map(MAP_DATA*, char*); //loads a map
void new_map(MAP_DATA* map); //makes a new map
int load_edit_map(MAP_DATA* map, char *file);//loads a map for editing.....
void clear_map(MAP_DATA *map);//clears some map variables
void release_map(MAP_DATA *map); // free some memory...

 
//Tile routines

int load_tiles(void);
void release_tiles(void);
void draw_tile_layer(BITMAP *virt, int layer,int solid, int xpos, int ypos);
int check_tile_collision(float x, float y, int w, int h);


//Character routines
int load_characters(void);
void release_characters(void);
void draw_fiend_char(BITMAP *dest,CHARACTER_INFO *temp,int x, int y,int action, int frame, float angle);
int char_action(CHARACTER_INFO *temp, char *name);

//Fiend routines
int init_fiend(void);
void exit_fiend(void);


//Misc routines
#define MAX_SHELL_NUM 100
typedef struct 
{
	int used;
	int type;
	float x;
	float y;
	float z;
	float max_z; 
	int dir; //0 = down 1 = up
	float angle;
	float speed;
	int time;
}SHELL_DATA;

extern SHELL_DATA shell_data[];


#define MAX_FLAME_DATA 30
typedef struct 
{
	int used;
	int x;
	int y;
	int time;
	int type;
	int num;
}FLAMEDATA;

extern FLAMEDATA flame_data[MAX_FLAME_DATA];

#define BLOODPOOL_NUM 100

typedef struct
{
	int used;

	float size;
	float max_size;
	float speed;

	int color;
	
	int x;
	int y;
}BLOODPOOL;

extern BLOODPOOL bloodpool[BLOODPOOL_NUM];

#define BEAM_NUM 60

typedef struct
{
	int used;

	int type;
	
	int x1;
	int y1;
	int x2;
	int y2;

	int frame;
	int next_frame;

	int time;
}BEAM;

extern BEAM beam[BEAM_NUM];

extern FMOD_SYSTEM *fmod_system;
extern FMOD_CHANNEL *fmod_channel;
extern FMOD_CREATESOUNDEXINFO soundex_info;


void init_frame_speed();
void reset_frame_speed();
double show_frame_speed();

void load_config_file(void);
void save_config_file(void);

void get_args(int argc, char *argv[]);
void update_load_text(char *string);


void update_before_map(void);

void check_look_at_areas(void);

//--engine error
void make_engine_error(char *string);
void update_engine_error(void);
void draw_engine_error(void);

//--cropping
void crop_picdata(BITMAP *bmp, PIC_DATA *pic);

//--pickup
void make_pickup_message(char *string,BITMAP *pic, int time);
void update_pickup_message(void);
void draw_pickup_message(void);


//--beams
void reset_beams(void);
void update_beams(void);
void draw_beams(void);
void make_new_beam(char *name,int x1,int y1, int x2, int y2, int time);

//--hits and blood
void make_bullet_hit_object(int i,int power);
void make_bullet_hit_flesh(int i,int power, int color);

void draw_bloodpools(void);
void update_bloodpools(void);
void reset_bloodpools(void);
void make_bloodpool(int x, int y, int size, int color);
void make_flesh_explosion(int x, int y, int size,int angle);

//--shell
void reset_shells(void);
void update_shells(void);
void make_shell(char *name, float x, float y, float z, float angle,int dir, float speed,float max_z,int time);
void draw_shells(void);

//--flame
int load_flames(void);
void release_flames(void);
void reset_flames(void);
void make_flame(int type, int x, int y, int time);
void update_flames(void);
void draw_flames(void);


//Draw level routines
void draw_level(void);

//Player routines
void update_player(void);
void auto_move_player(int x, int y,float speed);
void stop_auto_move(void);
void resume_auto_move(void);
void update_auto_move(void);



//lightmap
void create_light_map(BITMAP **dest, LIGHT_DATA *light);
void create_light_map2(BITMAP **dest, LIGHT_DATA *light);

//Link
void check_link_collison(void);


#define FACE_NUM 60

typedef struct
{
	char name[40];
	BITMAP *pic;
}FACE_DATA;


extern FACE_DATA message_face[FACE_NUM];
extern int num_of_message_faces;

//Message
void draw_message_box(BITMAP *dest, int x, int y, int w, int h);
void draw_message(void);
void update_message(void);
void make_new_message(char *string, char *face);

int load_message_faces(void);
void release_message_faces(void);

//update
void get_current_map_objects(void);


// Log file
void set_log_file(char *filename);
void log_event(char *msg, ...);

//TEST TEST TEST
void draw_object_rect(BITMAP *dest, float x1, float y1, int w1, int h1, float angle1);

