/*
 * 20000623
 *
 * log.c by Pierre Renaux
 *
 * Log file managment.
 *
 */

#include "allegro.h"
#include "stdio.h"
#include "string.h"

#define MAX_CHAR_STRING 512

static char log_file_name[MAX_CHAR_STRING] = "\0\0\0";
static int  first_log_open = 1;

/*

  Set the log file to use.

*/
void set_log_file(char *filename)
{
 if(filename)
  ustrncpy(log_file_name, filename, MAX_CHAR_STRING);
 else
  memset(log_file_name, 0, MAX_CHAR_STRING);
}

/*

  Write a message in the current log file.

*/
void log_event(char *msg, ...)
{
 FILE *file = NULL;
 va_list ap;
 char buf[8000];

 if(msg == NULL) 
  return;

 if(!log_file_name[0])
  return;

 if(first_log_open)
 {
  file = fopen(log_file_name, "w");
  first_log_open = 0;
 }
 else
 {
  file = fopen(log_file_name, "a");
 }

 if(!file)
  return;

 va_start(ap, msg);
 uvsprintf(buf, msg, ap);
 va_end(ap);

 fputs(buf, file);

 fclose(file);
}
