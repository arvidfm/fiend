#include <allegro.h>

#include <stdio.h>

#include "../fiend.h"
#include "mapedit.h"

static char temp_string[180];


BRUSH_DATA brush[MAX_BRUSH_NUM];

int brush_chosen=0;
int current_brush=0;

static int brush_x;
static int brush_y;

static char brush_load_path[80]="";


static BITMAP *bt_buffer; 


int save_brush(void)
{
	FILE *f;

	f = fopen(brush_load_path,"wb");
	if(f==NULL)return 0;

	fwrite(brush,sizeof(BRUSH_DATA),MAX_BRUSH_NUM,f);

	fclose(f);
	
	return 1;
}

int load_brush(void)
{
	
	FILE *f;

	f = fopen(brush_load_path,"rb");
	if(f==NULL)return 0;
	
	fread(brush,sizeof(BRUSH_DATA),MAX_BRUSH_NUM,f);
	
	fclose(f);
	
	return 1;
}

int d_load_brush_proc(void)
{
	int ans;
	ans = alert("Sure you want a load brush?",NULL,NULL,"Yes","Cancel",'\r',0);// are you sure?
	if(ans==2)return D_O_K;

	ans = file_select("LOAD A BRUSH",brush_load_path,"brs");//select map
	if(ans!=0)
	{
		if(exists(brush_load_path)==0) //check if the file exists
		{
			alert("could not load",get_filename(brush_load_path)," ","OK",NULL,'\r',0); 
			return D_REDRAW;
		}
		
		load_brush();
		
		return D_REDRAW;
	}
	else
	{
		return D_REDRAW;
	}
}


int d_save_brush_proc(void)
{
	int ans;
	char old_save_path[80];
 
	strcpy(old_save_path, brush_load_path);

	ans = file_select("SAVE BRUSH",brush_load_path,"brs");

	if(ans!=0)
	{
		if(exists(brush_load_path) && strcmp(brush_load_path, old_save_path)!=0)
			if(alert(get_filename(brush_load_path),"Allready exists!","Overwrite?","OK","Cancel",0,0)==2)
				return D_REDRAW;
      
		if(!save_brush())
		{
			alert("could not save",get_filename(brush_load_path)," ","OK",NULL,'\r',0);
		}
		return D_REDRAW;
	}
	else
	{
		return D_REDRAW;
	}
}

//this button makes the test window get redrawn...
int d_save_brush_button_proc(int msg,DIALOG *d,int c)
{
	int ans;
	ans = d_button_proc(msg,d,c);
	
	if(ans==D_EXIT)
	{
		d_save_brush_proc();
		return D_REDRAW;
		
	}
	

	return D_O_K;
}
int d_load_brush_button_proc(int msg,DIALOG *d,int c)
{
	int ans;
	ans = d_button_proc(msg,d,c);
	
	if(ans==D_EXIT)
	{
		d_load_brush_proc();
		return D_REDRAW;
		
	}
	

	return D_O_K;
}


void edit_brush_tile(void)
{
	int ans;
	
	char temp_set[10];
	char temp_num[10];

	int set,num;


	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 300, 200,  200,  200,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     305, 375,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     365, 375,  53,   20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,         "Cancel"},
		
		{  d_text_proc, 315,   205,   50,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Set:"},
		{  d_edit_proc, 365,   205,   120,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  7,    0,    temp_set},
			
		{  d_text_proc, 315,   245,   50,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Num:"},
		{  d_edit_proc, 365,   245,   80,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  7,    0,    temp_num},
					
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_set,"%d", brush[brush_chosen].data[brush_x][brush_y].tile_set);
	sprintf(temp_num,"%d", brush[brush_chosen].data[brush_x][brush_y].tile_num);

	
	dialog_again:
	ans = popup_dialog(trigger_dialog,-1);


	if(ans==1)
	{
		set = atoi(temp_set);
		num = atoi(temp_num);

		if(set>num_of_tilesets-1 || set<0)
		{
			alert("Tile set not legal.",NULL,NULL,"OK",NULL,'\r',0);
			goto dialog_again;
		}
		if(num>tile_info[set].num_of_tiles || num<0)
		{
			alert("Tile num not legal.",NULL,NULL,"OK",NULL,'\r',0);
			goto dialog_again;
		}


		brush[brush_chosen].data[brush_x][brush_y].tile_set = set;
		brush[brush_chosen].data[brush_x][brush_y].tile_num = num;
	}	

}



//Shows all the tiles in the tileset and lets you select
int d_brush_tile_edit_proc(int msg, DIALOG *d, int c)
{
 int i,j;

 
 switch(msg)
 {
 case MSG_START:
	 bt_buffer = create_bitmap(d->w, d->h);
	 clear_to_color(bt_buffer, makecol(255,0,255));
	 break;

 case MSG_END:
	 destroy_bitmap(bt_buffer);
	 break;
 
 case MSG_DRAW:

	 clear_to_color(bt_buffer,makecol(255,255,255));
	 
	 
	 for(i=0;i<MAX_BRUSH_SIZE;i++)
		for(j=0;j<MAX_BRUSH_SIZE;j++)
		{
			if(brush[brush_chosen].data[i][j].tile_num>-1)
				draw_rle_sprite(bt_buffer, tile_data[brush[brush_chosen].data[i][j].tile_set][brush[brush_chosen].data[i][j].tile_num].dat,i*32,j*32);
			rect(bt_buffer,i*32,j*32,i*32+32,j*32+32,makecol(255,0,255));

		}	
	 rect(bt_buffer,0,0, d->w-1, d->h-1, makecol(0,0,0));//Draw a rect around all tiles

	 blit(bt_buffer, screen,0,0,d->x,d->y,d->w, d->h);
	 break;

 case MSG_CLICK:
	 
	 brush_x = (mouse_x-d->x)/32;
	 brush_y = (mouse_y-d->y)/32;

	 edit_brush_tile();

	 return	D_REDRAWME;
	 	 
	 
	 break;
	 



 }




 return D_O_K;
}




void edit_brush(void)
{
	int ans;

	char temp_name[20];
	char temp_w[7];
	char temp_h[7];


	DIALOG mission_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 250, 150,  340,  400,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		{  d_brush_tile_edit_proc, 255, 155,  320,  320,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     310, 515,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     380, 515,   53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },

		{  d_text_proc, 300,   482,   50,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "Name:"},
		{  d_edit_proc, 350,   482,   120,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  18,    0,    temp_name},
		
		{  d_text_proc, 300,   493,   50,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "w:"},
		{  d_edit_proc, 330,   493,   40,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  2,    0,    temp_w},
		
		{  d_text_proc, 370,   493,   50,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    "h:"},
		{  d_edit_proc, 400,   493,   120,   20  ,   FG_COLOR,  BG_COLOR,    0,    0,  2,    0,    temp_h},
		

		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

  	
	sprintf(temp_name,"%s",brush[brush_chosen].name);
	sprintf(temp_w,"%d",brush[brush_chosen].w);
	sprintf(temp_h,"%d",brush[brush_chosen].h);

	ans = popup_dialog(mission_dialog,-1);

  
	if(ans==2)
	{
		sprintf(brush[brush_chosen].name,"%s",temp_name);
		brush[brush_chosen].w = atoi(temp_w);
		brush[brush_chosen].h = atoi(temp_h);
	}

}



void init_brushes(void)
{
	int i,j,k;
	
	strcpy(brush_load_path,"");

	for(i=0;i<MAX_BRUSH_NUM;i++)
	{
		sprintf(brush[i].name,"%s%d","Brush",i);
		sprintf(brush[i].file,"");
		brush[i].w = MAX_BRUSH_SIZE;
		brush[i].h = MAX_BRUSH_SIZE;
		
		for(j=0;j<MAX_BRUSH_SIZE;j++)
			for(k=0;k<MAX_BRUSH_SIZE;k++)
			{
				brush[i].data[j][k].tile_num=0;
				brush[i].data[j][k].tile_set=0;
			}


	}
}
int d_reset_brush_button_proc(int msg,DIALOG *d,int c)
{
	int ans;
	ans = d_button_proc(msg,d,c);
	
	if(ans==D_EXIT)
	{
		if(alert("Sure you want to reset the brushes?",NULL,NULL,"Yes","No",0,0)==1)
			init_brushes();
		
		return D_REDRAW;
		
	}
	

	return D_O_K;
}



//The list-function for the NPCs
char *brush_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MAX_BRUSH_NUM;
  return NULL;
 }
 
 strcpy(temp_string, brush[index].name);
 
 return temp_string;
}




//this makes the list
int d_brush_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_text_list_proc(msg,d,c);
 
 brush_chosen=d->d1;

 if(ans==D_EXIT)
 {
	edit_brush();
	return D_REDRAW;
 }
 else
 {
	return ans;
 }
}

