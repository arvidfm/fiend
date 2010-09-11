////////////////////////////////////////////////////
// This file contains the note drawing/logic code 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////


#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"
#include "../draw.h"


int fiend_note_is_on=0;

static NOTE_DATA *note;

static int page_chapter[100];
static int page_word[100];

extern int pickup_key_down;

#define FONT_NUM 1
struct
{
	FONT *font;
	char name[40];
}font_info[FONT_NUM]={NULL,"AVALON"};

static void setup_font_info(void)
{
	int i;

	for(i=0;i<FONT_NUM;i++)
		font_info[i].font = font_avalon->dat;
}

static void clear_fiend_note(void)
{
	int i;

	for(i=0;i<MAX_CHAPTER_NUM;i++)
	{
		note->chapter[i].allignment =0;
		note->chapter[i].begin_property =0;
		note->chapter[i].end_property =0;
	}

	for(i=0;i<100;i++)
	{
		page_chapter[i]=0;
		page_word[i]=0;
	}

	page_chapter[0]=0;
	page_word[0]=0;
}



 

int load_fiend_note(char *file)
{
	FILE *f;
	
	char temp_line[1000];
	
	char buffer[80];
	char temp_string[8][40];
	
	int file_ended=0,i,found_font,num;

	int chapter_begin=1;

	clear_fiend_note();

	f = fopen(file,"rb");
	if(f==NULL)return 0;

	//Get the first line
	if(fgets(temp_line,299,f)==NULL)
		return 0;
	
	//Check what the 2 main properties of the note is
	sscanf(temp_line,"%s %s %s %s",buffer,temp_string[0],buffer,temp_string[1]);
	
		//get the type of doument
	if( _stricmp(temp_string[0], "SCROLL")==0)
		note->type = NOTE_TYPE_SCROLLING;
	else if( _stricmp(temp_string[0], "PAGES")==0)
		note->type = NOTE_TYPE_PAGES;
	else 
		note->type = NOTE_TYPE_SCROLLING;
	
	
	//Get the font type
	found_font = 0;
	setup_font_info();

	for(i=0;i<FONT_NUM;i++)
	{
		if(_stricmp(font_info[i].name,temp_string[1])==0)
		{
			found_font =1;
			note->font = font_info[i].font;
		}
	}

	if(!found_font)
		note->font = font_avalon->dat;
	

	///////// Check The Other Lines ///////
	note->num_of_chapters=1;
	strcpy(note->chapter[0].text,"");
	chapter_begin = 1;

	while(fgets(temp_line,299,f)!=NULL && file_ended==0)
	{
		sscanf(temp_line,"%s",temp_string[0]);
		
		//if the line is a property row
		if(strcmp(temp_string[0],";")==0)
		{
			for(i=0;i<8;i++)
				strcpy(temp_string[i],"");
			
			num = sscanf(temp_line,"%s %s %s %s",temp_string[0],temp_string[1],temp_string[2],temp_string[3]);
			
			if(chapter_begin)
			{
				if(num!=EOF)
					for(i=1;i<num;i++)
					{
						if(_stricmp(temp_string[i],"LINE")==0)
							note->chapter[note->num_of_chapters-1].begin_property = 2;
						else if(_stricmp(temp_string[i],"CENTERSCREEN")==0)
							note->chapter[note->num_of_chapters-1].begin_property = 1;
						else if(_stricmp(temp_string[i],"RIGHT")==0)
							note->chapter[note->num_of_chapters-1].allignment = 1;
						else if(_stricmp(temp_string[i],"CENTER")==0)
							note->chapter[note->num_of_chapters-1].allignment = 2;
					}
				chapter_begin =0;
			}
			else
			{
				if(num!=EOF)
					for(i=1;i<num;i++)
					{
						if(_stricmp(temp_string[i],"LINE")==0)
							note->chapter[note->num_of_chapters-1].end_property = 2;
						else if(_stricmp(temp_string[i],"PAGE")==0)
							note->chapter[note->num_of_chapters-1].end_property = 1;
						else if(_stricmp(temp_string[i],"END_OF_FILE")==0)
							file_ended=1;
					}

				note->num_of_chapters++;
				strcpy(note->chapter[note->num_of_chapters-1].text,"");
				chapter_begin =1;
			}
		}
		//if the line is a normal text row
		else
		{
			if(strlen(temp_line)>2)
			{
				temp_line[strlen(temp_line)-2]=0;
			}
			
			if(strcmp(note->chapter[note->num_of_chapters-1].text,"")!=0 && note->chapter[note->num_of_chapters-1].text[strlen(note->chapter[note->num_of_chapters-1].text)-1]!=' ')
				strcat(note->chapter[note->num_of_chapters-1].text," ");

			strcat(note->chapter[note->num_of_chapters-1].text,temp_line);
			chapter_begin =0;
		}
	}

	fclose(f);

	return 1;
}




void read_fiend_note(char *file)
{
	char *file_path ="data/notes/";
	char final_path[80];
	char error[70];
	
	sprintf(final_path,"%s%s",file_path,file);
	
	if(!load_fiend_note(final_path))
	{
		sprintf(error,"Couldn't open note \"%s\"",final_path);
		make_engine_error(error);
		return;
	}
	
	fiend_note_is_on = 1;
	inventory_is_on=0;
	speed_counter = 0;
}



#define NOTE_WIDTH 420
#define NOTE_HEIGHT 340
#define NOTE_DARKNESS 6

static BITMAP *black_map=NULL;
static BITMAP *arrow_up=NULL;
static BITMAP *arrow_down=NULL;
static BITMAP *arrow_left=NULL;
static BITMAP *arrow_right=NULL;

static int main_row=0; 
static int main_page=0;
static int max_rows;
static int row_add;
static int row_begin;
static int col_begin;
static int row=0;
static int col=0;

static int more_down =0;
static int more_up =0;
static int more_right =0;
static int more_left =0;


void init_fiend_note(void)
{
	black_map = create_bitmap_ex(8,480,480);
	clear_to_color(black_map,NOTE_DARKNESS);


	note = malloc(sizeof(NOTE_DATA));
}

void release_fiend_note(void)
{
	destroy_bitmap(black_map);

	free(note);
}




void clear_note_data(void)
{
	main_row=0; 
	main_page=0;
	max_rows;
	row_add;
	row_begin;
	col_begin;
	row=0;
	col=0;
}

static void draw_arrow_down(int x, int y)
{
	triangle(virt,x-18,y-7,x+18,y-7,x,y+7,makecol(55,55,55));
	triangle(virt,x-14,y-5,x+14,y-5,x,y+5,makecol(255,255,255));
}

static void draw_arrow_up(int x, int y)
{
	triangle(virt,x+18,y+7,x-18,y+7,x,y-7,makecol(55,55,55));
	triangle(virt,x+14,y+5,x-14,y+5,x,y-5,makecol(255,255,255));
}


static void draw_arrow_left(int x, int y)
{
	triangle(virt,x+7,y+18,x+7,y-18,x-7,y,makecol(55,55,55));
	triangle(virt,x+5,y+14,x+5,y-14,x-5,y,makecol(255,255,255));
}

static void draw_arrow_right(int x, int y)
{
	triangle(virt,x-7,y-18,x-7,y+18,x+7,y,makecol(55,55,55));
	triangle(virt,x-5,y-14,x-5,y+14,x+5,y,makecol(255,255,255));
}

static void print_note_text(char *string,int x, int y,int mode)
{
	if(mode==0)
	{
		textprintf(virt,note->font,x-1,y-1,makecol(55,55,55),string);
		textprintf(virt,note->font,x+1,y+1,makecol(55,55,55),string);
		textprintf(virt,note->font,x,y,makecol(255,255,255),string);
	}
	else if(mode==1)
	{
		x-=text_length(note->font,string);
		
		textprintf(virt,note->font,x-1,y-1,makecol(55,55,55),string);
		textprintf(virt,note->font,x+1,y+1,makecol(55,55,55),string);
		textprintf(virt,note->font,x,y,makecol(255,255,255),string);
	}
	else if(mode==2)
	{
		textprintf_centre(virt,note->font,x-1,y-1,makecol(55,55,55),string);
		textprintf_centre(virt,note->font,x+1,y+1,makecol(55,55,55),string);
		textprintf_centre(virt,note->font,x,y,makecol(255,255,255),string);
	}
	
}


//get a word, number <word_num> in a string
static int get_word(char *dest, char *src,int word_num)
{
	int i =-1,j;
	int num=-1;
	
	do
	{
		i++;
		if(src[i]=='\0')
			return -1;	

		if(src[i]!=' ')
		{
			j=0;
			do
			{
				dest[j] = src[i];
				i++;
				j++;
			}while(src[i]!=' ' && src[i]!='\0');
						
			dest[j] = '\0';
		
			num++;

			if(src[i]=='\0')
			{
				if(num==word_num)
					return i-strlen(dest);
				else
					return -1;
			}
		}
	}while(num<word_num);

	
	return i-strlen(dest);
}



static int draw_normal_chapter(int num, int word)
{
	int i=word;
	char temp_string[70];
		
	while(get_word(temp_string,note->chapter[num].text,i)>-1)
	{
		
			
		if(col+text_length(note->font,temp_string) < NOTE_WIDTH)
		{
			if(row>=max_rows+main_row)
				return i;
						
			if(row >= main_row)
			{
				print_note_text(temp_string,col_begin+col,(row-main_row)*row_add+row_begin,0);
				col += text_length(note->font,temp_string);
				print_note_text(" ",col_begin+col,row*row_add+row_begin-main_row,0);
				col += text_length(note->font," ");
			}
			else
			{
				col += text_length(note->font,temp_string);
				col += text_length(note->font," ");
			}

		}
		else
		{
			col=0;
			row++;

			if(row>=max_rows+main_row)
				return i;
			
			if(row >= main_row)
			{
				print_note_text(temp_string,col_begin+col,(row-main_row)*row_add+row_begin,0);
				col += text_length(note->font,temp_string);
				print_note_text(" ",col_begin+col,(row-main_row)*row_add+row_begin,0);
				col += text_length(note->font," ");
			}
			else
			{
				col += text_length(note->font,temp_string);
				col += text_length(note->font," ");
			}
		}
		i++;

	}
	return -1;
}



//draws the formated text returns 1 if all
//text is written.
static int draw_note_text(int chapter, int mode)
{
	int num = chapter;
	int temp;
		
	row_begin = (480-NOTE_HEIGHT)/2;
	col_begin = (480-NOTE_WIDTH)/2;
	
	row = 0;
	col =0;


	max_rows = NOTE_HEIGHT/text_height(note->font)-1;
	row_add = text_height(note->font)+2;
		
	
	///// SCROLLING ////////
	if(note->type == NOTE_TYPE_SCROLLING)
	{
		more_down=0;
		more_up=0;
		
		
		while(row+1<max_rows+main_row && num<note->num_of_chapters-1)
		{
			
			if(num>-1)
				if(note->chapter[num-1].end_property==2){row++;col=0;}

			if(row>=max_rows+main_row || num>=note->num_of_chapters-1)
				break;
			
			if(note->chapter[num].begin_property==2){row++;col=0;}
			
			if(row>=max_rows+main_row || num>=note->num_of_chapters-1)
				break;
			
			if(note->chapter[num].allignment==1)
			{
				if(row >= main_row && row<=max_rows+main_row)
					print_note_text(note->chapter[num].text,NOTE_WIDTH+(480-NOTE_WIDTH)/2,(row-main_row)*row_add+row_begin,1);
				row++;
				col = 0;
			}
			else if(note->chapter[num].allignment==2)
			{
				if(row >= main_row && row<=max_rows+main_row)
					print_note_text(note->chapter[num].text,240,(row-main_row)*row_add+row_begin,2);
				row++;
				col = 0;
			}
			else
			{
				draw_normal_chapter(num,0);
			}


			num++;
		}

		if(num!=note->num_of_chapters-1)more_down =1;

		if(main_row>0)more_up=1;

		if(more_down)
			draw_arrow_down(240,NOTE_HEIGHT+(480-NOTE_HEIGHT)/2+30);
		if(more_up)
			draw_arrow_up(240,(480-NOTE_HEIGHT)/2-20);

	}
	/////// MULTIPLE PAGES ////////////
	else
	{
		more_left=0;
		more_right=0;
		
		main_row = 0;

		num = page_chapter[main_page];

		temp = -1;

		while(row+1<max_rows && num<note->num_of_chapters-1)
		{
			if(note->chapter[num].begin_property==1)
			{
				row += max_rows/2-1;
			}
			else if(note->chapter[num].begin_property==2 )
			{
				if(num != page_chapter[main_page] || page_chapter[main_page]==0)
				{
					row++;
					col=0;
				}
			}
			
			if(row>=max_rows || num>=note->num_of_chapters-1)
				break;
			
			if(note->chapter[num].allignment==1)
			{
				if(row >= main_row && row<=max_rows)
					print_note_text(note->chapter[num].text,NOTE_WIDTH+(480-NOTE_WIDTH)/2,(row)*row_add+row_begin,1);
				row++;
				col = 0;
			}
			else if(note->chapter[num].allignment==2)
			{
				if(row >= main_row && row<=max_rows)
					print_note_text(note->chapter[num].text,240,(row)*row_add+row_begin,2);
				row++;
				col = 0;
			}
			else
			{
				
				if(num == page_chapter[main_page])
					temp = draw_normal_chapter(num,page_word[main_page]);
				else
					temp = draw_normal_chapter(num,0);

				if(temp!=-1)
					page_word[main_page+1] = temp;
			}
			
		
			if(note->chapter[num].end_property==1)
			{
				num++;
				break;
			}
			else if(note->chapter[num].end_property==2)
			{
				row++;
				col=0;
			}

			num++;
		}
		
		if(temp == -1)
			page_chapter[main_page+1] = num;
		else
			page_chapter[main_page+1] = num-1;

		if(num!=note->num_of_chapters-1)more_right =1;
		if(main_page>0)more_left=1;


		if(more_right)
			draw_arrow_right(NOTE_WIDTH+(480-NOTE_WIDTH)/2+20,240);
		
		if(more_left)
			draw_arrow_left((480-NOTE_WIDTH)/2-20,240);
	}
	
	
	return 0;
}


void update_note_graphic(void)
{
	draw_lightsprite(virt,black_map,0,0);

	draw_note_text(0,0);
}





void update_note_logic(void)
{
	static int right_down=1;
	static int left_down=1;
	static int up_down=1;
	static int down_down=1;
	
	
	
	//key presses!
	if(!key[key_forward])up_down=0;
	if(key[key_forward] && !up_down)
	{
		if(more_up && note->type == NOTE_TYPE_SCROLLING)
		{
			play_fiend_sound("menu_move",0,0,0,0,200);
			main_row--;
		}
		
		up_down = 1;
	}
	
	if(!key[key_backward])down_down=0;
	if(key[key_backward] && !down_down)
	{
		if(more_down && note->type == NOTE_TYPE_SCROLLING)
		{
			play_fiend_sound("menu_move",0,0,0,0,200);
			main_row++;
		}
		
		down_down = 1;
	}

	if(!key[key_left])left_down=0;
	if(key[key_left] && !left_down)
	{
		if(note->type == NOTE_TYPE_PAGES && more_left)
		{
			play_fiend_sound("menu_note",0,0,0,0,200);
			main_page--;
		}
		
		left_down = 1;
	}
	
	if(!key[key_right])right_down=0;
	if(key[key_right] && !right_down)
	{
		if(note->type == NOTE_TYPE_PAGES && more_right)
		{
			play_fiend_sound("menu_note",0,0,0,0,200);
			main_page++;
		}
		
		right_down = 1;
	}
	
	if(!key[key_pickup])pickup_key_down=0;
	if(key[key_pickup] && !pickup_key_down)
	{
		play_fiend_sound("menu_back",0,0,0,0,200);

		inventory_is_on=1;
		fiend_note_is_on=0;
		clear_note_data();

		pickup_key_down=1;		
	}
	
}





//A test might need it later
/*if(!load_fiend_note(file))make_engine_error(file);

	clear(screen);
	textprintf(screen,note->font,0,0,makecol(255,255,255),"type: %d",note->type);
	for(i=0;i<note->num_of_chapters;i++)
	{
		textprintf(screen,font,0,i*30+15,makecol(255,255,255),"prop: %d allign: %d",note->chapter[i].begin_property,note->chapter[i].allignment);
		textprintf(screen,font,0,i*30+25,makecol(255,255,255),"%s",note->chapter[i].text);
		textprintf(screen,font,0,i*30+35,makecol(255,255,255),"end: %d",note->chapter[i].end_property);
	}
	clear_keybuf();
	readkey();*/