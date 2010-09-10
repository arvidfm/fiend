#ifndef __CONSOLE_H__
#define __CONSOLE_H__
/*
 * 20000326
 *
 * console.h by Pierre Renaux
 *
 * Console functions.
 * 
 */

#include "allegro.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef U8
 #define U8 unsigned char
#endif

#ifndef MAX_CHAR_STRING
 #define MAX_CHAR_STRING 512
#endif

#define CSL_MSG_SIZE          1200
#define CSL_TEXT_SIZE       120000
#define CSL_PROMPT_MAX_SIZE     12
#define CSL_MAX_LAST_CMD        33
#define CSL_MAX_FUNC            64
#define CSL_MAX_VAR             64
#define CSL_MAX_ARG            128

#define CSL_DBUFF     csl_dbuff
#define CSL_DBUFF_W   csl_dbuff->w
#define CSL_DBUFF_H   csl_dbuff->h
#define CSL_DBUFF_BPP csl_dbuff->bpp

/* For the cdf argument of csl_update() */
#define CSL_DRAW_NONE   0  // Nothing special
#define CSL_DRAW_DOWN   1  // Move down the console y
#define CSL_DRAW_UP     2  // Move up the console y
#define CSL_DRAW_REDRAW 3  // Redraw the console

/* Cursor type */
#define CSL_CURSOR_DEFAULT  1
#define CSL_CURSOR_EMPTY    2
#define CSL_CURSOR_BITMAP   3

/* Console messagess */
#define CSLMSG_ERROR   0        /* Error has happen, this is just an internal
                                   message for you. The console don't check
                                   about it. */
#define CSLMSG_O_K     1        /* Ok, Default message  */
#define CSLMSG_CLOSE   2        /* Close the console    */
#define CSLMSG_QUIT    3        /* Quit the program     */

/* Console background type */
#define CSL_BKGD_NONE       0
#define CSL_BKGD_UNI        1
#define CSL_BKGD_BITMAP     2

void set_csl_font_color(int fg, int bg);
void set_draw_csl_dbuff(void (*draw_csl_dbuff)(BITMAP *bmp, int csl_y));
int  init_console(int width, void (*draw_csl_dbuff)(BITMAP *bmp, int csl_y));
void close_console(void);

int csl_set_bkgd_type(int bkgd_type, int color, BITMAP *bmp);
int csl_set_y(int cy);
int csl_get_y(void);
int csl_start(char *s, int q);
int csl_update(int (*func)(void), int cdf);
int csl_start_func(int savelast, char *s, ...);

void csl_textout(int newline, U8 *s);
void csl_textoutf(int newline, U8 *s, ...);
void csl_textschar(int newline, U8 schar);
void csl_set_draw_after_textout(int onoff);

int  csl_argc(void);
U8  *csl_argv(int arg);

void csl_set_schar(U8 schar, void (*func)(int cr, int cl, int y));

int  csl_add_func(U8 *cmd, int (*func)(void));
int  csl_remove_func(int (*func)(void));
void csl_remove_all_func(void);

int  csl_add_var(U8 *name, U8 *val);
int  csl_remove_var(U8 *name);
void csl_remove_all_var(void);
int  csl_set_var(U8 *name, U8 *val);
U8  *csl_get_var(U8 *name, U8 *def);

void csl_set_rect(int cl, int ct, int cr, int cb);
void csl_set_quick_quit_key(int scan_code);
void csl_set_prompt(U8 *prompt, U8 *iprompt);
void csl_set_cursor(int type, BITMAP *bmp);
void csl_set_move_speed(int spddown, int spdup);
void csl_set_prompt_text(char *s, ...);
void csl_set_always_redraw(int onoff);
void csl_set_show_command(int onoff);
int  csl_get_show_command(void);

void csl_push_cursor(void);
void csl_pop_cursor(void);

int csl_get_rect_l(void);
int csl_get_rect_r(void);
int csl_get_rect_t(void);
int csl_get_rect_b(void);
int csl_get_temp_y(void);

extern FONT    *csl_font;
extern PALETTE  csl_font_palette;
extern BITMAP  *csl_dbuff;
extern BITMAP  *csl_cursor_sprite;
extern int csl_started;

extern void (*csl_cb_before_draw_write)(void);
extern void (*csl_cb_after_draw_write)(void);
#define csl_cb_draw_own_frame  csl_cb_after_draw_write

/* END OF FILE **************************************************************/
#endif