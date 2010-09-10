#include <allegro.h>

#ifndef BMP_ARRAY_H
#define BMP_ARRAY_H

typedef struct
{
	BITMAP *dat;
	int num;
}BMP_ARRAY;

typedef struct
{
	RLE_SPRITE *dat;
	int num;
}RLE_ARRAY;

BMP_ARRAY* load_bmp_array(char *dir,int item_num);

void unload_bmp_array(BMP_ARRAY *temp);

RLE_ARRAY* load_rle_array(char *dir,int item_num);

void unload_rle_array(RLE_ARRAY *temp);


#endif