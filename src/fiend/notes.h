#include <allegro.h>


#ifndef NOTES_H
#define NOTES_H


#define NOTE_TYPE_SCROLLING 0
#define NOTE_TYPE_PAGES 1

#define MAX_CHAPTER_NUM 100


typedef struct
{
	int allignment; //0= left 1 = right 2 = center
	int begin_property; //0= nothing 1= center on screen 2 = new line
	int end_property; //0= nothing 1= new page 2 = new line
	
	char text[2000];
}NOTE_CHAPTER_DATA;


typedef struct
{
	FONT *font;
	int type; //0= scrolling 1= pages
	
	int num_of_chapters;
	NOTE_CHAPTER_DATA chapter[MAX_CHAPTER_NUM];
}NOTE_DATA;


extern int fiend_note_is_on;

void init_fiend_note(void);
void release_fiend_note(void);

void read_fiend_note(char *file);

void update_note_graphic(void);
void update_note_logic(void);

void clear_note_data(void);

#endif