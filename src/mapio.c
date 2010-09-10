////////////////////////////////////////////////////
// This file contains map loading and saving 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "fiend.h"
#include "lightmap.h"


extern int map_x;
extern int map_y;


//Save map *map to *file
int save_map(MAP_DATA *map, char* file)
{
	FILE *f;
	f = fopen(file, "wb");
	if(f==NULL)return 0;

	fwrite(map, sizeof(MAP_DATA), 1,f);
	fwrite(map->light, sizeof(LIGHT_DATA), map->num_of_lights, f);		
	
    fwrite(map->layer1, sizeof(TILE_DATA), map->w*map->h,f);
	fwrite(map->layer2, sizeof(TILE_DATA), map->w*map->h,f);
	fwrite(map->layer3, sizeof(TILE_DATA), map->w*map->h,f);

	fwrite(map->shadow, sizeof(char), map->w*map->h,f);

	fwrite(map->object, sizeof(OBJECT_DATA),map->num_of_objects, f);
	fwrite(map->area, sizeof(AREA_DATA),map->num_of_areas, f);
	fwrite(map->look_at_area, sizeof(LOOK_AT_AREA_DATA),map->num_of_look_at_areas, f);
	fwrite(map->link, sizeof(LINK_DATA),map->num_of_links, f);
	fwrite(map->soundemitor, sizeof(SOUNDEMITOR_DATA),map->num_of_soundemitors, f);

	fwrite(map->trigger, sizeof(TRIGGER_DATA),map->num_of_triggers, f);
	

	fclose(f);
	return 1;
}


//Loads *file to *map. Memory must be alloctaed for the map struct
//like MAP_DATA map...
int load_map(MAP_DATA* map, char *file)
{

	int i;
	FILE *f;
	
	for(i=0;i<map->num_of_lights;i++)
		destroy_bitmap(lightmap_data[i]);

	
	
	f = fopen(file, "rb");
	if(f==NULL){ sprintf(fiend_errorcode,"couldn't load %s",map);return 0;}

	fread(map, sizeof(MAP_DATA), 1,f);

	map->light = calloc(sizeof(LIGHT_DATA), map->num_of_lights); 
	fread(map->light, sizeof(LIGHT_DATA), map->num_of_lights, f);		
	
	map->layer1 = calloc(sizeof(TILE_DATA), map->w*map->h); 
	fread(map->layer1, sizeof(TILE_DATA), map->w*map->h,f);
		
	map->layer2 = calloc(sizeof(TILE_DATA), map->w*map->h); 
	fread(map->layer2, sizeof(TILE_DATA), map->w*map->h,f);
		
	map->layer3 = calloc(sizeof(TILE_DATA), map->w*map->h); 
	fread(map->layer3, sizeof(TILE_DATA), map->w*map->h,f);

	map->shadow = calloc(sizeof(char), map->w*map->h); 
	fread(map->shadow, sizeof(char), map->w*map->h,f);

		
	map->object = calloc(sizeof(OBJECT_DATA), map->num_of_objects); 
	fread(map->object, sizeof(OBJECT_DATA),map->num_of_objects, f);
		
	map->area = calloc(sizeof(AREA_DATA), map->num_of_areas); 
	fread(map->area, sizeof(AREA_DATA),map->num_of_areas, f);
	
	map->look_at_area = calloc(sizeof(LOOK_AT_AREA_DATA), map->num_of_look_at_areas); 
	fread(map->look_at_area, sizeof(LOOK_AT_AREA_DATA),map->num_of_look_at_areas, f);
	
	map->link = calloc(sizeof(LINK_DATA), map->num_of_links); 
	fread(map->link, sizeof(LINK_DATA),map->num_of_links, f);

	map->soundemitor = calloc(sizeof(SOUNDEMITOR_DATA), map->num_of_soundemitors); 
	fread(map->soundemitor, sizeof(SOUNDEMITOR_DATA),map->num_of_soundemitors, f);
	
	map->trigger = calloc(sizeof(TRIGGER_DATA), map->num_of_triggers); 
	fread(map->trigger, sizeof(TRIGGER_DATA),map->num_of_triggers, f);
		
	
	
	for(i=0;i<map->num_of_lights;i++)
		create_light_map2(&lightmap_data[i], &map->light[i]);
	
	

	fclose(f);
	return 1;



}


//loads a map reday for editing...this map takes up lots of memory...
//new_map must have been called
int load_edit_map(MAP_DATA* map, char *file)
{

	int i;
	MAP_DATA *temp_map;
	FILE *f;
	
	for(i=0;i<map->num_of_lights;i++)
		destroy_bitmap(lightmap_data[i]);

	
	
	f = fopen(file, "rb");
	if(f==NULL){sprintf(fiend_errorcode,"couldn't load %s",map);return 0;}

	// Do some new funny stuff.....
	
	temp_map = calloc(sizeof(MAP_DATA),1);
	fread(temp_map, sizeof(MAP_DATA), 1,f);
	
	map->w = temp_map->w;
	map->h = temp_map->h;
	map->num_of_lights = temp_map->num_of_lights;
	map->num_of_areas = temp_map->num_of_areas;
	map->num_of_look_at_areas = temp_map->num_of_look_at_areas;
	map->num_of_links = temp_map->num_of_links;
	map->num_of_objects = temp_map->num_of_objects;
	map->num_of_soundemitors = temp_map->num_of_soundemitors;
	map->num_of_triggers = temp_map->num_of_triggers;
	map->num_of_path_nodes = temp_map->num_of_path_nodes;
	
	map->player_x = temp_map->player_x;
	map->player_y = temp_map->player_y;
	map->player_angle = temp_map->player_angle;
	
	map->light_level = temp_map->light_level;
	map->outside = temp_map->outside;
	
	strcpy(map->name, temp_map->name);

	memcpy(map->var, temp_map->var, sizeof(VARIABLE_DATA)*LOCAL_VAR_NUM);
	memcpy(map->path_node, temp_map->path_node, sizeof(PATH_NODE)*MAX_PATHNODE_NUM);
	
	free(temp_map);
	
	//stop doing that funny stuff----
		
		
	fread(map->light, sizeof(LIGHT_DATA), map->num_of_lights, f);
	
	
	fread(map->layer1, sizeof(TILE_DATA), map->w*map->h,f);
	
	fread(map->layer2, sizeof(TILE_DATA), map->w*map->h,f);
	
	fread(map->layer3, sizeof(TILE_DATA), map->w*map->h,f);
	
	fread(map->shadow, sizeof(char), map->w*map->h,f);
		
	fread(map->object, sizeof(OBJECT_DATA),map->num_of_objects, f);
	
	fread(map->area, sizeof(AREA_DATA),map->num_of_areas, f);
	
	fread(map->look_at_area, sizeof(LOOK_AT_AREA_DATA),map->num_of_look_at_areas, f);
	
	fread(map->link, sizeof(LINK_DATA),map->num_of_links, f);

	fread(map->soundemitor, sizeof(SOUNDEMITOR_DATA),map->num_of_soundemitors, f);
	
	fread(map->trigger, sizeof(TRIGGER_DATA),map->num_of_triggers, f);
				
	
	map_x=0;
    map_y=0;
    
	if(in_the_game)
	{
		for(i=0;i<map->num_of_lights;i++)
			create_light_map2(&lightmap_data[i], &map->light[i]);

		sprintf(map_file,"%s",file);
	}
	else
	{
		for(i=0;i<map->num_of_lights;i++)
			create_light_map(&lightmap_data[i], &map->light[i]);
	}

		
	
	fclose(f);

	update_tile_object_height();

	return 1;

}





//this is being lazy....I use fixed sizes on all of the data...
//these fixed sizes are the max size.. so the map takes up alot of 
//memory, but it is saved in as compact size as possible.....
void new_map(MAP_DATA* map)
{
 int i;

 strcpy(map->name, "noname");

 map->light = calloc(sizeof(LIGHT_DATA), MAX_LIGHT_NUM); 
 
 map->num_of_lights = 0;
 
 map->light_level=31;

 map->h = 20;
 map->w = 20;

 map->player_x = 80;
 map->player_y = 80;
 
 map->player_angle = 0;

 map->outside = 0;


 
 map->layer1 = calloc(sizeof(TILE_DATA), MAX_LAYER_H*MAX_LAYER_W); 
 map->layer2 = calloc(sizeof(TILE_DATA), MAX_LAYER_H*MAX_LAYER_W); 
 map->layer3 = calloc(sizeof(TILE_DATA), MAX_LAYER_H*MAX_LAYER_W); 

 map->shadow = calloc(sizeof(char), MAX_LAYER_H*MAX_LAYER_W); 
				
 map->object = calloc(sizeof(OBJECT_DATA), MAX_OBJECT_NUM); 
 map->area = calloc(sizeof(AREA_DATA), MAX_AREA_NUM); 
 map->look_at_area = calloc(sizeof(LOOK_AT_AREA_DATA), MAX_LOOK_AT_AREA_NUM); 
 map->link = calloc(sizeof(LINK_DATA), MAX_LINK_NUM); 
 map->soundemitor = calloc(sizeof(SOUNDEMITOR_DATA), MAX_SOUNDEMITOR_NUM); 
 map->trigger = calloc(sizeof(TRIGGER_DATA), MAX_TRIGGER_NUM); 

 
 
  map->num_of_objects = 0;
  map->num_of_areas = 0;
  map->num_of_look_at_areas = 0;
  map->num_of_links =0;
  map->num_of_soundemitors =0;
  map->num_of_triggers =0;
  map->num_of_path_nodes =0;
	
 for(i=0;i<LOCAL_VAR_NUM;i++)
 {
	map->var[i].value=0;
	sprintf(map->var[i].name,"null",i);
 }

 for(i=0;i<MAX_PATHNODE_NUM;i++)
 {
	map->path_node[i].used=0;
	map->path_node[i].x=0;
	map->path_node[i].y=0;

 }
 
  
 for(i=0;i<MAX_LAYER_H*MAX_LAYER_W;i++)
  {
    map->layer1[i].tile_set=0;
	map->layer1[i].tile_num=2;
	

	map->layer2[i].tile_set=0;
	map->layer2[i].tile_num=0;
	
	map->layer3[i].tile_set=0;
	map->layer3[i].tile_num=0;

	map->shadow[i]=-1;

	
  }

	/*acquire_screen();
	for(i=0;i<40;i++)
	textprintf(screen,font,10+i*27,200,makecol(255,40,0),"%d-%d,",map->layer1[i].tile_set, map->layer1[i].tile_num);
	release_screen();
	readkey();*/
}



//reset or clear some varialbles
void clear_map(MAP_DATA *map)
{
 int i;

 for(i=0;i<map->num_of_lights;i++)
	destroy_bitmap(lightmap_data[i]);
	
 strcpy(map->name,"noname");
 
 map->light_level=31;
 map->outside = 0;
 
 map->h=20;
 map->w=20;

 map->player_x = 80;
 map->player_y = 80;

 map->player_angle = 0;


 map->num_of_lights = 0;
 
 map->num_of_objects = 0;
 map->num_of_areas = 0;
 map->num_of_look_at_areas = 0;
 map->num_of_links =0;
 map->num_of_soundemitors =0;
 map->num_of_triggers =0;
 map->num_of_path_nodes =0;

 
 for(i=0;i<LOCAL_VAR_NUM;i++)
 {
	map->var[i].value=0;
	sprintf(map->var[i].name,"null");
 }

 for(i=0;i<MAX_PATHNODE_NUM;i++)
 {
	map->path_node[i].used=0;
	map->path_node[i].x=0;
	map->path_node[i].y=0;
 }
  
 for(i=0;i<MAX_LAYER_H*MAX_LAYER_W;i++)
  {
    map->layer1[i].tile_set=0;
	map->layer1[i].tile_num=2;
	

	map->layer2[i].tile_set=0;
	map->layer2[i].tile_num=0;
	
	map->layer3[i].tile_set=0;
	map->layer3[i].tile_num=0;
	
	map->shadow[i]=-1;
	
  }


 map_x=0;
 map_y=0;


}



//frees the memory that map has taken up
//NOT the MAP_DATA struct though
void release_map(MAP_DATA *map)
{
  int i;
 	
  for(i=0;i<map->num_of_lights;i++)
	destroy_bitmap(lightmap_data[i]);

 
	
  free(map->light);
  
  
  free(map->layer1);
  free(map->layer2);
  free(map->layer3);

  free(map->shadow);

  free(map->object);
  free(map->area);
  free(map->look_at_area);
  free(map->link);
  free(map->soundemitor);
  free(map->trigger);
  
  
}




