#include <allegro.h>

#ifndef PICDATA_H
#define PICDATA_H 


typedef struct
{
	int center_x;
	int center_y;
	BITMAP *data;
	RLE_SPRITE *rle_data;
}PIC_DATA;


#endif