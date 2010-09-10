
#ifndef TILE_H
#define TILE_H 

// the structs
typedef struct
{
	int masked; //Is the sprite masked 0=no 1=yes
	int trans; //Is the sprite transperant 0=no 1,2...= level
	int solid; //Is it solid (not walkable) 0=no 1=yes 2=wall you cant see through
	int next_tile; //the next tile to show (-1 if not animated)
	int anim_speed;//the speed of the aniamtion
	int current_tile; //internal the current tileshowing
	int anim_count;//an animation counter...
	char sound[30];
}TILE_INFO;


typedef struct
{
	char name[15]; //the nmae of the tileset
	int num_of_tiles; //the number of tiles
	TILE_INFO tile[100]; //the above
}TILESET_INFO;



//the functions
//int load_tiles(void);
int tile_is_solid(int x, int y);

void update_tiles(void);

#define MAX_TILES 100

#endif