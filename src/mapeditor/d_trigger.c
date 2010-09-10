#include <allegro.h>

#include <stdio.h>

#include "../fiend.h"
#include "mapedit.h"

static char temp_string[180];


char cond_string[][60]={
"1.  Player is in area [string1].",
"2.  Player energy is [logic] [x].",
"3.  Player triggers object [string1].",
"4.  Player triggers area [string1].",
"5.  Player triggers enemy [string1].",
"6.  Player use item [string1] on object [string2].",
"7.  Player use item [string1] on npc [string2].",
"8.  Player use item [string1] on area [string2].",
"9.  Player use item [string1] on enemy [string2].",
"10. Player has item [string1].",
"11. Enemy [string1] energy is [logic] [x].",
"12. Enemy [string1] mission is [x].",
"13. Enemy [string1] is in area [string2].",
"14. NPC [string1] energy is [logic] [x].",
"15. NPC [string1] is in area [string2].",
"16. Object [string1] energy is [logic] [x].",
"17. Object [string1] is in area [string2].",
"18. Lightlevel is [logic] [x].",
"19. Global var [string1] is [logic] [x].",
"20. Local var [string1] is [logic] [x].",
"21. Item [string1] is picked up",
"22. Item [string1] is abled",
"23. Npc [string1] is abled",
"24. Enemy [string1] is abled",
"25. Object [string1] is abled",
"26. Player is automoving",
"27. Message is active",
"28. Random one out of [x]",
"29. Always.",
"30. NPC [string1] dialog is [logic] [x]",
"31. Map outside is [logic] [x]",
""
};




char event_string[][80]={
"1.  Player energy becomes [x].",
"2.  Player energy adds with [x].",
"3.  Player posistion becomes  [x] [y].",
"4.  Player map becomes [text].",
"5.  Player gets item [string1].",
"6.  Player active becomes [x].",
"7.  Player angle becomes [x]",
"8.  Player action becomes [string1] [x] times.",

"9.  Enemy [string1] energy becomes [x].",
"10. Enemy [string1] energy adds with [x].",
"11. Enemy [string1] posistion becomes [x] [y].",
"12. Enemy [string1] map becomes [string2].",
"13. Enemy [string1] know player pos",
"14. Enemy [string1] active becomes [x].",
"15. Enemy [string1] dead becomes [x]",
"16. Enemy [string1] action becomes [string2] [x] times.",
"17. Enemy [string1] mission becomes [x].",
"18. Enemy [string1] runs to [x] [y].",
"19. Enemy [string1] movement path becomes [x].",

"20. NPC [string1] energy becomes [x].",
"21. NPC [string1] energy adds with [x].",
"22. NPC [string1] posistion becomes [x] [y].",
"23. NPC [string1] map becomes [string1].",
"24. NPC [string1] makes ambient sound [x]",
"25. NPC [string1] active becomes [x].",
"26. NPC [string1] active toggles.",
"27. NPC [string1] action becomes [string2] [x] times.",
"28. NPC [string1] dialog becomes [x].",
"29. NPC [string1] movement path becomes [x].",
"30. NPC [string1] AI state becomes [x].",

"31. Object [string1] energy becomes [x].",
"32. Object [string1] energy adds with [x].",
"33. Object [string1] posistion becomes [x] [y].",
"34. Object [string1] goes to posistion [x] [y] with speed [z].",
"35. Object [string1] active becomes [x].",
"36. Object [string1] active toggles.",
"37. Object [string1] action becomes [x].",

"38. Item [string1] posistion becomes [x] [y].",
"39. Item [string1] active becomes [x].",
"40. Item [string1] active toggles.",

"41. Area [string1] posistion becomes [x] [y].",
"42. Area [string1] active becomes [x].",
"43. Area [string1] active toggles.",

"44. Link [string1] event becomes [x].",

"45. Light [z] posistion becomes [x] [y].",
"46. Light [string1] goes to posistion [x] [y] with speed [z].",
"47. Light [z] active becomes [x].",
"48. Light [z] active toggles.",
"49. Light [z] flash becomes [x].",
"50. Light [z] flash toggles.",


"51. Lightlevel becomes [x].",
"52. Lightlevel adds with [x].",
"53. Lightlevel fades to [x] with speed [y].",

"54. Show message [text] with face [string1].",
"55. Show image [string1].",

"56. Make explosion [s1] with level [x],time [y] at area [s2]",
"57. Make earthquake size [z] for [x] time.",
"58. Make bloodpool size [z] at area [string1], color [string2] (r g b).",
"59. Make beam [text] [x] time from area [string1] to [string2]",
"60. Make pickup text [text] for [x] time.",
"61. Make thunder",
"62. Make rain size [z]/100 chance of drop.",
"63. Make xxx",

"64. Soundemitor [z] posistion becomes [x] [y].",
"65. Soundemitor [z] active becomes [x].",
"66. Soundemitor [z] active toggles.",

"67. Play sound [string1] at area [string2].loop is [x]",
"68. Play music [string1].loop is [x]",
"69. Stop sound [string1]",
"70. Stop music [string1]",

"71. Global var [string1] becomes [x].",
"72. Global var [string1] adds with [x].",
"73. Local var [string1] becomes [x].",
"74. Local var [string1] adds with [x].",

"75. Automove player to [x] [y].",
"76. Pause Automove",
"77. Resume Automove",
"78. Toggle Pause Automove",
"79. Stop Automove",

"80. Fade out speed x",
"81. Fade in speed x",

"82. Autosave game",

"83. Outside lightlevel becomes [x]",

"84. Quit to menu",
"85. Complete game",

"86. Nothing.",

	
""
};

static char trigg_type_string[][60] ={"Check at start.", "Check Once.", "Check Always."};
static char logic_string[][60] ={"Exact", "Greater", "Lesser"};

static int current_trigger=0;
static int current_cond=0;
static int current_event=0;



//////////////////////////////////////////////////////
////////////THE COND AND EDIT DIALOG//////////////////////////////
///////////////////////////////////////////////////



////////////////Edit cond/////////////////////////////////////////

char *logic_list(int index, int *list_size)
{
	if(index<0)
	{
		*list_size = 3;
		return NULL;
	}
 
	return logic_string[index];
 
}
	



char *cond_type_list(int index, int *list_size)
{
	if(index<0)
	{
		*list_size = COND_NUM;
		return NULL;
	}
 
	return cond_string[index];
 
}



	
int edit_trigg_cond(void)
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

	sprintf(temp_string1, "%s",map->trigger[current_trigger].condition[current_cond].string1);
	sprintf(temp_string2, "%s",map->trigger[current_trigger].condition[current_cond].string2);
	
	sprintf(temp_x, "%s",map->trigger[current_trigger].condition[current_cond].x);
	sprintf(temp_y, "%d",map->trigger[current_trigger].condition[current_cond].correct);
	sprintf(temp_z, "%s",map->trigger[current_trigger].condition[current_cond].z);

	trigger_dialog[3].d1= map->trigger[current_trigger].condition[current_cond].type-1;
	trigger_dialog[15].d1= map->trigger[current_trigger].condition[current_cond].logic;
	
	ans = popup_dialog(trigger_dialog,-1);

	if(ans==1)
	{
		strcpy(map->trigger[current_trigger].condition[current_cond].string1, temp_string1);
		strcpy(map->trigger[current_trigger].condition[current_cond].string2, temp_string2);
		
		strcpy(map->trigger[current_trigger].condition[current_cond].x,  temp_x);
		map->trigger[current_trigger].condition[current_cond].correct = atoi(temp_y);
		strcpy(map->trigger[current_trigger].condition[current_cond].z,  temp_z);

		map->trigger[current_trigger].condition[current_cond].type = trigger_dialog[3].d1+1;
		map->trigger[current_trigger].condition[current_cond].logic = trigger_dialog[15].d1;

		map->trigger[current_trigger].condition[current_cond].used=1;
	

	}


	return D_O_K;
}


void make_trigg_cond(void)
{
	

	strcpy(map->trigger[current_trigger].condition[current_cond].x,"0");
	map->trigger[current_trigger].condition[current_cond].correct=1;
	strcpy(map->trigger[current_trigger].condition[current_cond].z,"0");
	map->trigger[current_trigger].condition[current_cond].logic=0;
	
	
	map->trigger[current_trigger].condition[current_cond].type=0;
	
	strcpy(map->trigger[current_trigger].condition[current_cond].string1,"nothing");
	strcpy(map->trigger[current_trigger].condition[current_cond].string2,"nothing");
	
}




////////////////Edit event/////////////////////////////////////////

char *event_type_list(int index, int *list_size)
{
	if(index<0)
	{
		*list_size = EVENT_NUM;
		return NULL;
	}
 
	return event_string[index];
 
}



	
int edit_trigg_event(void)
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

	sprintf(temp_string1, "%s",map->trigger[current_trigger].event[current_event].string1);
	sprintf(temp_string2, "%s",map->trigger[current_trigger].event[current_event].string2);
	sprintf(temp_text, "%s",map->trigger[current_trigger].event[current_event].text);
	
	sprintf(temp_x, "%s",map->trigger[current_trigger].event[current_event].x);
	sprintf(temp_y, "%s",map->trigger[current_trigger].event[current_event].y);
	sprintf(temp_z, "%s",map->trigger[current_trigger].event[current_event].z);

	trigger_dialog[3].d1= map->trigger[current_trigger].event[current_event].type-1;
	
	ans = popup_dialog(trigger_dialog,-1);

	if(ans==1)
	{
		strcpy(map->trigger[current_trigger].event[current_event].string1, temp_string1);
		strcpy(map->trigger[current_trigger].event[current_event].string2, temp_string2);
		strcpy(map->trigger[current_trigger].event[current_event].text, temp_text);

		strcpy(map->trigger[current_trigger].event[current_event].x,temp_x);
		strcpy(map->trigger[current_trigger].event[current_event].y,temp_y);
		strcpy(map->trigger[current_trigger].event[current_event].z,temp_z);

		map->trigger[current_trigger].event[current_event].type = trigger_dialog[3].d1+1;

		map->trigger[current_trigger].event[current_event].used=1;
	}


	return D_O_K;
}


void make_trigg_event(void)
{
	

	strcpy(map->trigger[current_trigger].event[current_event].x,"0");
	strcpy(map->trigger[current_trigger].event[current_event].y,"0");
	strcpy(map->trigger[current_trigger].event[current_event].z,"0");
	
	map->trigger[current_trigger].event[current_event].type=0;
	
	strcpy(map->trigger[current_trigger].event[current_event].string1,"nothing");
	strcpy(map->trigger[current_trigger].event[current_event].string2,"nothing");
	strcpy(map->trigger[current_trigger].event[current_event].text,"nothing");
}








////////////////the lists////////////////////////
char *trigg_event_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MAX_EVENT_NUM;
  return NULL;
 }
 
 if(map->trigger[current_trigger].event[index].used==0)
	 return "Empty";
 else
	 return event_string[map->trigger[current_trigger].event[index].type-1];
 
}




int d_trigg_event_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

	
 current_event=d->d1;	

 if(ans==D_EXIT)
 {
	if(map->trigger[current_trigger].event[current_event].used==0) 
	
	make_trigg_event();

	edit_trigg_event();
	return D_REDRAW;
 }
 
 return ans;
}




char *trigg_cond_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = MAX_CONDITION_NUM;
  return NULL;
 }
 
 if(map->trigger[current_trigger].condition[index].used==0)
	 return "Empty";
 else
	 return cond_string[map->trigger[current_trigger].condition[index].type-1];
 
}




int d_trigg_cond_list_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_list_proc(msg,d,c);

	
 current_cond=d->d1;	

 if(ans==D_EXIT)
 {
	if(map->trigger[current_trigger].condition[current_cond].used==0) make_trigg_cond();

	edit_trigg_cond();
	
	return D_REDRAW;
 }
 
 return ans;
}




int d_delete_cond_button_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_button_proc(msg,d,c);

 
 if(ans==D_EXIT)
 {
    ans = alert("Sure you want to delete the condition?",NULL,NULL,"Yes","Cancel",'\r',0);
	if(ans==2)return D_REDRAW;

	map->trigger[current_trigger].condition[current_cond].used=0;
	return D_REDRAW;
 }
 
 return ans;
}



int d_delete_event_button_proc(int msg, DIALOG *d, int c)
{
 int ans;

 ans=d_button_proc(msg,d,c);

 
 if(ans==D_EXIT)
 {
	ans = alert("Sure you want to delete the event?",NULL,NULL,"Yes","Cancel",'\r',0);
	if(ans==2)return D_REDRAW;

	map->trigger[current_trigger].event[current_event].used=0;
	return D_REDRAW;
 }
 
 return ans;
}








char *trigg_type_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = 3;
  return NULL;
 }
 
  return trigg_type_string[index];
}





////////////////the dialog////////////////////////
void edit_cond_and_event(void)
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

	
	trigger_dialog[13].d1 = map->trigger[current_trigger].type;
	sprintf(temp_name,"%s",map->trigger[current_trigger].name);
	sprintf(temp_active,"%d",map->trigger[current_trigger].active);
	
	ans = popup_dialog(trigger_dialog,-1);

	if(ans==1)
	{
			map->trigger[current_trigger].type = trigger_dialog[13].d1;
			map->trigger[current_trigger].active = atoi(temp_active);
			strcpy(map->trigger[current_trigger].name,temp_name);
	}


}




//////////////////////////////////////////////////////
////////////THE TRIGGER LIST//////////////////////////////
///////////////////////////////////////////////////

char *trigger_list(int index, int *list_size)
{
 if(index<0)
 {
  *list_size = map->num_of_triggers;
  return NULL;
 }
 
  
 return map->trigger[index].name;
}




int d_trigger_list_proc(int msg, DIALOG *d, int c)
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
int d_new_trigger_button_proc(int msg, DIALOG *d, int c)
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


		map->trigger[map->num_of_triggers].type=1;
		map->trigger[map->num_of_triggers].active=1;
		sprintf(map->trigger[map->num_of_triggers].name,"Trigger %d", map->num_of_triggers);
			
			
		for(j=0;j<MAX_CONDITION_NUM;j++)
		{
			map->trigger[map->num_of_triggers].condition[j].used=0;
		}
		
		for(j=0;j<MAX_EVENT_NUM;j++)
		{
			map->trigger[map->num_of_triggers].event[j].used=0;
		}
		
		map->num_of_triggers++;

		return D_REDRAW;
	}	
	
}

int d_edit_trigger_button_proc(int msg, DIALOG *d, int c)
{
	int ans;

	ans=d_button_proc(msg,d,c);

	if(ans==D_EXIT)
	{
		if(map->num_of_triggers>0)edit_cond_and_event();
		return D_REDRAW;
	}	
	
}
int d_delete_trigger_button_proc(int msg, DIALOG *d, int c)
{
	int ans,i;

	ans=d_button_proc(msg,d,c);

	if(ans==D_EXIT)
	{
		ans = alert("Sure you want to delete the trigger?",NULL,NULL,"Yes","Cancel",'\r',0);

		if(ans==2)return D_REDRAW;

		for(i=current_trigger;i<map->num_of_triggers-1;i++)
		{
			strcpy(map->trigger[i].name, map->trigger[i+1].name);
			map->trigger[i].active=map->trigger[i+1].active;
			map->trigger[i].type=map->trigger[i+1].type;
			
			memcpy(map->trigger[i].condition, map->trigger[i+1].condition, sizeof(map->trigger[i].condition));
			memcpy(map->trigger[i].event, map->trigger[i+1].event, sizeof(map->trigger[i].event));
			 	 
		 
 		}
		map->num_of_triggers--;



		return D_REDRAW;
	}	
	
}

//////////////////////////////////////////////////////
////////////THE MAIN DIALOG//////////////////////////////
///////////////////////////////////////////////////


int edit_triggers(void)
{
	int ans;
	
	
	DIALOG trigger_dialog[] =
	{
   /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)  (bg)  (key) (flags)  (d1)  (d2)         (dp) */
		{  d_shadow_box_proc, 60, 40,  610,  330,  FG_COLOR,   BG_COLOR,     0,     0,     0,     0,           NULL},
		
		
		{  d_button_proc,     70, 330,  53,   20,   FG_COLOR,   BG_COLOR,     '\r',  D_EXIT,   0,    0,         "OK"},
		{  d_button_proc,     128, 330,  53,  20,   FG_COLOR,   BG_COLOR,     27,  D_EXIT,   0,    0,        "Cancel" },
		
		{  d_trigger_list_proc,   70,   150,     180,   150  ,   FG_COLOR,  BG_COLOR,    0,    D_EXIT,  0,    0,    trigger_list},
     
		{  d_new_trigger_button_proc,     255, 155,  53,   20,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,         "New"},
		{  d_edit_trigger_button_proc,     255, 185,  53,  20,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,        "Edit" },
		{  d_delete_trigger_button_proc,     255, 215,  53,  20,   FG_COLOR,   BG_COLOR,     0,  D_EXIT,   0,    0,        "Delete" },
		
			
		{  NULL,              0,   0,    0,     0,    0,    0,     0,     0,     0,    0,    NULL}
	};

	ans = popup_dialog(trigger_dialog,-1);


	return D_O_K;
}










