////////////////////////////////////////////////////
// This file contains cropping for pics
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <stdio.h>

#include "grafik4.h"


#include "fiend.h"
#include "picdata.h"


void crop_picdata(BITMAP *bmp,PIC_DATA *pic)
{
	int i,j;
	int mask_color = makecol(255,0,255);

	int c_x = bmp->w/2;
	int c_y = bmp->h/2;
	int w = bmp->w;
	int h = bmp->h;
	int tx=0;
	int ty=0;


	//searh the top row
	for(j=ty;j<h;j++)
	{
		for(i=tx;i<w;i++)
			if(getpixel(bmp,i,j)!=mask_color)
				goto finnished_top;

		h--;
		ty++;
	}

	finnished_top:

	//search the bottom row
	for(j=ty+h-1;j>ty;j--)
	{
		for(i=tx;i<w;i++)
			if(getpixel(bmp,i,j)!=mask_color)
				goto finnished_bottom;

		h--;
	}

	finnished_bottom:

	
	//search the left row
	for(i=tx;i<tx+w;i++)
	{
		for(j=ty;j<h+ty;j++)
			if(getpixel(bmp,i,j)!=mask_color)
				goto finnished_left;

		w--;
		tx++;
	}
	
	finnished_left:


	//search the right row
	for(i=w+tx-1;i>tx;i--)
	{
		for(j=ty;j<h+ty;j++)
			if(getpixel(bmp,i,j)!=mask_color)
				goto finnished_right;

		w--;
	}

	finnished_right:

	if(w % 2 !=0)w++;
	if(h % 2 !=0)h++;


	pic->data = create_bitmap(w,h);

	blit(bmp,pic->data,tx,ty,0,0,w,h);

	pic->center_x = c_x-tx;
	pic->center_y = c_y-ty;
	
}

