/*
 * 19990407
 *
 * console.c by Pierre Renaux
 *
 * Console functions
 *
 */

#include "console.h"

#define MY_FREE(x)    if(x) { free(x); x = NULL; }
#define MY_DESTROY(type,x) if(x) { destroy_##type(x); x = NULL; }

void _add_exit_func(void (*func)(void));
void _remove_exit_func(void (*func)(void));

/*
  A special char
*/
typedef struct
{
 void (*func)(int cl, int cr, int y);
} SPECIAL_CHAR;
static SPECIAL_CHAR schar[256];

/*
  A console function
*/
typedef struct
{
 U8 cmd[MAX_CHAR_STRING];
 int (*func)(void);
} FUNC;

static int csl_func_quit_console(void);
static int csl_func_clear_console(void);
static int csl_func_echo(void);
static int csl_func_list_funcs(void);
static int csl_func_list_vars(void);
static int csl_func_set(void);

// Default console functions
static FUNC funcs[CSL_MAX_FUNC] =
{
 { "quit_console",  csl_func_quit_console },
 { "clear_console", csl_func_clear_console },
 { "echo",          csl_func_echo },
 { "list_funcs",    csl_func_list_funcs },
 { "list_vars",     csl_func_list_vars },
 { "set",           csl_func_set },
};

/*
  Console variable
*/
typedef struct
{
 U8 *name;
 U8 *val;
} VAR;
static VAR vars[CSL_MAX_VAR];

/*============================================================================

   INTERNAL FUNCTIONS

============================================================================*/
static void draw_console(int cdf);
static void draw_prompt(void);
static void draw_csl_text(void);
static int process_csl_msg(int savelast);
static void c_get_argc(U8 *str, FUNC *c_func);
static void c_get_argv(U8 *str, FUNC *c_func);
static void c_set_clip(void);
static void c_line_up(void);
static void c_line_down(void);
static int create_default_cursor(void);
static void save_last_command(U8 *cmd);
static U8 *get_last_command(int godown);
#define CC_SIZE  14 /* Color Code size */
#define RCC_FG   0
#define RCC_BG   1
static void read_color_code(int mode);

/*============================================================================

   INTERNAL MACROS AND DEFINES

============================================================================*/
#define CT_NEW_LINE         20
#define CT_SPECIAL_CHAR    21
#define CT_FGCOLOR_CODE    '\f'
#define CT_BGCOLOR_CODE    '\b'

#define ZERO_CSL_MSG()  if(csl_msg) memset(csl_msg, 0, CSL_MSG_SIZE); chars = 0
#define ZERO_TEMP_STR() if(temp_str) memset(temp_str, 0, CSL_MSG_SIZE)
#define ZERO_CSL_TEXT() if(csl_text) memset(csl_text, 0, CSL_TEXT_SIZE)

#define CBMP_W (csl_dbuff->w)
#define CBMP_H (csl_dbuff->h)

/*============================================================================

   INTERNAL DATA

============================================================================*/
static void (*draw_csl_dbuff)(BITMAP *csl_dbuff, int temp_y) = NULL;

int csl_started = 0;

static int csl_font_color_fg = 0;
static int csl_font_color_bg = 0;

/* Char qui represente le prompte */
static U8 csl_prompt[CSL_PROMPT_MAX_SIZE];
/* Char qui represente le prompte inverse */
static U8 csl_i_prompt[CSL_PROMPT_MAX_SIZE];
/* Texte de la console */
static U8 csl_text[CSL_TEXT_SIZE];
/* Ce qui est ecrit pour le moment dans la console */
static U8 csl_msg[CSL_MSG_SIZE];
/* String temporaire de travail */
static U8 temp_str[CSL_MSG_SIZE];
/* Derniere commande validee */
static U8 *last_cmd[CSL_MAX_LAST_CMD];
static int current_last_cmd = 0;
/* Debut du texte de la console a affiche */
static U8 *csl_text_start = NULL;
/* Double buffer d'affichage de la console */
BITMAP *csl_dbuff = NULL;
/* Fonte d'ecriture de la console */
FONT *csl_font = NULL;
#define CSL_FONT csl_font
/* Palette de la Fonte d'ecriture de la console */
PALETTE csl_font_palette;
/* Est-il possible de lancer la console ? */
static int  csl_possible = 0;
/* Traiter les messages de la console ? */
static int csl_process_msg_flag = 0;
/* Fin de l'execution de la console ? */
static int csl_done = 0;
/* La console est-elle initialisee ? */
static int csl_initialised = 0;
/* Touche d'arret rapide de la console */
static U8 csl_quick_quit_key = KEY_TILDE;
/* Nombre de char que l'on peut ecrire sur la longeur de la console */
static int csl_num_char_w = 0;
/* Nombre de ligne que l'on peut voire dans la console */
static int csl_num_char_h = 0;
/* Frontiere de dessin de la console */
static int c_cb = 80;
static int c_cr = 0;
static int c_cl = 320;
static int c_ct = 0;
/* On doit quitter le programme ? */
static int cm_quit_prog = 0;
/* Type de fond de la console */
static int  csl_bkgd_type = CSL_BKGD_UNI;
/* Couleur de fond de la console dans le cas d'un fond uni */
static int csl_bkgd_color = 0;
/* Bitmap de fond de la console dans le cas d'un fond en bitmap */
static BITMAP *csl_bkgd_bitmap = NULL;
/* Coordonnee y jusqu'a laquelle descand la console */
static int csl_y = 80;
/* Fonction console draw de l'utilisateur qui sert a rajoute un petit incone
   ou des trucs dans le genre */
void (*csl_cb_before_draw_write)(void) = NULL;
void (*csl_cb_after_draw_write)(void) = NULL;
/* Sprite du curseur */
BITMAP *csl_cursor_sprite = NULL;
/* Sprite du curseur par defaut */
static BITMAP *default_cursor_sprite = NULL;
/* Variables Y temporaire */
static int temp_y = 0;
/* Flag de dessin de la console */
int cdf = 1;
/* Vitesse de descante de la console */
static int csl_move_speed_down = 1;
/* Vitesse de montee de la console */
static int csl_move_speed_up = 1;
/* nbr de char tape */
static int chars = 0;
/* Coo Y de la descante de la console */
static int csl_y_coo = 0;
/* Plus grande ligne */
static int csl_longest_line = 0;
/* Valeur de decalage */
static int csl_text_decalage = 0;
/* Dessiner la console ? */
static int draw = 0;
/* Bitmap qui contient l'arriere du prompte */
static BITMAP *prompt_bkgd = NULL;
/* Doit-on reprendre l'arriere du prompte de la console */
static int get_prompt_bkgd = 1;
/* process_msg() doit-il ecrire dans le prompt la commande executee apres l'execution
  de celle-ci ?*/
static int write_prompt = 1;
/* On redessine a chaque fois (utile si on a des objets anime dans la console) */
int c_always_redraw = 0;
/* Quand on dessine le fond stretch a la taille du rectangle definit
   par csl_set_rect() */
static int stretch_to_rect = 1;
/* Doit-on redessiner la console apres un appel a csl_textout si elle est
   affichee ? */
static int draw_after_textout = 0;
/* Argc pour les fonctions de la console */
static int i_argc = 0;
/* Argv pour les fonctions des la console */
static U8 i_argv[CSL_MAX_ARG][MAX_CHAR_STRING];

static BITMAP *pushed_csl_cursor = NULL;

/*============================================================================

   INTERNAL FUNCTIONS

============================================================================*/
static int font_w(FONT *font, int dummy)
{
 char tmp[2] = { 'W', '\0' };
 return text_length(font, tmp);
}

//---------------------------------------------------------------------------
// Name: zero_state()
// Desc: Zeros the console status
//---------------------------------------------------------------------------
static void zero_state(void)
{
 csl_done = 0;
 csl_process_msg_flag = 0;
 cm_quit_prog = 0;
 temp_y = 0;
 cdf = 1;
 chars = 0;

 ZERO_CSL_MSG();

 draw = 1;
}

/*

  Initialise les buffers de la console

*/
static int init_csl_buffers(int width)
{
 MY_DESTROY(bitmap, csl_dbuff);

 csl_dbuff = create_bitmap(width, csl_y);
 if(!csl_dbuff)
 {
  sprintf(allegro_error, "init_csl_buffers() - Unable to create csl_dbuff");
  return 0;
 }
 clear(csl_dbuff);

 csl_set_rect(0, 0, CSL_DBUFF_W, CSL_DBUFF_H);
 csl_set_cursor(CSL_CURSOR_DEFAULT, NULL);

 draw = 1;

 return 1;
}

/*

  Affiche la console

*/
static void draw_console(int godown)
{
 int pcl, pcr, pct, pcb;

 if(godown == CSL_DRAW_DOWN)
 {
  if(temp_y < csl_y)
  {
   if((temp_y + csl_move_speed_down) > csl_y)
    temp_y = csl_y;
   else
    temp_y += csl_move_speed_down;
   draw = 1;
  }
 }
 else if(godown == CSL_DRAW_UP)
 {
  if(temp_y >= 0)
  {
   if((temp_y - csl_move_speed_up) < 0)
    temp_y = 0;
   else
    temp_y -= csl_move_speed_up;

   draw = 1;
  }
 }

 if(draw || c_always_redraw)
 {
  if((csl_bkgd_type == CSL_BKGD_BITMAP) && (csl_bkgd_bitmap))
  {
   blit(csl_bkgd_bitmap, CSL_DBUFF, 0, 0,
        (CSL_DBUFF->w/2)-(csl_bkgd_bitmap->w/2),
        (CSL_DBUFF->h/2)-(csl_bkgd_bitmap->h/2),
        csl_bkgd_bitmap->w, csl_bkgd_bitmap->h);
        
  }
  else if(csl_bkgd_type == CSL_BKGD_UNI)
  { 
   clear_to_color(CSL_DBUFF, csl_bkgd_color);
  }
  else
  {
   clear_to_color(CSL_DBUFF, CSL_DBUFF->vtable->mask_color);
  }

  if(get_prompt_bkgd)
  {
   if(prompt_bkgd)
    destroy_bitmap(prompt_bkgd);

   prompt_bkgd = create_bitmap(c_cr-c_cl+1, text_height(CSL_FONT)+1);

   clear_to_color(prompt_bkgd, csl_bkgd_color);

   blit(CSL_DBUFF, prompt_bkgd, c_cl, c_cb-prompt_bkgd->h-1, 0, 0,
        prompt_bkgd->w, prompt_bkgd->h);

   get_prompt_bkgd = 0;
  }
 }

 if(csl_cb_before_draw_write)
   csl_cb_before_draw_write();

 // push clip state
 pcl = CSL_DBUFF->cl;
 pcr = CSL_DBUFF->cr;
 pct = CSL_DBUFF->ct;
 pcb = CSL_DBUFF->cb;

 c_set_clip();

 csl_num_char_w = (unsigned long)((CSL_DBUFF->cr-CSL_DBUFF->cl)/font_w(CSL_FONT, -1))-2;
 csl_num_char_h = csl_y/text_height(CSL_FONT);

 if(csl_num_char_w > CSL_MSG_SIZE)
  csl_num_char_w = CSL_MSG_SIZE;

 draw_prompt();

 if(draw || c_always_redraw)
  draw_csl_text();

 // pop_clip_state
 CSL_DBUFF->cl = pcl;
 CSL_DBUFF->cr = pcr;
 CSL_DBUFF->ct = pct;
 CSL_DBUFF->cb = pcb;

 if(csl_cb_after_draw_write)
  csl_cb_after_draw_write();

 if(draw_csl_dbuff)
  draw_csl_dbuff(CSL_DBUFF, temp_y);

 csl_y_coo = temp_y;
}

/*

  Dessine le prompt

*/
static void draw_prompt(void)
{
 int i, cf = 0, x = 0;
 int j;

 ZERO_TEMP_STR();

 j = chars - csl_num_char_w;
 if(j < 0)
  j = 0;
 else
  cf = 1;

 for(i = 0; i < csl_num_char_w+1; i++)
 {
  if((i+j) < CSL_MSG_SIZE)
   usetc(temp_str+i, ugetc(csl_msg+i+j));
 }

 if(prompt_bkgd)
 {
  blit(prompt_bkgd, CSL_DBUFF, 0, 0,
       CSL_DBUFF->cl, CSL_DBUFF->cb-prompt_bkgd->h-1,
       prompt_bkgd->w, prompt_bkgd->h);
 }
 else
 {
  rectfill(CSL_DBUFF,
           CSL_DBUFF->cl, CSL_DBUFF->cb-text_height(CSL_FONT)-1,
           CSL_DBUFF->cr, CSL_DBUFF->cb+1,
           csl_bkgd_color);
 }

 if(!cf || (chars <= csl_num_char_w))
 {
  textprintf_ex(CSL_DBUFF, CSL_FONT,
             CSL_DBUFF->cl, CSL_DBUFF->cb-text_height(CSL_FONT)-1,
             csl_font_color_fg, -1,
             "%s%s", csl_prompt, temp_str);

  x = text_length(CSL_FONT, temp_str) + text_length(CSL_FONT, csl_prompt);
 }
 else
 {
  textprintf_ex(CSL_DBUFF, CSL_FONT,
             CSL_DBUFF->cl, CSL_DBUFF->cb-text_height(CSL_FONT)-1,
             csl_font_color_fg, -1,
             "%s%s", csl_i_prompt, temp_str);
  x = text_length(CSL_FONT, temp_str) +
      text_length(CSL_FONT, csl_i_prompt);
 }

 if(csl_cursor_sprite)
 {
  if(chars > csl_num_char_w)
  {
   draw_sprite(CSL_DBUFF, csl_cursor_sprite,
               ((csl_num_char_w+1) * font_w(CSL_FONT, -1)) + CSL_DBUFF->cl,
               CSL_DBUFF->cb - csl_cursor_sprite->h - 1);
  }
  else
  {
   draw_sprite(CSL_DBUFF, csl_cursor_sprite,
               ((chars+1) * font_w(CSL_FONT, -1)) + CSL_DBUFF->cl,
               CSL_DBUFF->cb - csl_cursor_sprite->h - 1);
  }
 }

 ZERO_TEMP_STR();
}

/*

  Dessine le texte de la console

*/
static void draw_csl_text(void)
{
 int x = CSL_DBUFF->cl, y = CSL_DBUFF->cb - (text_height(CSL_FONT)*2) - 2; 
 int cl = CSL_DBUFF->cl, cr = CSL_DBUFF->cr;
 int first = 1;
 int c = 0, k = 0;
 int oups = 0;
 U8 *old_csl_text_start = csl_text_start;
 U8 tmp[2];

 if(!csl_text_start)
  return;

 do
 {
  if(ugetc(csl_text_start) == CT_NEW_LINE)
  {
   first = 1;
   y -= text_height(CSL_FONT);
   cl = x = CSL_DBUFF->cl;
   cr = CSL_DBUFF->cr; 
  }
  else if(ugetc(csl_text_start) == CT_SPECIAL_CHAR)
  {
   cl = CSL_DBUFF->cl;
   cr = CSL_DBUFF->cr;

   csl_text_start += uwidth(csl_text_start);
   for(k = 0; k < csl_text_decalage; k++)
   {
    cl -= font_w(CSL_FONT, -1);
    cr -= font_w(CSL_FONT, -1);
   }

   c = ugetc(csl_text_start) & 255;
   if(schar[c].func)
    schar[c].func(cl, cr, y);
  }
  else if(ugetc(csl_text_start) == CT_FGCOLOR_CODE)
  {
   read_color_code(RCC_FG);
  }
  else if(ugetc(csl_text_start) == CT_BGCOLOR_CODE)
  {
   read_color_code(RCC_BG);
  }
  else
  {
   cl = CSL_DBUFF->cl;
   cr = CSL_DBUFF->cr;

   if(first)
   {
    csl_text_start += uwidth(csl_text_start);
    for(k = 0; k < csl_text_decalage; k++)
    {
     if((ugetc(csl_text_start)) && (ugetc(csl_text_start) != '\0') &&
        (ugetc(csl_text_start) != CT_NEW_LINE))
     {
      if(ugetc(csl_text_start) == CT_FGCOLOR_CODE)
      {
       read_color_code(RCC_FG);
       csl_text_start += uwidth(csl_text_start);
       x -= font_w(CSL_FONT, -1);
      }
      else if(ugetc(csl_text_start) == CT_BGCOLOR_CODE)
      {
       read_color_code(RCC_BG);
       csl_text_start += uwidth(csl_text_start);
       x -= font_w(CSL_FONT, -1);
      }
      else
       csl_text_start += uwidth(csl_text_start);
     }
     else
     {
      oups = 1;
     }

     cl -= font_w(CSL_FONT, -1);
     cr -= font_w(CSL_FONT, -1);
    }

    csl_text_start -= uwidth(csl_text_start);
   }


   if(!oups)
   {
    tmp[0] = ugetc(csl_text_start);
    tmp[1] = '\0';
    textout_ex(CSL_DBUFF, CSL_FONT, tmp, x, y, csl_font_color_fg, -1);
   }

   tmp[0] = 'W';
   tmp[1] = '\0';
   x += text_length(CSL_FONT, tmp);
   first = 0;
  }

  oups = 0;
 } while(ugetx((char**)&csl_text_start) && (ugetc(csl_text_start) != '\0')
         && (y+text_height(CSL_FONT) > 0));

 csl_text_start = old_csl_text_start;
}

/*

  Traite les message envoye a la console

*/
static int process_csl_msg(int savelast)
{
 int i = 0, d = 0;
 U8 *p1 = csl_msg, *p;
 U8 *c;
 int  nc = 0, np = 0, uw = 0;
 U8 tmp[8000];
 int ret = CSLMSG_O_K;
 int tc = 0;

 if(ugetc(csl_msg))
  tc = ugetc(csl_msg);

 if((!tc) || (tc == '\0'))
 {
  csl_textoutf(1, "%s", csl_prompt);
  goto end;
 }

 ustrcpy(tmp, csl_msg);

 /* Fonctions definie par l'utilisateur */
 for(i = 0; i < CSL_MAX_FUNC; i++)
 {
  nc = np = 0;
  p1 = p = tmp;
  if(funcs[i].func)
  {
   c = funcs[i].cmd;

   /* On passe les espaces devant */
   while((p1) && (ugetc(p1) == ' ') && (ugetc(p1) != '\0'))
   {
    ugetx((char**)&p1);
    ugetx((char**)&p);
   }

   while((p) && (ugetc(p) != ' ') && (ugetc(p) != '\0'))
   {
    uw = uwidth(p);
    np += uw;
    p += uw;
   }

   while((c) && (ugetc(c) != ' ') && (ugetc(c) != '\0'))
   {
    uw = uwidth(c);
    nc += uw;
    c += uw;
   }

   if(np > nc)
   {
    int tmp;
    tmp = np;
    np = nc;
    nc = tmp;
   }

   if(ustrncmp((U8*)p1, (U8*)funcs[i].cmd, nc) == 0)
   {
    c_get_argc(tmp, &funcs[i]);
    c_get_argv(tmp, &funcs[i]);

    if(write_prompt)
     csl_textoutf(1, "%s%s", csl_prompt, p1);

    d = draw; draw = 1;
    draw_console(CSL_DRAW_REDRAW);
    draw = d; d = 0;
    ret = funcs[i].func();
    goto end;
   }
  }
 }

 while((p1) && uisspace(ugetc((U8*)p1)))
  ugetx((char**)&p1);

 if(write_prompt)
 {
  csl_textoutf(1, "%s%s", csl_prompt, p1);
  csl_textout(1, "Bad Command !");
 }

end:;
 if(savelast)
  save_last_command(tmp);
 csl_process_msg_flag = 0;
 ZERO_CSL_MSG();

 return ret;
}

/*

  Retourne le nombre d'argument de la ligne de commande

*/
static void c_get_argc(U8 *str, FUNC *c_func)
{
 i_argc = 0;

 str += ustrsize((U8*)c_func->cmd);
 str -= uwidth(str);

 while(ugetx((char**)&str))
 {
  if(ugetc(str) == ' ')
  {
   if(ugetc(str-1) != ' ')
    i_argc += 1;
  }
 }

 if(ugetc(str-2) == ' ')
  i_argc -= 1;

 i_argc = MIN(i_argc, CSL_MAX_ARG);
}

/*

  Capture les arguments de la ligne de commande

*/
static void c_get_argv(U8 *str, FUNC *c_func)
{
 int j = 0, k = 0, c = 0;

 memset(i_argv, 0, CSL_MAX_ARG*MAX_CHAR_STRING);

 do
 {
  if(ugetc(str) == ' ')
  {
   if(ugetc(str-1) != ' ')
   {
    j++;
    k = 0;
   }
  }

  c = ugetc(str);
  if(c != ' ')
  {
   usetc(i_argv[j]+k, c);
   k++;
  }

  if(j >= CSL_MAX_ARG)
   return;

 } while(ugetx((char**)&str));
}

/*

  Sauvegarde la dernier commande

*/
static void save_last_command(U8 *cmd)
{
 int i = 0;

 for(i = 0; i < CSL_MAX_LAST_CMD; i++)
 {
  if(last_cmd[i])
   if(ustrcmp(last_cmd[i], cmd) == 0)
    return;
 }

 for(i = 0; i < CSL_MAX_LAST_CMD; i++)
 {
  if(!last_cmd[i])
   break;
 }
 if(i >= CSL_MAX_LAST_CMD-1)
 {
  MY_FREE(last_cmd[0]);
  for(i = 1; i < CSL_MAX_LAST_CMD; i++)
   last_cmd[i-1] = last_cmd[i];
  for(i = 0; i < CSL_MAX_LAST_CMD; i++)
  {
   if(!last_cmd[i])
       break;
  }
 }

 MY_FREE(last_cmd[i]);

 if((cmd) && ugetc(cmd))
 {
  last_cmd[i] = (U8*)malloc(ustrsizez(cmd));
  if(last_cmd[i])
   ustrcpy(last_cmd[i], cmd);
  current_last_cmd = i+1;
 }
}

/*

  Retourne la derniere commande

*/
static U8 *get_last_command(int godown)
{
 if(godown)
 {
  current_last_cmd--;
  if(current_last_cmd < 0)
   current_last_cmd = CSL_MAX_LAST_CMD-1;

  if(!last_cmd[current_last_cmd])
  {
   current_last_cmd = CSL_MAX_LAST_CMD-1;
   while(!last_cmd[current_last_cmd])
   {
    current_last_cmd--;
    if(current_last_cmd < 0)
    {
     current_last_cmd = 0;
     return NULL;
    }
   }
  }
 }
 else
 {
  current_last_cmd++;
  if(current_last_cmd >= CSL_MAX_LAST_CMD)
   current_last_cmd = 0;

  if(!last_cmd[current_last_cmd])
   current_last_cmd = 0;
 }

 return last_cmd[current_last_cmd];
}

/*

  Lit un code couleur et effectue l'operation necessaire

*/
void read_color_code(int mode)
{
 int r = -1, g = -1, b = -1;

 if(mode == RCC_FG)
 {
  csl_text_start += uwidth(csl_text_start)*2;
  r = atoi(csl_text_start);
  csl_text_start += uwidth(csl_text_start)*4;
  g = atoi(csl_text_start);
  csl_text_start += uwidth(csl_text_start)*4;
  b = atoi(csl_text_start);
  csl_text_start += uwidth(csl_text_start)*3;
  if(r < 0 || g < 0 || b < 0)
  {
   csl_font_color_fg = -1;
  }
  else
  {
   set_csl_font_color(makecol(r, g, b), csl_font_color_bg);
  }
 }
 else if(mode == RCC_BG)
 {
  csl_text_start += uwidth(csl_text_start)*2;
  r = atoi(csl_text_start);
  csl_text_start += uwidth(csl_text_start)*4;
  g = atoi(csl_text_start);
  csl_text_start += uwidth(csl_text_start)*4;
  b = atoi(csl_text_start);
  csl_text_start += uwidth(csl_text_start)*3;
  if(r < 0 || g < 0 || b < 0)
  {
   csl_font_color_bg = -1;
  }
  else
  {
   set_csl_font_color(makecol(r,g,b), csl_font_color_bg);
  }
 }
}

/*============================================================================

   IMPLEMENTATION DES FONCTIONS GLOBALES

============================================================================*/
/*

  Selectionne les couleurs de dessin de la fonte

*/
void set_csl_font_color(int fg, int bg)
{
 csl_font_color_fg = fg;
 csl_font_color_bg = bg;
 //text_mode(csl_font_color_bg);
}

/*

  Change la fonction draw_csl_dbuff

*/
void set_draw_csl_dbuff(void (*_draw_csl_dbuff)(BITMAP *bmp, int csl_y))
{
 draw_csl_dbuff = _draw_csl_dbuff;
}

/*

  Initialise la console (a faire a chaque chagement de BPP)

*/
int init_console(int width, void (*_draw_csl_dbuff)(BITMAP *bmp, int csl_y))
{
 if(csl_initialised)
  close_console();

 draw_csl_dbuff = _draw_csl_dbuff;
 if(!draw_csl_dbuff)
 {
  sprintf(allegro_error, "init_console() - You have to pass a valid draw_csl_dbuff function !");
  return 0;
 }

 if(!csl_font)
  csl_font = font;

 if(!init_csl_buffers(width))
 {
  sprintf(allegro_error, "init_console() - Unable to alloc console buffers !");
  return 0;
 }

 if(!create_default_cursor())
 {
  sprintf(allegro_error, "init_console() - Can't create default cursor !");
  return 0;
 }

 csl_text_start = csl_text;
 csl_bkgd_color = makecol(0,0,0);
 csl_possible = 1;

 csl_set_prompt(">","<");
 csl_set_cursor(CSL_CURSOR_DEFAULT, NULL);

 csl_initialised = 1;
 _add_exit_func(close_console);

// log_event("* Console Initialised !\n");

 return 1;
}

/*

  Libere la memoire allouee pour la console

*/
void close_console(void)
{
 if(!csl_initialised)
  return;

 MY_DESTROY(bitmap, csl_dbuff);
 MY_DESTROY(bitmap, prompt_bkgd);

 draw = 0;
 csl_initialised = 0;
 csl_possible = 0;

 _remove_exit_func(close_console);
// log_event("* Console Closed !\n");
}

/*

  Change les attributs de la console

*/
int csl_set_bkgd_type(int bkgd_type, int color, BITMAP *bmp)
{
 if(bkgd_type != -1)
  csl_bkgd_type = bkgd_type;

 csl_bkgd_color = color;
 csl_bkgd_bitmap = bmp;

 draw = 1;
 get_prompt_bkgd = 1;

 return 1;
}

/*

  Change la coo y jusqu'a laquelle la console descend

*/
int csl_set_y(int cy)
{
 csl_y = cy;
 if(!init_csl_buffers(CSL_DBUFF_W))
  return 0;

 zero_state();
 return 1;
}

/*

  Retourne csl_y

*/
int csl_get_y(void)
{
 return csl_y;
}

/*

  Met a jour la console

*/
int csl_update(int (*func)(void), int cdf)
{
 int ret = CSLMSG_O_K;

 if(func)
  ret = func();

 if(cdf == CSL_DRAW_REDRAW)
 {
  draw = 1;
  cdf = CSL_DRAW_DOWN;
 }
 draw_console(cdf);

 return ret;
}

/*

  Demarre un fonction de la console

*/
int csl_start_func(int savelast, char *s, ...)
{
 char str[8000];
 int ret = CSLMSG_ERROR;
 va_list ap;

 if(!s)
  return ret;

 /* Recuperation du texte formate */
 memset(str,'\0',8000);
 va_start(ap, s);
 uvsprintf(str, s, ap);
 va_end(ap);

 /* Execution de la fonction */
 csl_set_prompt_text(str);
 ret = process_csl_msg(savelast);
 switch(ret)
 {
  case CSLMSG_CLOSE:
  case CSLMSG_QUIT:
       csl_done = 1;
  default:
       break;
 }

 return ret;
}

/*

  Gere les entrees

*/
int process_csl_input(void)
{
 int ret = CSLMSG_O_K;
 int scan = 0;
 int ascii = 0;
 U8 *lcmd = NULL;

 if(!keypressed())
  return ret;

 ascii = ureadkey(&scan);

 if(scan == csl_quick_quit_key)
 {
  if(cdf == 1)
  {
   csl_done = 1;
  }
  else if(cdf == 2)
  {
   csl_done = 0;
   cdf = 1;
  }
 }
 else
 {
  if(scan == KEY_UP && ((key_shifts & KB_CTRL_FLAG) || (key_shifts & KB_SHIFT_FLAG)))
  {
   c_line_up();
   draw = 1;
  }
  else if(scan == KEY_DOWN && ((key_shifts & KB_CTRL_FLAG) || (key_shifts & KB_SHIFT_FLAG)))
  {
   c_line_down();
   draw = 1;
  }
  else if(scan == KEY_LEFT && ((key_shifts & KB_CTRL_FLAG) || (key_shifts & KB_SHIFT_FLAG)))
  {
   if(csl_text_decalage > 0)
   {
    csl_text_decalage--;
    draw = 1;
   }
  }
  else if(scan == KEY_RIGHT && ((key_shifts & KB_CTRL_FLAG) || (key_shifts & KB_SHIFT_FLAG)))
  {
   if(csl_longest_line > csl_text_decalage)
   {
    csl_text_decalage++;
    draw = 1;
   }
  }
  else if(scan == KEY_HOME && ((key_shifts & KB_CTRL_FLAG) || (key_shifts & KB_SHIFT_FLAG)))
  {
   while(ugetx((char**)&csl_text_start));
   csl_text_start -= uwidth(csl_text_start);
   draw = 1;
  }
  else if(scan == KEY_END && ((key_shifts & KB_CTRL_FLAG) || (key_shifts & KB_SHIFT_FLAG)))
  {
   csl_text_start = csl_text;
   draw = 1;
  }
  else
  {
   if(scan == KEY_UP)
   {
    lcmd = get_last_command(1);
    if(lcmd)
     csl_set_prompt_text((U8*)lcmd);
   }
   else if(scan == KEY_DOWN)
   {
    lcmd = get_last_command(0);
    if(lcmd)
     csl_set_prompt_text((U8*)lcmd);
   }
   else if(scan == KEY_LEFT)
   {
    if((chars-1) >= 0)
     chars--;
   }
   else if(scan == KEY_RIGHT)
   {
    if((chars + 1) <= ustrlen(csl_msg))
     chars++;
   }
   else if(scan == KEY_HOME)
   {
    chars = 0;
   }
   else if(scan == KEY_END)
   {
    chars = ustrlen(csl_msg);
   }
  }
  if(scan == KEY_ENTER)
  {
   csl_process_msg_flag = 1;
   chars = 0;
   draw = 1;
  }
  else if(scan == KEY_BACKSPACE)
  {
   if((chars-1) >= 0)
   {
    chars--;
    uremove((U8*)csl_msg, chars);
   }
  }
  else if(scan == KEY_DEL && ((key_shifts & KB_SHIFT_FLAG) || (key_shifts & KB_CTRL_FLAG)))
  {
   ZERO_CSL_MSG();
  }
  else if(scan == KEY_DEL)
  {
   if(ugetc(csl_msg+chars))
   {
    uremove(csl_msg, chars);
   }
  }
  else if(scan == KEY_SPACE)
  {
   if((chars + 1) <= CSL_MSG_SIZE)
   {
    uinsert((U8*)csl_msg, chars, ' ');
    chars++;
   }
  }
  else if(ascii > 32 && ascii < 256)
  {
   if((chars + 1) <= CSL_MSG_SIZE)
   {
    uinsert((U8*)csl_msg, chars, ascii);
    chars++;
   }
  }
 }

 return ret;
}

/*

  Demarre la console

*/
int csl_start(char *start_cmd, int q)
{
 int ret = CSLMSG_O_K;

 /* Initialisation des variables */
 zero_state();

 if(!csl_possible)
  return CSLMSG_O_K;

 csl_started = 1;

 /* Capture de se qui se trouve derriere la console */
 clear_keybuf();

 csl_start_func(0, start_cmd);

 if(q) goto end;

 while(((!csl_done) && (!cm_quit_prog)) || temp_y != 0)
 {
  if(csl_done || cm_quit_prog)
   cdf = 2;

  /* Traitement des messages */
  if(csl_process_msg_flag)
  {
   switch(process_csl_msg(1))
   {
    case CSLMSG_CLOSE:
         csl_done = 1;
         break;

    case CSLMSG_QUIT:
         ret = CSLMSG_QUIT;
         goto end;

    default: break;
   }
  }

  /* Mise a jour */
  switch(csl_update(process_csl_input, cdf))
  { 
   case CSLMSG_CLOSE:
        csl_done = 1;
        break;

   case CSLMSG_QUIT:
        ret = CSLMSG_QUIT;
        goto end;

   default: break;
  }
  draw = 0;
 }

end:;
 csl_done = 1;
 csl_started = 0;
 clear_keybuf();
 return ret;
}

/*

  Change la touche de fermeture rapide de la console

*/
void csl_set_quick_quit_key(int scan_code)
{
 csl_quick_quit_key = scan_code;
}

/*

  Ecrit dans la console

*/
void csl_textout(int newline, U8 *str)
{
 int i = 0, j;
 int ln = 0;
 int anc = 0;
 U8 str2[8000];
 int cc = 0;
 char *begin = NULL;
 U8 *temp = (U8*)malloc(CSL_TEXT_SIZE+1);

 if(!temp)
  return;

 if((!ugetc(str)) || (ugetc(str) == '\0'))
  goto end;

 ln = 0;
 begin = str;
 do
 {
  if(ugetc(str) == CT_FGCOLOR_CODE || ugetc(str) == CT_BGCOLOR_CODE)
   cc++;
  ln++;
 } while((ugetx((char**)&str)) && (ugetc(str) != '\0'));
 str = begin;

 if(ln <= 0)
  goto end;

 if((ln-(cc*CC_SIZE)) > csl_longest_line)
  csl_longest_line = ln-(cc*CC_SIZE);

 ustrcpy(temp, csl_text);
 memset(str2, 0, 8000);
 ZERO_CSL_TEXT();

 for(i = 0; i < ln; i++)
 {
  usetc(str2+i, ugetc(str+anc));
  anc++;
 }

 for(i = 0; i < ln; i++)
  usetc(csl_text+i, ugetc(str2+i));

 for(j = 0; j < newline; j++)
 {
  usetc(csl_text+i, CT_NEW_LINE);
  i++;
 }

 /* Copie de l'ancien contenu */
 for(j = 0; i < CSL_TEXT_SIZE; i++, j++)
  usetc(csl_text+i, ugetc(temp+j));

end:;
 csl_text_start = csl_text;
 MY_FREE(temp);
 if(csl_started)
 {
  draw = 1;
  if(draw_after_textout)
   csl_update(NULL, cdf);
 }
}

void csl_textoutf(int newline, U8 *s, ...)
{
 char str[8000];
 va_list ap;

 if(!s) return;

 va_start(ap, s);
 uvsprintf(str, s, ap);
 va_end(ap);

 csl_textout(newline, str);
}

/*

  Ecrit un char special dans la console

*/
void csl_textschar(int newline, U8 special_char)
{
 U8 tmp[6];

 memset(tmp, 0, 6);
 usetc(tmp, CT_SPECIAL_CHAR);
 usetc(tmp+uwidth(tmp), special_char);

 csl_textout(newline, tmp);
}

/*

  Ajoute une fonction

*/
int csl_add_func(U8 *cmd, int (*func)(void))
{
 int i;
 for(i = 0; i < CSL_MAX_FUNC; i++)
 {
  if(!funcs[i].func)
  {
   ustrncpy(funcs[i].cmd, cmd, MAX_CHAR_STRING);
   funcs[i].func = func;
   return i;
  }
 }

 return -1;
}

/*

  Enleve une fonction

*/
int csl_remove_func(int (*func)(void))
{
 int i;

 for(i = 0; i < CSL_MAX_FUNC; i++)
 {
  if(funcs[i].func == func)
  {
   memset(funcs[i].cmd, 0, MAX_CHAR_STRING);
   funcs[i].func = NULL;
   return 1;
  }
 }

 return 0;
}

/*

  Enleve toute les fonctions

*/
void csl_remove_all_func(void)
{
 int i;

 if(!funcs)
  return;

 for(i = 0; i < CSL_MAX_FUNC; i++)
 {
  memset(funcs[i].cmd, 0, MAX_CHAR_STRING);
  funcs[i].func = NULL;
 }
}

/*

  Retourne l'index de la variable passee

*/
int get_var_i(U8 *name)
{
 int i;

 for(i = 0; i < CSL_MAX_VAR; i++)
 {
  if(vars[i].name)
  {
   if(ustrncmp(vars[i].name, name, ustrsizez(name)) == 0)
    return i;
  } 
 }

 return -1;
}

/*

  Ajoute une variable

*/
int csl_add_var(U8 *name, U8 *val)
{
 int i;

 for(i = 0; i < CSL_MAX_VAR; i++)
 {
  if(!vars[i].name)
  {
   MY_FREE(vars[i].name);
   MY_FREE(vars[i].val);

   vars[i].name = (U8*)malloc(ustrsizez(name));
   ustrcpy(vars[i].name, name);

   vars[i].val = (U8*)malloc(ustrsizez(val)); 
   ustrcpy(vars[i].val, val);

   return i;
  }
 }

 return -1;
}

/*

  Enleve une variable

*/
int csl_remove_var(U8 *name)
{
 int i;

 i = get_var_i(name);
 if(i >= 0)
 {
  MY_FREE(vars[i].name);
  MY_FREE(vars[i].val);
  return 1;
 }

 return 0;
}

/*

  Enleve tout les variables

*/
void csl_remove_all_var(void)
{
 int i;

 if(!vars)
  return;

 for(i = 0; i < CSL_MAX_VAR; i++)
 {
  MY_FREE(vars[i].name);
  MY_FREE(vars[i].val);
 }
}

/*

  Change la valeur d'une variable

*/
int csl_set_var(U8 *name, U8 *val)
{
 int i;

 i = get_var_i(name);
 if(i >= 0)
 {
  MY_FREE(vars[i].val);
  vars[i].val = (U8*)malloc(ustrsizez(val)); 
  ustrcpy(vars[i].val, val);
  return 1;
 }

 return 0;
}

/*

  Retourne la valeur d'une variable

*/
U8 *csl_get_var(U8 *name, U8 *def)
{
 int i;

 i = get_var_i(name);
 if(i >= 0)
 { 
  return vars[i].val;
 }

 return def;
}

/*

  Change le rectangle de la console

*/
void csl_set_rect(int cl, int ct, int cr, int cb)
{
 if(cb > csl_y)
  cb = csl_y;

 c_cl = (cl >= 0) ? cl : 0;
 c_cr = (cr >= 0) ? cr : CSL_DBUFF_W;
 c_ct = (ct >= 0) ? ct : 0;
 c_cb = (cb >= 0) ? cb : CSL_DBUFF_H;
}

/*

  Retourne les coo du rectangle

*/
int csl_get_rect_l(void)
{
 return c_cl;
}

int csl_get_rect_r(void)
{
 return c_cr;
}

int csl_get_rect_t(void)
{
 return c_ct;
}

int csl_get_rect_b(void)
{
 return c_cb;
}

int csl_get_temp_y(void)
{
 return temp_y;
}

/*

  Initialise le clipping de CSL_DBUFF

*/
static void c_set_clip()
{
 set_clip_rect(CSL_DBUFF, c_cl, c_ct, c_cr, c_cb);
}

/*

  Change le prompte et le curseur

*/
void csl_set_prompt(U8 *prompt, U8 *iprompt)
{
 if(prompt)
  ustrncpy((U8*)csl_prompt,(U8*)prompt, CSL_PROMPT_MAX_SIZE);

 if(iprompt)
  ustrncpy((U8*)csl_i_prompt, (U8*)iprompt, CSL_PROMPT_MAX_SIZE);
}

/*

  Remonte d'une ligne le texte de la console

*/
static void c_line_up(void)
{
 while(ugetc(csl_text_start) != ('\0' && 0) &&
       ugetc(csl_text_start) != CT_NEW_LINE)
  csl_text_start += uwidth(csl_text_start);

 if(ugetc(csl_text_start) != ('\0' && 0))
  csl_text_start += uwidth(csl_text_start);
}

/*

  Descand d'une ligne le texte de la console

*/
static void c_line_down(void)
{
 if(csl_text_start == csl_text)
  return;

 if(ugetc(csl_text_start) == CT_NEW_LINE && csl_text_start-1 == csl_text)
  return;
 else
  csl_text_start -= uwidth(csl_text_start);

 if(csl_text_start == csl_text)
  return;

 if(ugetc(csl_text_start) == CT_NEW_LINE && csl_text_start-1 == csl_text)
  return;
 else
  csl_text_start -= uwidth(csl_text_start);

 while(csl_text_start != csl_text && ugetc(csl_text_start) != CT_NEW_LINE)
  csl_text_start -= uwidth(csl_text_start);;

 if(ugetc(csl_text_start) == CT_NEW_LINE)
  ugetx((char**)&csl_text_start);
}

/*

  Initialise le curseur par defaut

*/
int create_default_cursor(void)
{
 if(!CSL_FONT)
  return 0;

 MY_DESTROY(bitmap, default_cursor_sprite);

 default_cursor_sprite = create_bitmap(font_w(CSL_FONT, -1), text_height(CSL_FONT));
 if(!default_cursor_sprite)
 {
  sprintf(allegro_error, "set_default_cursor() - Unable to create default_cursor_sprite");
  return 0;
 }

 clear_to_color(default_cursor_sprite, default_cursor_sprite->vtable->mask_color);
 hline(default_cursor_sprite, 0, default_cursor_sprite->h,
       default_cursor_sprite->w, csl_font_color_fg);
 hline(default_cursor_sprite, 0, default_cursor_sprite->h-1,
       default_cursor_sprite->w, csl_font_color_fg);
 hline(default_cursor_sprite, 0, default_cursor_sprite->h-2,
       default_cursor_sprite->w, csl_font_color_fg);

 return 1;
}

/*

  Change le curseur par defaut

*/
void csl_set_cursor(int type, BITMAP *bmp)
{
 switch(type)
 {
  case CSL_CURSOR_EMPTY: csl_cursor_sprite = NULL; break;
  case CSL_CURSOR_BITMAP: csl_cursor_sprite = bmp; break;
  default:
       create_default_cursor();
       csl_cursor_sprite = default_cursor_sprite;
       break;
 }

 draw = 1;
 get_prompt_bkgd = 1;
}

/*

  Change la vitesse de descante et de montee de la console

*/
void csl_set_move_speed(int spddown, int spdup)
{
 if(spddown > 0)
  csl_move_speed_down = spddown;

 if(spdup > 0)
  csl_move_speed_up = spdup;
}

/*

  Met a on ou off la fonction de reddesin permanent de la console

*/
void csl_set_always_redraw(int on_off)
{
 c_always_redraw = on_off;
}

/*

  Change le prompte avec un string

*/
void csl_set_prompt_text(char *s, ...)
{
 char str[8000];

 va_list ap;

 va_start(ap, s);
 uvsprintf((U8*)str, (U8*)s, ap);
 va_end(ap);

 ZERO_CSL_MSG();
 ustrcpy((U8*)csl_msg, (U8*)str);
 chars = ustrlen((U8*)str);
}

/*

  Met a on ou off show_command

*/
void csl_set_show_command(int onoff)
{
 write_prompt = onoff;
}

/*

  Retourne show_command

*/
int csl_get_show_command(void)
{
 return write_prompt;
}

/*

  Push le curseur de la console

*/
void csl_push_cursor(void)
{
 pushed_csl_cursor = csl_cursor_sprite;
}

/*

  Pop le curseur de la console

*/
void csl_pop_cursor(void)
{
 csl_cursor_sprite = pushed_csl_cursor;
}

/*

  Change la fonction correspondante a un caractere special

*/
void csl_set_schar(U8 sc, void (*func)(int cl, int cr, int y))
{
 schar[sc].func = func;
}

/*

  Change l'etat de draw_after_textout

*/
void csl_set_draw_after_textout(int onoff)
{
 draw_after_textout = onoff;
}

/*

  Retourne le argc de la derniere fonction lancee

*/
int csl_argc(void)
{
 return i_argc;
}

/*

  Retourne le argv de la derniere fonction lancee

*/
U8 *csl_argv(int arg)
{
 U8 *b;

 b = i_argv[arg];

 if(ugetc(b) == '$')
 {
  b += uwidth(b);
  b = csl_get_var(b, i_argv[arg]);
 }

 return  b;
}



/*============================================================================

   DEFAULT CONSOLE FUNCTIONS

============================================================================*/
static int csl_func_quit_console(void)
{
 temp_y = 0;
 return CSLMSG_CLOSE;
}



static int csl_func_clear_console(void)
{
 int d;

 ZERO_CSL_TEXT();

 d = draw; draw = 1;
 draw_console(CSL_DRAW_REDRAW);
 draw = d; d = 0;

 return CSLMSG_O_K;
}



static int csl_func_echo(void)
{
 int i;
 int argc = csl_argc();
 U8 buf[(CSL_MAX_ARG*MAX_CHAR_STRING)+CSL_MAX_ARG];
 U8 *b = NULL;

 if(argc)
 {
  memset(buf, 0, (CSL_MAX_ARG*MAX_CHAR_STRING)+CSL_MAX_ARG);
  for(i = 1; i < argc+1; i++)
  {
   b = csl_argv(i);
   ustrcat(buf, b);
   ustrcat(buf, " ");
  }

  i = csl_get_show_command();
  csl_set_show_command(0);
  csl_textout(1, buf);
  csl_set_show_command(i);
 }

 return CSLMSG_O_K;
}



static int csl_func_list_funcs(void)
{
 int i;

 csl_textout(2, "List of functions:");
 for(i = 0; i < CSL_MAX_FUNC; i++)
 {
  if(funcs[i].func)
   csl_textoutf(1, "-> %s", funcs[i].cmd);
 }

 return CSLMSG_O_K;
}



static int csl_func_list_vars(void)
{
 int i;

 csl_textout(2, "List of variables:");
 for(i = 0; i < CSL_MAX_VAR; i++)
 {
  if(vars[i].name)
   csl_textoutf(1, " %s = %s", vars[i].name, vars[i].val);
 }

 return CSLMSG_O_K;
}



static int csl_func_set(void)
{
 if(csl_argc() != 2)
 {
  csl_textout(2, "syntax: set var_name value");
  return CSLMSG_O_K;
 }

 if(csl_set_var(csl_argv(1), csl_argv(2)))
 {
  csl_textoutf(1, "%s = %s", csl_argv(1), csl_get_var(csl_argv(1), "no_value"));
 }
 else
 {
  csl_textoutf(1, "variable [%s] not exist", csl_argv(1));
 }

 return CSLMSG_O_K;
}
