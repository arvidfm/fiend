//***************************************************************************
//**
//** Alias25 (C) 2000
//**
//** File: DRAW_POLYGON.C
//**
//** Date: 11/12/00
//**
//** Author: Pierre Renaux
//**
//** Desc:  Polygon drawing functions.
//**        Written for speed not for a code style contest ;)
//**
//**        The rendering functions Draw from the 1st to the last line
//**        include.
//**        You MUST pass INTERGER AND NOT FIXED !!!
//**
//***************************************************************************
//===========================================================================
//    IMPLEMENTATION HEADERS
//===========================================================================
#include "allegro.h"
#include "fiend.h"
#include "draw_polygon.h"

//===========================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//===========================================================================

#define RED_MASK16 63488
#define GREEN_MASK16 2016
#define BLUE_MASK16 31

#define RED_MASK15 31744
#define GREEN_MASK15 992
#define BLUE_MASK15 31


typedef struct HLINE_FILL_INFO
{
    long xstart, xend;
    long ustart, uend;
    long vstart, vend;
} HLINE_FILL_INFO;

static HLINE_FILL_INFO infos[480];

//===========================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES
//===========================================================================
//===========================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//===========================================================================

//===========================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//===========================================================================
//===========================================================================
//    IMPLEMENTATION PRIVATE DATA
//===========================================================================
static unsigned long tex_umask;
static unsigned long tex_vmask;
static unsigned long tex_vshift;
static void *tex_data;
static unsigned int color;
	

//===========================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//===========================================================================
#define DRAW_HLINE_LIST(type,name) \
static __inline void draw_hline_list_##name(BITMAP *bmp, HLINE_FILL_INFO *infos, int ystart, int yend) { \
    type *dest;         \
    long xstart, xend;  \
    long gap;           \
    int i = 0;          \
    DECLARE_VARS;       \
    \
    while(ystart <= yend)    {              \
        xstart = infos[i].xstart>>16;       \
        xend = infos[i].xend>>16;           \
        GET_HLINE_VARS;                     \
        \
        if(xstart < bmp->cl)        {       \
            gap = bmp->cl - xstart;         \
            xstart = bmp->cl;               \
            CLIPX;                          \
        }                                   \
        \
        if(xend >= bmp->cr)        {        \
            xend = bmp->cr - 1;             \
        }                                   \
        \
        dest = ((unsigned short*)bmp->line[ystart])+xstart; \
        \
        while(xstart <= xend)        {      \
            WRITE_PIXEL;                    \
            INCX;                           \
            xstart++;                       \
            INCDEST;                        \
        }                                   \
                                            \
        ystart++;                           \
        i++;                                \
    }                                       \
}

// ATEX 16 bit color
#define DECLARE_VARS        \
    long ustart, uend;      \
    long udelta;            \
    long vstart, vend;      \
    long vdelta;            \
    long puv;				\
	unsigned int d_pix;		\
	unsigned int temp_pix;	\
	unsigned short col;

#define GET_HLINE_VARS                  \
    ustart = infos[i].ustart;           \
    uend = infos[i].uend;               \
    vstart = infos[i].vstart;           \
    vend = infos[i].vend;               \
    puv = xend - xstart;                \
    if(puv) {                           \
        udelta = (uend-ustart)/puv;     \
        vdelta = (vend-vstart)/puv;     \
    }                                   \
    else    {                           \
        udelta = (uend-ustart);         \
        vdelta = (vend-vstart);         \
    }

#define CLIPX               \
    ustart += udelta*gap;   \
    vstart += vdelta*gap;

#define WRITE_PIXEL \
	col = ((unsigned short*)tex_data)[(((vstart>>16)&tex_vmask)<<tex_vshift)+((ustart>>16)&tex_umask)]; \
	d_pix=0;													\
	temp_pix = (col & RED_MASK16) + (*dest & RED_MASK16);		\
	if(temp_pix > RED_MASK16) temp_pix = RED_MASK16;			\
	d_pix |= temp_pix;											\
	temp_pix = (col & GREEN_MASK16) + (*dest & GREEN_MASK16);	\
	if(temp_pix > GREEN_MASK16)temp_pix = GREEN_MASK16;			\
	d_pix |= temp_pix;											\
	temp_pix = (col & BLUE_MASK16) + (*dest & BLUE_MASK16);		\
	if(temp_pix > BLUE_MASK16) temp_pix = BLUE_MASK16;			\
	d_pix |= temp_pix;											\
	*dest = (unsigned short)d_pix;								
	
	

#define INCX            \
    ustart += udelta;   \
    vstart += vdelta;

#define INCDEST dest++

DRAW_HLINE_LIST(unsigned short,atex_16)

#undef DECLARE_VARS
#undef GET_HLINE_VARS
#undef CLIPX
#undef WRITE_PIXEL
#undef INCX
#undef INCDEST

// ATEX 15 bit color
#define DECLARE_VARS        \
    long ustart, uend;      \
    long udelta;            \
    long vstart, vend;      \
    long vdelta;            \
    long puv;				\
	unsigned int d_pix;		\
	unsigned int temp_pix;	\
	unsigned short col;

#define GET_HLINE_VARS                  \
    ustart = infos[i].ustart;           \
    uend = infos[i].uend;               \
    vstart = infos[i].vstart;           \
    vend = infos[i].vend;               \
    puv = xend - xstart;                \
    if(puv) {                           \
        udelta = (uend-ustart)/puv;     \
        vdelta = (vend-vstart)/puv;     \
    }                                   \
    else    {                           \
        udelta = (uend-ustart);         \
        vdelta = (vend-vstart);         \
    }

#define CLIPX               \
    ustart += udelta*gap;   \
    vstart += vdelta*gap;

#define WRITE_PIXEL \
 col = ((unsigned short*)tex_data)[(((vstart>>16)&tex_vmask)<<tex_vshift)+((ustart>>16)&tex_umask)]; \
	d_pix=0;													\
	temp_pix = (col & RED_MASK15) + (*dest & RED_MASK15);		\
	if(temp_pix > RED_MASK15) temp_pix = RED_MASK15;			\
	d_pix |= temp_pix;											\
	temp_pix = (col & GREEN_MASK15) + (*dest & GREEN_MASK15);	\
	if(temp_pix > GREEN_MASK15)temp_pix = GREEN_MASK15;			\
	d_pix |= temp_pix;											\
	temp_pix = (col & BLUE_MASK15) + (*dest & BLUE_MASK15);		\
	if(temp_pix > BLUE_MASK15) temp_pix = BLUE_MASK15;			\
	d_pix |= temp_pix;											\
	*dest = (unsigned short)d_pix;								

#define INCX            \
    ustart += udelta;   \
    vstart += vdelta;

#define INCDEST dest++

DRAW_HLINE_LIST(unsigned short,atex_15)

#undef DECLARE_VARS
#undef GET_HLINE_VARS
#undef CLIPX
#undef WRITE_PIXEL
#undef INCX
#undef INCDEST

static __inline void poly3_atex(BITMAP *bmp, BITMAP *tex, VERTEX *v1, VERTEX *v2, VERTEX *v3)
{
    VERTEX *temp;

	long delta1, delta2;
    long xstart, xend;
    
    long ustart, uend;
    long udelta1, udelta2;
    
    long vstart, vend;
    long vdelta1, vdelta2;
    
    short index;
    short info_index = 0;
    
    long pac, pab, pbc;
    
    long gap = 0;
    
    int y1 = v1->y;
    int y2 = v2->y, oy2;
    int y3 = v3->y;
    int starty;
    int endy;

    if((y1 < bmp->ct) && (y2 < bmp->ct) && (y3 < bmp->ct))
        return;
    
    if((y1 >= bmp->cb) && (y2 >= bmp->cb) && (y3 >= bmp->cb))
        return;
    
    if((v1->x < bmp->cl) && (v2->x < bmp->cl) && (v3->x < bmp->cl))
        return;
    
    if((v1->x >= bmp->cr) && (v2->x >= bmp->cr) && (v3->x >= bmp->cr))
        return;
    
    if(v1->y > v2->y)
    {
        temp = v1;
        v1 = v2;
        v2 = temp;
    }
    
    if(v1->y > v3->y)
    {
        temp = v1;
        v1 = v3;
        v3 = temp;
    }
    
    if(v2->y > v3->y)
    {
        temp = v2;
        v2 = v3;
        v3 = temp;
    }

    y1 = v1->y;
    oy2 = y2 = v2->y;
    y3 = v3->y;

        
    if(y1 != y3)
    {
        pac = y1 - y3;
        delta1 = ((v1->x-v3->x)<<16)/pac;
        udelta1 = ((v1->u-v3->u)<<16)/pac;
        vdelta1 = ((v1->v-v3->v)<<16)/pac;
        ustart = v1->u<<16;
        uend = ustart;
        vstart = v1->v<<16;
        vend = vstart;
    }
    else
    {
        delta1 = 0;
        udelta1 = (v1->u-v3->u)<<16;
        vdelta1 = (v1->v-v3->v)<<16;
        ustart = v1->u<<16;
        uend = v3->u<<16;
        vstart = v1->v<<16;
        vend = v3->v<<16;
    }
    
    if(y1 != y2)
    {
        pab = y2 - y1;
        delta2 = ((v2->x-v1->x)<<16)/pab;
        udelta2 = ((v2->u-v1->u)<<16)/pab;
        vdelta2 = ((v2->v-v1->v)<<16)/pab;
        ustart = v1->u<<16;
        uend = ustart;
        vstart = v1->v<<16;
        vend = vstart;
    }
    else
    {
        delta2 = 0;
        udelta2 = (v1->u-v3->u)<<16;
        vdelta2 = (v1->v-v3->v)<<16;
        ustart = v1->u<<16;
        uend = v2->u<<16;
        vstart = v1->v<<16;
        vend = v2->v<<16;
    }
    
    // Fill global variables states
    tex_data = (unsigned short*)tex->line[0];
    tex_umask = tex->w - 1;
    tex_vmask = tex->h - 1;
    
    tex_vshift = 0;
    while((1 << tex_vshift) < tex->w)
        tex_vshift++;
    
    // Fill infos structure
    xstart = v1->x<<16;
    xend = xstart;
    
    if(y1 < bmp->ct)
    {
        gap = bmp->ct - y1;
        y1 = bmp->ct;
        xstart += delta1 * gap;
        xend += delta2 * gap;
        ustart += udelta1 * gap;
        uend += udelta2 * gap;
        vstart += vdelta1 * gap;
        vend += vdelta2 * gap;
    }    
    
    if(y2 >= bmp->cb)
    {
        y2 = bmp->cb-1;
    }
    
    starty = y1;
    endy = y2;
    
    if(y1 < y2 && y2 >= bmp->ct)
    {
        
        if(delta1 < delta2)
        {
            for(index = y1; index <= y2; index++)
            {
                infos[info_index].xstart = xstart;
                infos[info_index].xend = xend;
                infos[info_index].ustart = ustart;
                infos[info_index].uend = uend;
                infos[info_index].vstart = vstart;
                infos[info_index].vend = vend;
                xstart += delta1;
                xend += delta2;
                ustart += udelta1;
                uend += udelta2;
                vstart += vdelta1;
                vend += vdelta2;
                info_index++;
            }
        }
        else
        {
            for(index = y1; index <= y2; index++)
            {
                infos[info_index].xstart = xend;
                infos[info_index].xend = xstart;
                infos[info_index].ustart = uend;
                infos[info_index].uend = ustart;
                infos[info_index].vstart = vend;
                infos[info_index].vend = vstart;
                xstart += delta1;
                xend += delta2;
                ustart += udelta1;
                uend += udelta2;
                vstart += vdelta1;
                vend += vdelta2;
                info_index++;
            }
        }
        
        y2++;
    }
    
    if(y2 != y3)
    {
        if(oy2 != y3)
        {
            pbc = oy2 - y3;
            delta2  = ((v2->x - v3->x)<<16)/pbc;
            udelta2 = ((v2->u - v3->u)<<16)/pbc;
            vdelta2 = ((v2->v - v3->v)<<16)/pbc;
        }
        else
        {
            delta2  = ((v2->x - v3->x)<<16);
            udelta2 = ((v2->u - v3->u)<<16);
            vdelta2 = ((v2->v - v3->v)<<16);
        }
        
        xend = (v2->x<<16);
        uend = (v2->u<<16);
        vend = (v2->v<<16);

        if(y2 < bmp->ct)
        {
            gap = bmp->ct - oy2;
            if(y1 > y2)
                gap--;
            y2 = bmp->ct;
            xend += delta2 * gap;
            uend += udelta2 * gap;
            vend += vdelta2 * gap;
            starty = y2;
        }
        
        if(y3 >= bmp->cb)
        {
            y3 = bmp->cb - 1;
        }
        
        endy = y3;        
        
        if(delta2 < delta1)
        {
            for(index = y2; index <= y3; index++)
            {
                infos[info_index].xstart = xstart;
                infos[info_index].xend = xend;
                infos[info_index].ustart = ustart;
                infos[info_index].uend = uend;
                infos[info_index].vstart = vstart;
                infos[info_index].vend = vend;
                xstart += delta1;
                xend += delta2;
                ustart += udelta1;
                uend += udelta2;
                vstart += vdelta1;
                vend += vdelta2;
                info_index++;
            }
        }
        else
        {
            for(index = y2; index <= y3; index++)
            {
                infos[info_index].xstart = xend;
                infos[info_index].xend = xstart;
                infos[info_index].ustart = uend;
                infos[info_index].uend = ustart;
                infos[info_index].vstart = vend;
                infos[info_index].vend = vstart;
                xstart += delta1;
                xend += delta2;
                ustart += udelta1;
                uend += udelta2;
                vstart += vdelta1;
                vend += vdelta2;
                info_index++;
            }
        }
    }
    if(bitmap_color_depth(bmp)==16)
		draw_hline_list_atex_16(bmp, infos, starty, endy);
	else
		draw_hline_list_atex_15(bmp, infos, starty, endy);
}




void fiend_quad3d(BITMAP *dest,BITMAP *src,VERTEX *v0,VERTEX *v1,VERTEX *v2,VERTEX *v3)
{
	poly3_atex(dest, src, v0,v1,v2);
	poly3_atex(dest, src, v2,v3,v0);
}


/*static __inline void poly_atex(AGL_BITMAP *bmp, AGL_BITMAP *tex, VERTEX *v, int num)
{
    int i;
    VERTEX *v0;
    
    if(num < 3)
        return;

    v0 = &v[0];
    for(i = 2; i < num; i++)
        poly3_atex(bmp, tex, v0, &v[i-1], &v[i]);
}

static __inline void poly_atex_mask(AGL_BITMAP *bmp, AGL_BITMAP *tex, VERTEX *v, int num)
{
    int i;
    VERTEX *v0;
    
    if(num < 3)
        return;

    v0 = &v[0];
    for(i = 2; i < num; i++)
        poly3_atex_mask(bmp, tex, v0, &v[i-1], &v[i]);
}


//===========================================================================
//    INTERFACE DATA
//===========================================================================
//===========================================================================
//    INTERFACE FUNCTIONS
//===========================================================================
void rotate_quad(AGL_BITMAP *bmp, AGL_BITMAP *sprite, int x, int y, fixed angle)
{
 VERTEX v0, v1, v2, v3;
 fixed s = fsin(angle);
 fixed c = fcos(angle);
 int wgapi = sprite->w;
 int hgapi = sprite->h;
 int wgap2i = wgapi>>1;
 int hgap2i = hgapi>>1;

#define X(x,y) fixtoi(((x)*(c))-((y)*(s)))
#define Y(x,y) fixtoi(((y)*(c))+((x)*(s)))

 v0.x = X(wgap2i,hgap2i)+x;
 v0.y = Y(wgap2i,hgap2i)+y;
 v0.u = wgapi;
 v0.v = hgapi;

 v1.x = X(-wgap2i,hgap2i)+x;
 v1.y = Y(-wgap2i,hgap2i)+y;
 v1.u = 0;
 v1.v = hgapi;

 v2.x = X(-wgap2i,-hgap2i)+x;
 v2.y = Y(-wgap2i,-hgap2i)+y;
 v2.u = 0;
 v2.v = 0;

 v3.x = X(wgap2i,-hgap2i)+x;
 v3.y = Y(wgap2i,-hgap2i)+y;
 v3.u = wgapi;
 v3.v = 0;

#undef X
#undef Y

 poly3_atex_mask(bmp, sprite, &v0, &v1, &v2);
 poly3_atex_mask(bmp, sprite, &v2, &v3, &v0);
}

void rotate_scale_quad(AGL_BITMAP *bmp, AGL_BITMAP *sprite, int x, int y, fixed angle, fixed scale)
{
 VERTEX v0, v1, v2, v3;
 fixed s = fsin(angle);
 fixed c = fcos(angle);
 int wgapi = sprite->w;
 int hgapi = sprite->h;
 int wgap2i = ((wgapi>>1)*scale)>>16;
 int hgap2i = ((hgapi>>1)*scale)>>16;

#define X(x,y) ((((x)*(c))-((y)*(s)))>>16)
#define Y(x,y) ((((y)*(c))+((x)*(s)))>>16)

 v0.x = X(wgap2i,hgap2i)+x;
 v0.y = Y(wgap2i,hgap2i)+y;
 v0.u = 0;
 v0.v = hgapi;

 v1.x = X(-wgap2i,hgap2i)+x;
 v1.y = Y(-wgap2i,hgap2i)+y;
 v1.u = wgapi;
 v1.v = hgapi;

 v2.x = X(-wgap2i,-hgap2i)+x;
 v2.y = Y(-wgap2i,-hgap2i)+y;
 v2.u = wgapi;
 v2.v = 0;

 v3.x = X(wgap2i,-hgap2i)+x;
 v3.y = Y(wgap2i,-hgap2i)+y;
 v3.u = 0;
 v3.v = 0;

#undef X
#undef Y

 poly3_atex_mask(bmp, sprite, &v0, &v1, &v2);
 poly3_atex_mask(bmp, sprite, &v2, &v3, &v0);
}*/

//***************************************************************************
//**
//**    END MODULE DRAW_POLYGON.C
//**
//***************************************************************************
