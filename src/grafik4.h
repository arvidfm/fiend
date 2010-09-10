#include <allegro.h>


#define SIN(angle) sin_table[ (int)(angle*10)]
#define COS(angle) cos_table[ (int)(angle*10)]
#define TAN(angle) tan_table[ (int)(angle*10)]

extern float cos_table[3601];
extern float sin_table[3601];
extern float tan_table[3601];

//#define KEY_KANA              96
//#define KEY_HENKAN            97
//#define KEY_MUHENKAN          98



#define PI (3.1415926)

#define  check_collision(x1,y1,bredd1,hojd1,x2,y2,bredd2,hojd2) !(x1>x2+bredd2-1 || x2>x1+bredd1-1 || y1>y2+hojd2-1 || y2>y1+hojd1-1)


#define RANDOM(low, high) rand()%((int)(high)+1-((int)low))+((int)low)


//main routines
int init_grafik4(void);



//misc routines
void map_tile(int x, int y, int *tile_x, int *tile_y, int tile_size);

void energy_pic(int x, int y,BITMAP *pic,int maxenergi,int energi, BITMAP *vart);

void key_2_str(int k, char *dest);


//math routines
float add_angle(float angle, float add);

float distance(int x1,int y1,int x2,int y2);

void xyplus(float hypot,float angle, float *xplus, float *yplus);

void compute_towerxy(float angle,int xplus,int yplus, float *new_xplus, float *new_yplus);

float compute_angle(int center_x1,int center_y1,int center_x2,int center_y2);

float compute_angle_distance(float angle1, float angle2);

void add_vectors(float hypot1, float angle1, float hypot2, float angle2, float *new_hypot, float *new_angle);

fixed degree_to_fixed(float angle);
float fixed_to_degree(fixed angle);

int get_bitmap_h(BITMAP*temp);
int get_bitmap_w(BITMAP*temp);


float radian_to_degree(float r);
float degree_to_radian(float r);


int check_angle_collision(float x1, float y1, int h1, int w1, float angle1, float x2, float y2, int h2, int w2, float angle2);

int check_line_angle_collision(float x, float y, int w, int h, float angle, float line_x1, float line_y1, float line_x2, float line_y2);
