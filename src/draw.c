////////////////////////////////////////////////////
// This file conatins gfx functions like draw_lightsprite
// and draw additive sprite
//
// code by Thomas Grip and Miro Karjalainen 
///////////////////////////////////////////////////



#include <allegro.h>

#include "fiend.h"
#include "console.h"

unsigned short light_table[32][65536];
COLOR_MAP greyscale_map;

#define RED_MASK16 63488
#define GREEN_MASK16 2016
#define BLUE_MASK16 31

#define RED_MASK15 31744
#define GREEN_MASK15 992
#define BLUE_MASK15 31


//Initialize draw_lightsprite
void init_draw(void)
{
	int c;
	int r,g,b;
	int i,j;
	fixed mul;
 
	if(color_depth==16)
	{
	 for(i=0;i<32;i++)
	 {
		mul = itofix(i)/31;
		for(j=0;j<65536;j++)
		{
			r = fixtoi(getr(j)*mul);
			g = fixtoi(getg(j)*mul);
			b = fixtoi(getb(j)*mul);
		
			light_table[i][j]=makecol(r,g,b);
		}
	}
}
	

  for(i=0;i<256;i++)
   for(j=0;j<256;j++)
   { 
   	c = j*8;
	c = c + i;
	if(c>255)c=255;
	c=c/8;
	if(i/8==1)c=j+1;
	if(i<8 && i>2)c=j+1;
	if(c>31)c=31;
	greyscale_map.data[i][j] = c;
   }

}




//draw a light sprite
void draw_lightsprite(BITMAP *dest, BITMAP *src,int x, int y)
{
 int i, j;
 short x_start=0;
 short x_length=src->w;
 short y_start=0;
 short y_length=src->h;
 register short *dest_buffer;
 register char *src_buffer;
 short dest_add=dest->w-x_length;
 short src_add= 0;



//Do some stuff so the we only draw the part of the bitmap that is visable

 src_buffer = (char*)src->line[0];

  if(x<dest->cl || y<dest->ct || x+src->w>dest->cr || y+src->h>dest->cb)
   {
    if(x<dest->cl)
     {
      x_length-=-(x-dest->cl);
      src_buffer+=-(x-dest->cl);
      x_start=-(x-dest->cl);
     }
    if(x>dest->cr-src->w)
     {
      x_length-= (x-(dest->cr-src->w));
     }

    if(y<dest->ct)
     {
      src_buffer+=src->w*(-(y-dest->ct));
      y_length=src->h+(y-dest->ct);
     }
    if(y>dest->cb-src->h)
     {
      y_length=src->h-(y-(dest->cb-src->h));
     }

    src_add = src->w-x_length;
    dest_add = dest->w-x_length;
   }

 if(x_length<=0 || y_length<=0) return;

 dest_buffer = (short*)dest->line[0];

 if(y>dest->ct)
  dest_buffer+= y*dest->w+x+x_start;
 else
  dest_buffer+= dest->ct*dest->w+x+x_start;

 //draw the sprite
 i = y_length;
 while(i)
 {
  j= x_length;
   while(j)
    {
     *dest_buffer = light_table[*src_buffer][*dest_buffer];
     dest_buffer++;
     src_buffer++;
     j--;
    }
  dest_buffer+=dest_add;
  src_buffer+=src_add;
  i--;

 }


}


//draw a light sprite
void draw_additive_sprite(BITMAP *dest, BITMAP *src,int x, int y)
{
	int i, j;
	short x_start=0;
	short x_length=src->w;
	short y_start=0;
	short y_length=src->h;
	register short *dest_buffer;
	register short *src_buffer;
	short dest_add=dest->w-x_length;
	short src_add= 0;
	register unsigned int d_pix;
	register unsigned int temp_pix;
	

	//Do some stuff so the we only draw the part of the bitmap that is visable

	src_buffer = (short*)src->line[0];

	if(x<dest->cl || y<dest->ct || x+src->w>dest->cr || y+src->h>dest->cb)
	{
		if(x<dest->cl)
		{
			x_length-=-(x-dest->cl);
			src_buffer+=-(x-dest->cl);
			x_start=-(x-dest->cl);
		}
		if(x>dest->cr-src->w)
		{
			x_length-= (x-(dest->cr-src->w));
		}

		if(y<dest->ct)
		{
			src_buffer+=src->w*(-(y-dest->ct));
			y_length=src->h+(y-dest->ct);
		}
		if(y>dest->cb-src->h)
		{
			y_length=src->h-(y-(dest->cb-src->h));
		}

		src_add = src->w-x_length;
		dest_add = dest->w-x_length;
	}

	if(x_length<=0 || y_length<=0) return;

	dest_buffer = (short*)dest->line[0];

	if(y>dest->ct)
		dest_buffer+= y*dest->w+x+x_start;
	else
		dest_buffer+= dest->ct*dest->w+x+x_start;

 
	//draw the sprite
	if(bitmap_color_depth(dest)==16)
	{
		i = y_length;
		while(i)
		{
			j= x_length;
			while(j)
			{
				d_pix=0;

				temp_pix = (*src_buffer & RED_MASK16) + (*dest_buffer & RED_MASK16);
				if(temp_pix > RED_MASK16) temp_pix = RED_MASK16;
				d_pix |= temp_pix;
			
				temp_pix = (*src_buffer & GREEN_MASK16) + (*dest_buffer & GREEN_MASK16);
				if(temp_pix > GREEN_MASK16)temp_pix = GREEN_MASK16;
				d_pix |= temp_pix;
			
				temp_pix = (*src_buffer & BLUE_MASK16) + (*dest_buffer & BLUE_MASK16);
				if(temp_pix > BLUE_MASK16) temp_pix = BLUE_MASK16;
				d_pix |= temp_pix;
			
				*dest_buffer = (unsigned short)d_pix;
	

				dest_buffer++;
				src_buffer++;
				j--;
			}
		
			dest_buffer+=dest_add;
			src_buffer+=src_add;
			i--;
		}
	}
	else if(bitmap_color_depth(dest)==15)
	{
		i = y_length;
		while(i)
		{
			j= x_length;
			while(j)
			{
				if(*src_buffer!=0)
				{
					d_pix=0;

					temp_pix = (*src_buffer & RED_MASK15) + (*dest_buffer & RED_MASK15);
					if(temp_pix > RED_MASK15) temp_pix = RED_MASK15;
					d_pix |= temp_pix;
			
					temp_pix = (*src_buffer & GREEN_MASK15) + (*dest_buffer & GREEN_MASK15);
					if(temp_pix > GREEN_MASK15)temp_pix = GREEN_MASK15;
					d_pix |= temp_pix;
			
					temp_pix = (*src_buffer & BLUE_MASK15) + (*dest_buffer & BLUE_MASK15);
					if(temp_pix > BLUE_MASK15) temp_pix = BLUE_MASK15;
					d_pix |= temp_pix;
			
					*dest_buffer = (unsigned short)d_pix;

				}

				dest_buffer++;
				src_buffer++;
				j--;
			}
		
			dest_buffer+=dest_add;
			src_buffer+=src_add;
			i--;
		}

	}


}


//draw a light map for the editor
void draw_lightmap(BITMAP *dest, BITMAP *src,int x, int y)
{
	color_map = &greyscale_map;
				
	draw_trans_sprite(dest,src,x, y); 


}

//this is for_the game....
void draw_lightmap2(BITMAP *dest, BITMAP *src,int x, int y)
{
 int x_start=0;
 int x_length=src->w;
 int y_start=0;
 int y_length=src->h;
 register char *dest_buffer;
 register char *src_buffer;
 int dest_add=dest->w-x_length;
 int src_add= 0;
 

//Do some stuff so the we only draw the part of the bitmap that is visable

 src_buffer = (char*)src->line[0];

  if(x<dest->cl || y<dest->ct || x+src->w>dest->cr || y+src->h>dest->cb)
   {
    if(x<dest->cl)
     {
      x_length-=-(x-dest->cl);
      src_buffer+=-(x-dest->cl);
      x_start=-(x-dest->cl);
     }
    if(x>dest->cr-src->w)
     {
      x_length-= (x-(dest->cr-src->w));
     }

    if(y<dest->ct)
     {
      src_buffer+=src->w*(-(y-dest->ct));
      y_length=src->h+(y-dest->ct);
     }
    if(y>dest->cb-src->h)
     {
      y_length=src->h-(y-(dest->cb-src->h));
     }

    src_add = src->w-x_length;
    dest_add = dest->w-x_length;
   }

 if(x_length<=0 || y_length<=0) return;

 dest_buffer = (char*)dest->line[0];

 if(y>dest->ct)
  dest_buffer+= y*dest->w+x+x_start;
 else
  dest_buffer+= dest->ct*dest->w+x+x_start;

/* i = y_length;
 while(i)
 {
  j= x_length;
   while(j)
    {
//     if(*src_buffer!=31)
  	 *dest_buffer = (*dest_buffer+*src_buffer);
     if(*dest_buffer > 31)
      *dest_buffer = 31;
     dest_buffer++;
     src_buffer++;
     j--;
    }
  dest_buffer+=dest_add;
  src_buffer+=src_add;
  i--;

 }*/
 
 //draw the sprite
__asm
{ 
mov esi, src_buffer
mov edi, dest_buffer

mov edx, y_length
zoop: 

mov ecx, x_length 
zool: 
lodsb
add al, [edi]

cmp al, 32 
jb nook
mov al, 31
nook: 
mov [edi], al

inc edi 
dec ecx 

jnz zool
 
add esi, src_add
add edi, dest_add

dec edx 
jnz zoop 
}


}



