////////////////////////////////////////////////////
// This file contains the funcs for showing a box message
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"

#define MAX_MESSAGE_NUM 50

//properties of the meesage
#define border_color_max makecol(0,0,0)
#define border_color_min makecol(80,80,80)
#define message_bg_col makecol(0,0,0)
#define message_text_col makecol(255,255,255)
#define message_text_effect_col makecol(70,70,70)
#define message_font     font_avalon->dat

#define MESSAGE_X 50
#define MESSAGE_Y 30
#define MESSAGE_H 90
#define MESSAGE_W 380

#define MESSAGE_CLOSE_SPEED 3

static int message_box_h;
static int message_box_w;
static int message_box_x;
static int message_box_y;
 

//A varibale so that things aren't used after the message is out..
extern int action_key_down;


//some variables describing appearance
int border_w=7;
int text_speed=1;
int text_effect_offset=3;

//stuff used betwwen functions
int message_x,message_y;
unsigned int current_char=0;

char text_string[MAX_MESSAGE_NUM][400];
char text_string_face[MAX_MESSAGE_NUM][50];

int active_text_string=0;
int text_string_num=0;


//globals
int message_active=0;



//draw at text with "length" number of characters....that changes row if a word don't fit in max_w 
void draw_formated_message_text(BITMAP *dest, char *string, FONT* the_font,int x,int y, int max_w, int length)
{
	int i,j;
	int row=0,col=0;
	int temp;
	char temp_m_string[2]="";
	char temp_m_string2[400]="";
	int font_h = text_height(the_font);
	int string_length = strlen(string);
	
	//This can deleted....don't know why it is here.....
	//text_mode(-1);
	
	for(i=0;i<length;i++)
	{
		//set the charcer in use in a temp string
		temp_m_string[0] = string[i];
		temp_m_string[1] = '\0';

		//print it to screen
		
		textout_ex(dest,the_font,temp_m_string,x+col,y+row,message_text_effect_col,-1);
		
		textout_ex(dest,the_font,temp_m_string,x+col,y+row+text_effect_offset,message_text_col,-1);
				
		//go forward with the width of the character
		col+=text_length(the_font,temp_m_string);

		//if the character was a space check the next coming words length
		if(temp_m_string[0]==' ' || temp_m_string[0]=='_')
		{
			temp=0;
			//check the length of the word
			while(string[i+1+temp]!=' ' && string[i+1+temp]!='_' && string[i+1+temp]!='\0' )//&& temp+1+i<length)
				temp++;

			//clear the temp string for the word....
			for(j=0;j<40;j++)
				temp_m_string2[j]='\0';
			//copy the word to a temp string
			strncpy(temp_m_string2,string + i+1,temp);
			
			//if it don't fits on the row make a new row...
			if(col + text_length(the_font,temp_m_string2) >max_w)
			{
				row+=font_h;
				col=0;
			}
		}


	}

}




//make a new message string...change the text displayed in the message
void make_new_message_string(char* string)
{
		strcpy(text_string[active_text_string], string);
}		



int get_head_num(char *name)
{
	int i, num=-1;
	char error_string[40];

	for(i=0;i<num_of_message_faces;i++)
	{
		if(strcmp(message_face[i].name, name)==0)
		{
			num=i;
			break;
		}
	}

	if(num<0)
	{
		sprintf(error_string,"Script error: Face \"%s\" does not exist",name);
		make_engine_error(error_string);
	}

	return num;
}

//draw a message
void draw_message(void)
{
	int num=-1;
	float add;
	//if there is a message active....
	if(message_active)
	{
		
		if(text_string_face[active_text_string][0]!=0)
			num = get_head_num(text_string_face[active_text_string]);
				
		//draw the box....
		if(text_string_face[active_text_string][0]!=0)
		{
			draw_message_box(virt,12,message_box_y,message_box_w+65,message_box_h);
			
			//if(message_box_y > MESSAGE_Y)
			if(num!=-1)
			{
					add = (MESSAGE_H/2-(float)(message_box_y-MESSAGE_Y))/(MESSAGE_H/2);
					stretch_sprite(virt,message_face[num].pic, 12,message_box_y,message_face[num].pic->w, message_face[num].pic->h *add );
			}
		}
		else
		{
			draw_message_box(virt,message_box_x,message_box_y,message_box_w,message_box_h);
		}

		
		//if the box if ready draw the text...
		if(message_box_y <= MESSAGE_Y)
		{
			if(text_string_face[active_text_string][0]!=0)
			{
				draw_formated_message_text(virt,text_string[active_text_string],message_font,72, message_box_y, message_box_w, current_char);
			}
			else
			{
				draw_formated_message_text(virt,text_string[active_text_string],message_font,message_box_x, message_box_y, message_box_w, current_char);
			}
		}
		
	}
	else if(message_box_h>0)//if no message active and box not small keep drawing it...
	{
		draw_message_box(virt,message_box_x,message_box_y,message_box_w,message_box_h);
		
	}

}


//update a message
void update_message_text(void)
{
	static int a_key_down=0;

	static int next_char=0;

	//check so that the button push for ending a message becomes correct
	if(!key[key_action]) a_key_down = 0;
	
	if(message_active)
	{	
		
		if(current_char < strlen(text_string[active_text_string]))
		{
			next_char++;
		
			if(key[key_action] && (a_key_down==0 || a_key_down==2))
			{
				next_char+=3;
				a_key_down=2;

			}
						
			if(next_char>text_speed)
			{		
				current_char++;
				next_char=0;
			}

		}
		else
		{
			
			if(key[key_action] && !a_key_down)
			{
				active_text_string++;
				if(active_text_string>=text_string_num)
				{
					current_char=0;
					message_active=0;
					action_key_down=1; //so that things aren't used when you finnish the message
				}
				else
				{
					current_char=0;
					next_char=0;	
				}

				a_key_down = 1;
			}
			

		}

	}


}







//Draws a bow with borders....
void draw_message_box(BITMAP *dest, int x, int y, int w, int h)
{
	int i;
	int color=border_color_max;
	float r_add,g_add,b_add;
	float r,g,b;

	x-=border_w;//How bih is the box with borders...
	y-=border_w;
	h+=border_w*2;
	w+=border_w*2;
	
	r = getr(border_color_max);//get the maxcolor's r,g,b values
	g = getg(border_color_max);
	b = getb(border_color_max);
		
	//Interpolate...hehe....it's sounds advanced but it's not...
	//We just get how much we add to  r,g,b each time until
	//we have the color_min
	r_add = (float)(getr(border_color_min)-getr(border_color_max))/(float)(border_w/2);
	g_add = (float)(getg(border_color_min)-getg(border_color_max))/(float)(border_w/2);
	b_add = (float)(getb(border_color_min)-getb(border_color_max))/(float)(border_w/2);
		
	
	//draw a bordew_w thick border
	for(i=0;i<border_w;i++)
	{
		//draw a rect with the color in color!
		rect(dest,x+i,y+i,x+w+(border_w-i),y+h+(border_w-i),color);
			
		
		//make the new r,g,b values if are over  half the distance
		//we go backwards and go towards the max color
		if(i<border_w/2)
		{
			r+=r_add;
			g+=g_add;
			b+=b_add;
		}
		else
		{
			r-=r_add;
			g-=g_add;
			b-=b_add;
		}

		
		color = makecol(r,g,b);
	}
	
	//Do the rect in the middle
	rectfill(dest,x+border_w,y+border_w,x+w, y+h,message_bg_col);




}


void update_message(void)
{
	if(message_active)
	{
		if(message_box_y>MESSAGE_Y || message_box_h<MESSAGE_H)
		{
			if(message_box_y>MESSAGE_Y)
				message_box_y-=MESSAGE_CLOSE_SPEED*2;
			
			if(message_box_h<MESSAGE_H)
				message_box_h+=(MESSAGE_CLOSE_SPEED*4);
			
						
		}
		else
		{
			update_message_text();

		}


	}
	else if(message_box_h>0)
	{
		message_box_y+=MESSAGE_CLOSE_SPEED*2;
		
		message_box_h-=(MESSAGE_CLOSE_SPEED*4);
		if(message_box_h<0)message_box_h=0;


	}
		
		


}



void make_new_message(char *string, char *face)
{
	if(!message_active && message_box_h<=0)
	{
		message_box_x = MESSAGE_X;
		message_box_y = MESSAGE_Y + MESSAGE_H/2;
		message_box_h = 0;
		message_box_w = MESSAGE_W;
		message_active = 1;
		
		strcpy(text_string[0], string);
		
		active_text_string=0;
		text_string_num=1;
						
		if(face!=NULL)
			strcpy(text_string_face[0],face);
		else
			text_string_face[0][0] = 0;

	}
	else if(text_string_num<MAX_MESSAGE_NUM)
	{
		text_string_num++;
		
		strcpy(text_string[text_string_num-1],string);
		
		if(face!=NULL)
			strcpy(text_string_face[text_string_num-1],face);
		else
			text_string_face[text_string_num-1][0] = 0;
	}

}






