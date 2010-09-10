#include <allegro.h>

#include <stdio.h>

#include "../fiend.h"
#include "mapedit.h"

static char temp_string[180];


extern char cond_string[][60];
extern char event_string[][80];

static char trigg_type_string[][60] ={"Check at start.", "Check Once.", "Check Always."};
static char logic_string[][60] ={"Exact", "Greater", "Lesser"};

static int current_trigger=0;
static int current_cond=0;
static int current_event=0;



//////////////////////////////////////////////////////
////////////THE COND AND EDIT DIALOG//////////////////////////////
///////////////////////////////////////////////////



////////////////Edit cond/////////////////////////////////////////

static char *logic_list(int index, int *list_size)
{
	if(index<0)
	{
		*list_size = 3;
		return NULL;
	}
 
	return logic_string[index];
 
}
	



static char *cond_type_list(int index, int *list_size)
{
	if(index<0)
	{
		*list_size = COND_NUM;
		return NULL;
	}
 
	return cond_string[index];
 
}



	
static int edit_trigg_cond(void)
{
	int ans;


	char temp_string1[40]="";
	char temp_string2[40]="";
	char temp_text[160]="";
	char temp_x[40]="";
	char temp_y[40]="";
	char temp_z[40]="";

	
	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 210, 45,  455,  310,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     280, 320,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     338, 320,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_list_proc,   215,   75,  445,   100  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    cond_type_list},
     
		{  d_text_proc,     220, 230,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "String1:"},
		{  d_edit_proc,     285, 230,  120,   15,   FG_COLOR,   BG_COLOR,     0,  0,   39,  0,   temp_string1},
		
		{  d_text_proc,     410, 230,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "String2:"},
		{  d_edit_proc,     475, 230,  120,   15,   FG_COLOR,   BG_COLOR,     0,  0,   39,  0,   temp_string2},
		
		{  d_text_proc,     220, 270,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "x:"},
		{  d_edit_proc,     265, 270,  120,  15,   FG_COLOR,   BG_COLOR,     0,  0,   39,  0,   temp_x},
		{  d_text_proc,     395, 295,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "state is true:"},
		{  d_edit_proc,     515, 295,  53,  15,   FG_COLOR,   BG_COLOR,     0,  0,   7,  0,   temp_y},
		{  d_text_proc,     425, 270,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "z:"},
		{  d_edit_proc,     465, 270,  120,  15,   FG_COLOR,   BG_COLOR,     0,  0,   39,  0,   temp_z},
		
		{  d_text_proc,     220, 295,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "logic:"},
		{  d_list_proc,     270, 285,  100,  30,   FG_COLOR,   BG_COLOR,     0,  0,   0,  0,   logic_list},
		
		
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_string1, "%s",global_trigger[current_trigger].condition[current_cond].string1);
	sprintf(temp_string2, "%s",global_trigger[current_trigger].condition[current_cond].string2);
	
	sprintf(temp_x, "%s",global_trigger[current_trigger].condition[current_cond].x);
	sprintf(temp_y, "%d",global_trigger[current_trigger].condition[current_cond].correct);
	sprintf(temp_z, "%s",global_trigger[current_trigger].condition[current_cond].z);

	trigger_dialog[3].d1= global_trigger[current_trigger].condition[current_cond].type-1;
	trigger_dialog[15].d1= global_trigger[current_trigger].condition[current_cond].logic;
	
	ans = popup_dialog(trigger_dialog,-1);

	if(ans==1)
	{
		strcpy(global_trigger[current_trigger].condition[current_cond].string1, temp_string1);
		strcpy(global_trigger[current_trigger].condition[current_cond].string2, temp_string2);
		
		strcpy(global_trigger[current_trigger].condition[current_cond].x,temp_x);
		global_trigger[current_trigger].condition[current_cond].correct = atoi(temp_y);
		strcpy(global_trigger[current_trigger].condition[current_cond].z,temp_z);

		global_trigger[current_trigger].condition[current_cond].type = trigger_dialog[3].d1+1;
		global_trigger[current_trigger].condition[current_cond].logic = trigger_dialog[15].d1;

		global_trigger[current_trigger].condition[current_cond].used=1;
	

	}


	return D_O_K;
}


static void make_trigg_cond(void)
{
	

	strcpy(global_trigger[current_trigger].condition[current_cond].x,"0");
	global_trigger[current_trigger].condition[current_cond].correct=1;
	strcpy(global_trigger[current_trigger].condition[current_cond].z,"0");
	global_trigger[current_trigger].condition[current_cond].logic=0;
	
	
	global_trigger[current_trigger].condition[current_cond].type=0;
	
	strcpy(global_trigger[current_trigger].condition[current_cond].string1,"nothing");
	strcpy(global_trigger[current_trigger].condition[current_cond].string2,"nothing");
	
}




////////////////Edit event/////////////////////////////////////////

static char *event_type_list(int index, int *list_size)
{
	if(index<0)
	{
		*list_size = EVENT_NUM;
		return NULL;
	}
 
	return event_string[index];
 
}



	
static int edit_trigg_event(void)
{
	int ans;


	char temp_string1[40]="";
	char temp_string2[40]="";
	char temp_text[160]="";
	char temp_x[40]="";
	char temp_y[40]="";
	char temp_z[40]="";

	
	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)       (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 210, 45,  545,  310,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     280, 310,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     338, 310,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_list_proc,   215,   75,  535,   100  ,   FG_COLOR,  BG_COLOR,    0,    0,  0,    0,    event_type_list},
     
		{  d_text_proc,     220, 230,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "String1:"},
		{  d_edit_proc,     285, 230,  120,   15,   FG_COLOR,   BG_COLOR,     0,  0,   39,  0,   temp_string1},
		
		{  d_text_proc,     410, 230,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "String2:"},
		{  d_edit_proc,     475, 230,  120,   15,   FG_COLOR,   BG_COLOR,     0,  0,   39,  0,   temp_string2},
		
		{  d_text_proc,     220, 250,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Text:"},
		{  d_edit_proc,     285, 250,  280,   15,   FG_COLOR,   BG_COLOR,     0,  0,   159,  0,   temp_text},
		
		{  d_text_proc,     220, 270,  50,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "x:"},
		{  d_edit_proc,     240, 270,  126,  15,   FG_COLOR,   BG_COLOR,     0,  0,   39,  0,   temp_x},
		{  d_text_proc,     370, 270,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "y:"},
		{  d_edit_proc,     390, 270,  126,  15,   FG_COLOR,   BG_COLOR,     0,  0,   39,  0,   temp_y},
		{  d_text_proc,     520, 270,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "z:"},
		{  d_edit_proc,     540, 270,  126,  15,   FG_COLOR,   BG_COLOR,     0,  0,   39,  0,   temp_z},
		
		//{  d_text_proc,     220, 275,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "logic:"},
		//{  d_list_proc,     270, 265,  100,  30,   FG_COLOR,   BG_COLOR,     0,  0,   0,  0,   logic_list},
		
		
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	sprintf(temp_string1, "%s",global_trigger[current_trigger].event[current_event].string1);
	sprintf(temp_string2, "%s",global_trigger[current_trigger].event[current_event].string2);
	sprintf(temp_text, "%s",global_trigger[current_trigger].event[current_event].text);
	
	sprintf(temp_x, "%s",global_trigger[current_trigger].event[current_event].x);
	sprintf(temp_y, "%s",global_trigger[current_trigger].event[current_event].y);
	sprintf(temp_z, "%s",global_trigger[current_trigger].event[current_event].z);

	trigger_dialog[3].d1= global_trigger[current_trigger].event[current_event].type-1;
	
	ans = popup_dialog(trigger_dialog,-1);

	if(ans==1)
	{
		strcpy(global_trigger[current_trigger].event[current_event].string1, temp_string1);
		strcpy(global_trigger[current_trigger].event[current_event].string2, temp_string2);
		strcpy(global_trigger[current_trigger].event[current_event].text, temp_text);

		strcpy(global_trigger[current_trigger].event[current_event].x,temp_x);
		strcpy(global_trigger[current_trigger].event[current_event].y,temp_y);
		strcpy(global_trigger[current_trigger].event[current_event].z,temp_z);

		global_trigger[current_trigger].event[current_event].type = trigger_dialog[3].d1+1;

		global_trigger[current_trigger].event[current_event].used=1;
	}


	return D_O_K;
}


static void make_trigg_event(void)
{
	strcpy(global_trigger[current_trigger].event[current_event].x,"0");
	strcpy(global_trigger[current_trigger].event[current_event].y,"0");
	strcpy(global_trigger[current_trigger].event[current_event].z,"0");
	
	global_trigger[current_trigger].event[current_event].type=0;
	
	strcpy(global_trigger[current_trigger].event[current_event].string1,"nothing");
	strcpy(global_trigger[current_trigger].event[current_event].string2,"nothing");
	strcpy(global_trigger[current_trigger].event[current_event].text,"nothing");
}








////////////////the lists////////////////////////
static char *trigg_event_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MAX_EVENT_NUM;
  return NULL;
 }
 
 if(global_trigger[current_trigger].event[index].used==0)
	 return "Empty";
 else
	 return event_string[global_trigger[current_trigger].event[index].type-1];
 
}




static int d_trigg_event_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

	
 current_event=d->d1;	

 if(ans==D_EXIT)
 {
	if(global_trigger[current_trigger].event[current_event].used==0) 
	
	make_trigg_event();

	edit_trigg_event();
	return D_REDRAW;
 }
 
 return ans;
}




static char *trigg_cond_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MAX_CONDITION_NUM;
  return NULL;
 }
 
 if(global_trigger[current_trigger].condition[index].used==0)
	 return "Empty";
 else
	 return cond_string[global_trigger[current_trigger].condition[index].type-1];
 
}




static int d_trigg_cond_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

	
 current_cond=d->d1;	

 if(ans==D_EXIT)
 {
	if(global_trigger[current_trigger].condition[current_cond].used==0) make_trigg_cond();

	edit_trigg_cond();
	
	return D_REDRAW;
 }
 
 return ans;
}




static int d_delete_cond_button_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_button_proc(msg,d,c);

 
 if(ans==D_EXIT)
 {
    ans = alert("Sure you want to delete the condition?",NULL,NULL,"Yes","Cancel",'\r',0);
	if(ans==2)return D_REDRAW;

	global_trigger[current_trigger].condition[current_cond].used=0;
	return D_REDRAW;
 }
 
 return ans;
}



static int d_delete_event_button_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_button_proc(msg,d,c);

 
 if(ans==D_EXIT)
 {
	ans = alert("Sure you want to delete the event?",NULL,NULL,"Yes","Cancel",'\r',0);
	if(ans==2)return D_REDRAW;

	global_trigger[current_trigger].event[current_event].used=0;
	return D_REDRAW;
 }
 
 return ans;
}








static char *trigg_type_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = 3;
  return NULL;
 }
 
  return trigg_type_string[index];
}





////////////////the dialog////////////////////////
static void edit_cond_and_event(void)
{
	int ans;
	
	char temp_active[5]="";
	char temp_name[30]="";



	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 210, 45,  455,  310,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     280, 310,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     338, 310,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_text_proc,  285,   170,   10,   50  ,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    "Condition:"},
		{  d_trigg_cond_list_proc,  280,   180,   120,   70  ,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    trigg_cond_list},
		
		{  d_text_proc,  465,   170,   10,   50  ,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    "Event:"},
		{  d_trigg_event_list_proc,  460,   180,   120,   70  ,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    trigg_event_list},
		
		{  d_delete_cond_button_proc,     280, 260,  53,   20,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,         "Delete"},
		{  d_delete_event_button_proc,     460, 260,  53,  20,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,        "Delete" },
		
		{  d_text_proc,     280, 100,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Active:"},
		{  d_edit_proc,     345, 100,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   29,  0,   temp_active},
		
		{  d_text_proc,     400, 100,  53,   15,   FG_COLOR,   BG_COLOR,     0,  0,   0,    0,   "Name:"},
		{  d_edit_proc,     465, 100,  150,   15,   FG_COLOR,   BG_COLOR,     0,  0,   29,  0,   temp_name},
		
		{  d_list_proc,     280, 120,  150,   30,   FG_COLOR,   BG_COLOR,     0,  0,   0,  0,   trigg_type_list},
		
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	
	trigger_dialog[13].d1 = global_trigger[current_trigger].type;
	sprintf(temp_name,"%s",global_trigger[current_trigger].name);
	sprintf(temp_active,"%d",global_trigger[current_trigger].active);
	
	ans = popup_dialog(trigger_dialog,-1);

	if(ans==1)
	{
			global_trigger[current_trigger].type = trigger_dialog[13].d1;
			global_trigger[current_trigger].active = atoi(temp_active);
			strcpy(global_trigger[current_trigger].name,temp_name);
	}


}




//////////////////////////////////////////////////////
////////////THE TRIGGER LIST//////////////////////////////
///////////////////////////////////////////////////

static char *trigger_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = GLOBAL_TRIGGER_NUM;
  return NULL;
 }
 
  
 return global_trigger[index].name;
}




static int d_trigger_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

	
 current_trigger=d->d1;	

 if(ans==D_EXIT)
 {	
	
	edit_cond_and_event();
	return D_REDRAW;
 }
 
 return ans;
}



//////////////////////////////////////////////////////
////////////THE TRIGGER BUTTONS//////////////////////////////
///////////////////////////////////////////////////
static int d_new_trigger_button_proc(int msg, DIALOG *d, int c)
{
	int ans,j;

	ans=d_button_proc(msg,d,c);

	if(ans==D_EXIT)
	{
		if(map->num_of_triggers >= MAX_TRIGGER_NUM) //Check so we have space for the object
		{	
			alert("No more triggers can be made!",NULL,NULL,"OK",NULL,0,0);
			return D_REDRAW;
		}


		global_trigger[map->num_of_triggers].type=1;
		global_trigger[map->num_of_triggers].active=1;
		sprintf(global_trigger[map->num_of_triggers].name,"Trigger %d", map->num_of_triggers);
			
			
		for(j=0;j<MAX_CONDITION_NUM;j++)
		{
			global_trigger[map->num_of_triggers].condition[j].used=0;
		}
		
		for(j=0;j<MAX_EVENT_NUM;j++)
		{
			global_trigger[map->num_of_triggers].event[j].used=0;
		}
		
		map->num_of_triggers++;

		return D_REDRAW;
	}	
	
}

static int d_edit_trigger_button_proc(int msg, DIALOG *d, int c)
{
	int ans;

	ans=d_button_proc(msg,d,c);

	if(ans==D_EXIT)
	{
		edit_cond_and_event();
		return D_REDRAW;
	}	
	
}
static int d_delete_trigger_button_proc(int msg, DIALOG *d, int c)
{
	int ans,i;

	ans=d_button_proc(msg,d,c);

	if(ans==D_EXIT)
	{
		ans = alert("Sure you want to delete the trigger?",NULL,NULL,"Yes","Cancel",'\r',0);

		if(ans==2)return D_REDRAW;

		for(i=current_trigger;i<map->num_of_triggers-1;i++)
		{
			strcpy(global_trigger[i].name, global_trigger[i+1].name);
			global_trigger[i].active=global_trigger[i+1].active;
			global_trigger[i].type=global_trigger[i+1].type;
			
			memcpy(global_trigger[i].condition, global_trigger[i+1].condition, sizeof(global_trigger[i].condition));
			memcpy(global_trigger[i].event, global_trigger[i+1].event, sizeof(global_trigger[i].event));
			 	 
		 
 		}
		

		return D_REDRAW;
	}	
	
}

//////////////////////////////////////////////////////
////////////THE MAIN DIALOG//////////////////////////////
///////////////////////////////////////////////////


int edit_global_triggers(void)
{
	int ans;
	
	
	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 60, 40,  610,  330,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     70, 330,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     128, 330,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_trigger_list_proc,   70,   150,     180,   150  ,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    trigger_list},
     
		//{  d_new_trigger_button_proc,     255, 155,  53,   20,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,         "New"},
		{  d_edit_trigger_button_proc,     255, 185,  53,  20,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,        "Edit" },
		{  d_delete_trigger_button_proc,     255, 215,  53,  20,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,        "Delete" },
		
			
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	ans = popup_dialog(trigger_dialog,-1);


	return D_O_K;
}












