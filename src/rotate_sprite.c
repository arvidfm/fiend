////////////////////////////////////////////////////
// This file conatins gfx functions drawing rotated sprites
//
// code by Thomas Grip 
///////////////////////////////////////////////////


#include <math.h>
#include <allegro.h>

#include "grafik4.h"
#include "rotate_sprite.h"

#define RED_MASK16 63488
#define GREEN_MASK16 2016
#define BLUE_MASK16 31

#define RED_MASK15 31744
#define GREEN_MASK15 992
#define BLUE_MASK15 31


#define MAX_HYPOT_LENGTH 1000

static fixed max_x_buffer[480];
static fixed min_x_buffer[480];

static fixed max_u_buffer[480];
static fixed min_u_buffer[480];

static fixed max_v_buffer[480];
static fixed min_v_buffer[480];

static int src_add[MAX_HYPOT_LENGTH];

void pivot_fiend_sprite(BITMAP *bmp, BITMAP *sprite, int x, int y, int cx, int cy, float angle, int draw_mode)
{
   x -= fixtoi(sprite->w/2);
   y -= fixtoi(sprite->h/2);

   cx -= sprite->w/2;
   cy -= sprite->h/2;

   x -= cx*COS(angle) - cy*SIN(angle);
   y -= cx*SIN(angle) + cy*COS(angle);

   rotate_fiend_sprite(bmp, sprite, x, y, angle, draw_mode);
}



void rotate_fiend_sprite(BITMAP *dest, BITMAP * src, int dest_x, int dest_y, float angle, int draw_mode)
{
	int x[4],y[4];
	float temp_x, temp_y;
	int max_y,min_y;
	int max_x,min_x;
	int i,k;
	int y1,y2;
	float dx,dy,du,dv;
	fixed m,draw_x;
	fixed mu,mv, u,v;
	int * src_add_buffer;
	int dist;


	register unsigned int d_pix;
	register unsigned int temp_pix;

	register unsigned short *dest_buffer;
	register unsigned short *src_buffer;
	register int j;
	
	// test for errors //
	if(angle>360)angle=angle - 360;
	if(angle<0)angle=360 - angle;


	/////// Rotate the Points ////////////////

	temp_x = src->w/2;
	temp_y = src->h/2;
	x[0] = (-temp_x*COS(angle)+temp_y*SIN(angle))+dest_x;
	y[0] = (-temp_y*COS(angle)-temp_x*SIN(angle))+dest_y;

	temp_x = src->w/2;
	temp_y = src->h/2;
	x[1] = (temp_x*COS(angle)+temp_y*SIN(angle))+dest_x;
	y[1] = (-temp_y*COS(angle)+temp_x*SIN(angle))+dest_y;

	
	temp_x = src->w/2;
	temp_y = src->h/2;
	x[2] = (temp_x*COS(angle)-temp_y*SIN(angle))+dest_x;
	y[2] = (temp_y*COS(angle)+temp_x*SIN(angle))+dest_y;
	
	temp_x = src->w/2;
	temp_y = src->h/2;
	x[3] = (-temp_x*COS(angle)-temp_y*SIN(angle))+dest_x;
	y[3] = (temp_y*COS(angle)-temp_x*SIN(angle))+dest_y;


	/////// Get the min and max y /////////////

	max_y = y[0];
    min_y = y[0];
	
	if(y[1]<min_y) min_y = y[1];
	else if(y[1]>max_y) max_y = y[1];
	
	if(y[2] < min_y) min_y = y[2];
	else if(y[2]>max_y) max_y = y[2];
	
	if(y[3]<min_y) min_y = y[3];
	else if(y[3]>max_y) max_y = y[3];

	if(min_y<0)min_y=0;
	if(max_y>=dest->h)max_y=dest->h-1;

	if(min_y>=dest->h)return;
	if(max_y<0)return;
	
	

	/////// Make the x min and max buffers //////
	
	//clear the buffers
	for(i=min_y;i<max_y;i++)min_x_buffer[i] = itofix(3000);
	for(i=min_y;i<max_y;i++)max_x_buffer[i] = itofix(0);

	
	/////////////////////// the cords 0 - 1 //////////////////////
	
	dx = x[0]-x[1];
	dy = y[0]-y[1];
	du = src ->w-1;
	du = -du;
	dv = 0;
	if(dy!=0)
	{
		m = ftofix(dx/dy);
		mu = ftofix(du/dy);
		mv = ftofix(dv/dy);
		
		if(y[0] < y[1])
		{
			y1=y[0]; 
			y2=y[1];
			draw_x=itofix(x[0]);
			u = itofix(0);
			v = itofix(0);
		}
		else 
		{
			y1=y[1];
			y2=y[0];
			draw_x=itofix(x[1]);
			u = itofix(src->w-1);
			v = itofix(0);
		}
		
		if(y2>max_y)y2 = max_y;
	
		for(i=y1;i<y2;i++)
		{
			if(i>=0 && draw_x < min_x_buffer[i])
			{
				min_x_buffer[i]=draw_x;
				min_u_buffer[i]=u;
				min_v_buffer[i]=v;
				
			}
			if(draw_x > max_x_buffer[i])
			{
				max_x_buffer[i]=draw_x;
				max_u_buffer[i]=u;
				max_v_buffer[i]=v;
		
			}

			draw_x+=m;
			u+=mu;
			v+=mv;

		}
	}

	/////////////// the cords 1 - 2 //////////////////// 
	dx = x[1]-x[2];
	dy = y[1]-y[2];
	du = 0;
	dv = src->h-1;
	dv = -dv;
	if(dy!=0)
	{
		m = ftofix(dx/dy);
		mu = ftofix(du/dy);
		mv = ftofix(dv/dy);
	
		if(y[1] < y[2])
		{
			y1=y[1];
			y2=y[2];
			draw_x=itofix(x[1]);
			u = itofix(src->w-1);
			v = itofix(0);
		}
		else 
		{
			y1=y[2];
			y2=y[1];
			draw_x=itofix(x[2]);
			u = itofix(src->w-1);
			v = itofix(src->h-1);
		}
		
		//if(y1<min_y)y1 = min_y;
		if(y2>max_y)y2 = max_y;
	
		for(i=y1;i<y2;i++)
		{
			if(i>=0 && draw_x < min_x_buffer[i])
			{
				min_x_buffer[i]=draw_x;
				min_u_buffer[i]=u;
				min_v_buffer[i]=v;
				
	
			}
			if(draw_x > max_x_buffer[i])
			{
				max_x_buffer[i]=draw_x;
				max_u_buffer[i]=u;
				max_v_buffer[i]=v;
		
			}
			draw_x+=m;
			u+=mu;
			v+=mv;
		}
	}

	////////// the cords 2 - 3 //////////////////////
	dx = x[2]-x[3];
	dy = y[2]-y[3];
	du = src ->w-1;
	//du = -du;
	dv = 0;
	if(dy!=0)
	{
		m = ftofix(dx/dy);
		mu = ftofix(du/dy);
		mv = ftofix(dv/dy);
	
		if(y[2] < y[3])
		{
			y1=y[2];
			y2=y[3];
			draw_x=itofix(x[2]);
			u = itofix(src->w-1);
			v = itofix(src->h-1);
		}
		else 
		{
			y1=y[3];
			y2=y[2];
			draw_x=itofix(x[3]);
			u = itofix(0);
			v = itofix(src->h-1);
		}
		//if(y1<min_y)y1 = min_y;
		if(y2>max_y)y2 = max_y;
			
		for(i=y1;i<y2;i++)
		{
			if(i>=0 && draw_x < min_x_buffer[i])
			{
				min_x_buffer[i]=draw_x;
				min_u_buffer[i]=u;
				min_v_buffer[i]=v;
				
			}
			if(draw_x > max_x_buffer[i])
			{
				max_x_buffer[i]=draw_x;
				max_u_buffer[i]=u;
				max_v_buffer[i]=v;
		
			}
			draw_x+=m;
			u+=mu;
			v+=mv;
		}
	}

	
	///////// the cords 3 - 0 ///////////////
	dx = x[3]-x[0];
	dy = y[3]-y[0];
	du = 0;
	dv = src ->h-1;
	if(dy!=0)
	{
		m = ftofix(dx/dy);
		mu = ftofix(du/dy);
		mv = ftofix(dv/dy);
		
		if(y[3] < y[0])
		{
			y1=y[3];
			y2=y[0];
			draw_x=itofix(x[3]);
			u = itofix(0);
			v = itofix(src->h-1);
		}
		else 
		{
			y1=y[0];
			y2=y[3];
			draw_x=itofix(x[0]);
			u = itofix(0);
			v = itofix(0);
		}

		//if(y1<min_y)y1 = min_y;
		if(y2>max_y)y2 = max_y;
	
		for(i=y1;i<y2;i++)
		{
			if(i>=0 && draw_x < min_x_buffer[i])
			{
				min_x_buffer[i]=draw_x;
				min_u_buffer[i]=u;
				min_v_buffer[i]=v;
			}
			if(draw_x > max_x_buffer[i])
			{
				max_x_buffer[i]=draw_x;
				max_u_buffer[i]=u;
				max_v_buffer[i]=v;
		
			}
			draw_x+=m;
			u+=mu;
			v+=mv;
		}
	}


	
	//////////////// Calculate the m:s....////////////

	i = (min_y+max_y)/2;//get the middle of the rows....
	mv = fdiv(min_v_buffer[i]-max_v_buffer[i], min_x_buffer[i]-max_x_buffer[i]); 
	mu = fdiv(min_u_buffer[i]-max_u_buffer[i], min_x_buffer[i]-max_x_buffer[i]); 

	
	///////////////// Some final Clipping ////////////
	
	for(i=min_y;i<max_y;i++)
	{
		if(min_x_buffer[i]<0)
		{
			dist = fixtoi(min_x_buffer[i]);
			dist = 0 - dist;
			
			min_u_buffer[i] += mu * dist;
			min_v_buffer[i] += mv * dist;

			min_x_buffer[i]=0;
		}
		else if(min_x_buffer[i]>itofix(dest->w-1))
		{
			min_x_buffer[i]=itofix(dest->w-1);
		}

		if(max_x_buffer[i]>itofix(dest->w-1))
		{
			max_x_buffer[i]=itofix(dest->w-1);
		}
		else if(max_x_buffer[i]<0)
		{
			max_x_buffer[i]=0;
		}
	}


	/////////////// Draw the line!!///////////////////
	
	u = 0;
	v = 0;
	j = 0;
	k = sqrt(src->w*src->w + src->h*src->h)+1;
	for(i=0;i<k;i++)
	{
		u+=mu;
		v+=mv;
		src_add[i]= (fixtoi(v)*src->w+fixtoi(u))-j;
		j+=src_add[i];
	}
	
	dest_buffer = (short*)dest->line[0];
	dest_buffer += (min_y*dest->w)+fixtoi(min_x_buffer[min_y]);
		
	if(bitmap_color_depth(dest) == 16)
	{
		if(draw_mode == FIEND_DRAW_MODE_TRANS)
		{
			ROTATE_DRAW_LOOP_TRANS16
		}
		else if(draw_mode == FIEND_DRAW_MODE_FLAT)
		{
			ROTATE_DRAW_LOOP_FLAT16
		}
		else if(draw_mode == FIEND_DRAW_MODE_ADDITIVE)
		{
			ROTATE_DRAW_LOOP_ADDITIVE16
		}
	}
	else if(bitmap_color_depth(dest) == 15)
	{
		if(draw_mode == FIEND_DRAW_MODE_TRANS)
		{
			ROTATE_DRAW_LOOP_TRANS15
		}
		else if(draw_mode == FIEND_DRAW_MODE_FLAT)
		{
			ROTATE_DRAW_LOOP_FLAT16
		}
		else if(draw_mode == FIEND_DRAW_MODE_ADDITIVE)
		{
			ROTATE_DRAW_LOOP_ADDITIVE15
		}
	}
}



