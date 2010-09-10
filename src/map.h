#include "lightmap.h"
#include "object.h"
#include "mapeditor/mapedit.h"
#include "trigger.h"

#ifndef MAP_H
#define MAP_H 


#define MAX_PATHNODE_NUM 500

typedef struct
{
 char name[30];
 int active;
 int x;
 int y;
 int w;
 int h;
 char message[120];
}LOOK_AT_AREA_DATA;


typedef struct
{
	int used;
	int x;
	int y;
}PATH_NODE;


typedef struct
{
 int active;
 int x;
 int y;
 int loop;
 char sound_name[40];
 int emitor_type; //Can be : 1-The sound is normal     0-The sound is always heard
 int sound_type; //CAn be 0-WAVE    1-MOD
 int voice_num;
}SOUNDEMITOR_DATA;



typedef struct
{
 char name[40];
 int active;
 int x;
 int y;
 int w;
 int h;
}AREA_DATA;


typedef struct
{
	int type; //What type 0=Go to new map... 1=Stop and display message..
	char message[LIMK_MESSAGE_LENGTH]; //the message...
	char map_file[80]; //The map file....
	char link_name[20]; //name of the link on the new map...
}LINK_EVENT;

typedef struct
{
 char name[20];
 int x;//the cords
 int y;
 int w;//w and h
 int h;
 int event_chosen; //what event is default......
 int type; //What tpye of link 0 = Verical (h) 1 = Horisontal (w)
 LINK_EVENT event[5]; 
}LINK_DATA;



typedef struct
{
 int tile_set; //The tileset
 int tile_num; //the tile in the set
}TILE_DATA;


typedef struct
{
 int length; //string length
 char *string; //the string
}SCRIPT_DATA;



typedef struct
{
 char name[40];//the name of the map

 int w;  //width
 int h;  //height
 
 int player_x;//The player pos...
 int player_y;
 int player_angle;

 int outside; //The lightlevel is the same for all outside maps....
 
 int light_level; //the ambient lightlevel
 
 int num_of_lights;
 LIGHT_DATA *light; //The lights
  
 TILE_DATA *layer1;
 TILE_DATA *layer2; //the diffrent layers
 TILE_DATA *layer3;

 char *shadow;
 
 int num_of_objects;
 OBJECT_DATA *object; //The objects

 int num_of_areas;
 AREA_DATA *area; //The areas
 
 int num_of_look_at_areas;
 LOOK_AT_AREA_DATA *look_at_area; //The areas

 int num_of_links;
 LINK_DATA *link; //The Links

 int num_of_soundemitors;
 SOUNDEMITOR_DATA *soundemitor;//The SoundEmitors

 int num_of_triggers;  
 TRIGGER_DATA* trigger;  //The triggers
 
 VARIABLE_DATA var[LOCAL_VAR_NUM];  //The local variables

 int num_of_path_nodes;  
 PATH_NODE path_node[MAX_PATHNODE_NUM]; //the path nodes...

}MAP_DATA;






#endif


 





