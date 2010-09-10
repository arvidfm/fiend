////////////////////////////////////////////////////
// This is various gfx and math functions used
// in the game. 
//
// Code by Thomas Grip, Pierre Renaux and Johan Peitz
///////////////////////////////////////////////////



#include <allegro.h>
#include <math.h>
#include <time.h>
#include "grafik4.h"


float cos_table[3601];
float sin_table[3601];
float tan_table[3601];

//initialize some stuff
int init_grafik4(void)
{
int i;
 float a;

 for(i=0;i<3601;i++) // make the sinus tables....
  {
   a = (float)i;
   sin_table[i] =(float) sin( ((a/10)/360)*2*PI);
   cos_table[i] =(float) cos( ((a/10)/360)*2*PI);
   tan_table[i] =(float) tan( degree_to_radian(a/10) );
  }
 
 srand(time(NULL));
 
 return 0;
}



//what tile is att x,y? This fucntion tells you...
void map_tile(int x, int y, int *tile_x, int *tile_y, int tile_size)
{
 *tile_x=(x-(x%tile_size))/tile_size;
 *tile_y=(y-(y%tile_size))/tile_size;
}


//draws a pic with with a energi/max_energi length
void energy_pic(int x, int y,BITMAP *pic,int maxenergi,int energi, BITMAP *vart)
{
 float ans;
 int final_length;
 ans = (float)energi/(float)maxenergi;
 final_length =(int) (ans * (float)pic->w);
 blit(pic,vart,0,0,x,y,final_length,pic->h);

}



//adds add to angle and makes sure 0<=angle<=360
float add_angle(float angle, float add)
{
 angle+=add;
 if(angle>360)
  angle = angle-360;
 if(angle<0)
  angle = 360+angle;

 return angle;

}


//calulates distance from 1 to 2 
float distance(int x1,int y1,int x2,int y2)
{

 double ans;

 x1=x1-x2;
 y1=y1-y2;
 
 ans=(x1*x1)+(y1*y1);
 ans=sqrt(ans);
 return ans;
}


//Checks wich of the strings that comes first in alphabetic 
//order, it returns -1 if they are the same...
/*int first_in_alphabet(char *string1, char *string2)
{
	int end=0;
	int letter=0;

	while(!end)
	{
		if(letter>strlen(string1) || letter>strlen(string2))
			return -1;

		if(string1[letter]!=string2[letter])
		{
			if(string1[letter]<string2[letter])
				return 1;
			else
				return 2;
		}

		letter++;
	}
}*/


//calculates x and y from a vektor
void xyplus(float hypot,float angle, float *xplus, float *yplus)
{
 float ans;
 if(angle <= 90)
 {
  ans = COS(angle);
  ans = ans*hypot;
  *yplus = -ans;
  ans = SIN(angle);
  ans = ans*hypot;
  *xplus = ans;
 }
 else if(angle <= 180)
 {
  angle-=90;
  ans = SIN(angle);
  ans = ans*hypot;
  *yplus = ans;
  ans = COS(angle);
  ans = ans*hypot;
  *xplus = ans;
 }
 else if(angle <= 270)
 {
  angle-=180;
  ans = COS(angle);
  ans = ans*hypot;
  *yplus = ans;
  ans = SIN(angle);
  ans = ans*hypot;
  *xplus = -ans;
 }
 else
 {
  angle-=270;
  ans = SIN(angle);
  ans = ans*hypot;
  *yplus = -ans;
  ans = COS(angle);
  ans = ans*hypot;
  *xplus = -ans;
 }
}




//I can't explain this shit....
// or maybe  I can.. if an objects position is x,y relative to antoher object 
//the this object calculates the new x,y when the main object is turned
void compute_towerxy(float angle,int xplus,int yplus, float *new_xplus, float *new_yplus)
{
 float distance;
 float tempfix=0;
 float tempfix2=0;
 float angle_add;

 tempfix = (xplus*xplus)+(yplus*yplus);
 
 distance = sqrt(tempfix);
 angle_add = compute_angle(xplus,yplus,0,0);
 
 
 xyplus(distance, add_angle(angle, angle_add),new_xplus, new_yplus);
}

// int k, key to find name of
// char *dest, string to copy name to
// ex: 
//   char str[32];
//    key_2_str(KEY_ENTER, str);
void key_2_str(int k, char *dest)
{
    if (k == KEY_A) strcpy(dest, "A");
    else if (k == KEY_B) strcpy(dest, "B");
    else if (k == KEY_C) strcpy(dest, "C");              
    else if (k == KEY_D) strcpy(dest, "D");
    else if (k == KEY_E) strcpy(dest, "E");
    else if (k == KEY_F) strcpy(dest, "F");
    else if (k == KEY_G) strcpy(dest, "G");
    else if (k == KEY_H) strcpy(dest, "H");
    else if (k == KEY_I) strcpy(dest, "I");
    else if (k == KEY_J) strcpy(dest, "J");
    else if (k == KEY_K) strcpy(dest, "K");
    else if (k == KEY_L) strcpy(dest, "L");
    else if (k == KEY_M) strcpy(dest, "M");
    else if (k == KEY_N) strcpy(dest, "N");
    else if (k == KEY_O) strcpy(dest, "O");
    else if (k == KEY_P) strcpy(dest, "P");
    else if (k == KEY_Q) strcpy(dest, "Q");
    else if (k == KEY_R) strcpy(dest, "R");
    else if (k == KEY_S) strcpy(dest, "S");
    else if (k == KEY_T) strcpy(dest, "T");
    else if (k == KEY_U) strcpy(dest, "U");
    else if (k == KEY_V) strcpy(dest, "V");
    else if (k == KEY_W) strcpy(dest, "W");
    else if (k == KEY_X) strcpy(dest, "X");
    else if (k == KEY_Y) strcpy(dest, "Y");
    else if (k == KEY_Z) strcpy(dest, "Z");
    else if (k == KEY_0) strcpy(dest, "0");
    else if (k == KEY_1) strcpy(dest, "1");
    else if (k == KEY_2) strcpy(dest, "2");
    else if (k == KEY_3) strcpy(dest, "3");
    else if (k == KEY_4) strcpy(dest, "4");
    else if (k == KEY_5) strcpy(dest, "5");
    else if (k == KEY_6) strcpy(dest, "6");
    else if (k == KEY_7) strcpy(dest, "7");
    else if (k == KEY_8) strcpy(dest, "8");
    else if (k == KEY_9) strcpy(dest, "9");
    else if (k == KEY_0_PAD) strcpy(dest, "0 (Pad)");
    else if (k == KEY_1_PAD) strcpy(dest, "1 (Pad)");
    else if (k == KEY_2_PAD) strcpy(dest, "2 (Pad)");
    else if (k == KEY_3_PAD) strcpy(dest, "3 (Pad)");
    else if (k == KEY_4_PAD) strcpy(dest, "4 (Pad)");
    else if (k == KEY_5_PAD) strcpy(dest, "5 (Pad)");
    else if (k == KEY_6_PAD) strcpy(dest, "6 (Pad)");
    else if (k == KEY_7_PAD) strcpy(dest, "7 (Pad)");
    else if (k == KEY_8_PAD) strcpy(dest, "8 (Pad)");
    else if (k == KEY_9_PAD) strcpy(dest, "9 (Pad)");
    else if (k == KEY_F1) strcpy(dest, "F1");
    else if (k == KEY_F2) strcpy(dest, "F2");
    else if (k == KEY_F3) strcpy(dest, "F3");
    else if (k == KEY_F4) strcpy(dest, "F4");
    else if (k == KEY_F5) strcpy(dest, "F5");
    else if (k == KEY_F6) strcpy(dest, "F6");
    else if (k == KEY_F7) strcpy(dest, "F7");
    else if (k == KEY_F8) strcpy(dest, "F8");
    else if (k == KEY_F9) strcpy(dest, "F9");
    else if (k == KEY_F10) strcpy(dest, "F10");
    else if (k == KEY_F11) strcpy(dest, "F11");
    else if (k == KEY_F12) strcpy(dest, "F12");
    else if (k == KEY_ESC) strcpy(dest, "ESC");
    else if (k == KEY_TILDE) strcpy(dest, "~");
    else if (k == KEY_MINUS) strcpy(dest, "-");
    else if (k == KEY_EQUALS) strcpy(dest, "=");
    else if (k == KEY_BACKSPACE) strcpy(dest, "Backspace");
    else if (k == KEY_TAB) strcpy(dest, "Tab");
    else if (k == KEY_OPENBRACE) strcpy(dest, "{");
    else if (k == KEY_CLOSEBRACE) strcpy(dest, "}");
    else if (k == KEY_ENTER) strcpy(dest, "Enter");
    else if (k == KEY_COLON) strcpy(dest, ":");
    else if (k == KEY_QUOTE) strcpy(dest, "'");
    else if (k == KEY_BACKSLASH) strcpy(dest, "\\");
    else if (k == KEY_BACKSLASH2) strcpy(dest, "\\");
    else if (k == KEY_COMMA) strcpy(dest, ",");
    else if (k == KEY_STOP) strcpy(dest, ".");
    else if (k == KEY_SLASH) strcpy(dest, "/");
    else if (k == KEY_SPACE) strcpy(dest, "Space");
    else if (k == KEY_INSERT) strcpy(dest, "Insert");
    else if (k == KEY_DEL) strcpy(dest, "Delete");
    else if (k == KEY_HOME) strcpy(dest, "Home");
    else if (k == KEY_END) strcpy(dest, "End");
    else if (k == KEY_PGUP) strcpy(dest, "Page Up");
    else if (k == KEY_PGDN) strcpy(dest, "Page Down");
    else if (k == KEY_LEFT) strcpy(dest, "Left Arrow");
    else if (k == KEY_RIGHT) strcpy(dest, "Right Arrow");
    else if (k == KEY_UP) strcpy(dest, "Up Arrow");
    else if (k == KEY_DOWN) strcpy(dest, "Down Arrow");
    else if (k == KEY_SLASH_PAD) strcpy(dest, "/ (Pad)");
    else if (k == KEY_ASTERISK) strcpy(dest, "*");
    else if (k == KEY_MINUS_PAD) strcpy(dest, "- (Pad)");
    else if (k == KEY_PLUS_PAD) strcpy(dest, "+ (Pad)");
    else if (k == KEY_DEL_PAD) strcpy(dest, "Delete (Pad)");
    else if (k == KEY_ENTER_PAD) strcpy(dest, "Enter (Pad)");
    else if (k == KEY_PRTSCR) strcpy(dest, "Print Screen");
    else if (k == KEY_PAUSE) strcpy(dest, "Pause");
    else if (k == KEY_YEN) strcpy(dest, "Yen");
    //else if (k == KEY_YEN2) strcpy(dest, "Yen2");
    //else if (k == KEY_KANA) strcpy(dest, "Kana");
    //else if (k == KEY_HENKAN) strcpy(dest, "Henkan");
    //else if (k == KEY_MUHENKAN) strcpy(dest, "Muhenkan");
    else if (k == KEY_LSHIFT) strcpy(dest, "Left Shift");
    else if (k == KEY_RSHIFT) strcpy(dest, "Right Shift");
    else if (k == KEY_LCONTROL) strcpy(dest, "Left Control");
    else if (k == KEY_RCONTROL) strcpy(dest, "Right Control");
    else if (k == KEY_ALT) strcpy(dest, "Alt");
    else if (k == KEY_ALTGR) strcpy(dest, "Alt Gr");
    else if (k == KEY_LWIN) strcpy(dest, "Left Win");
    else if (k == KEY_RWIN) strcpy(dest, "Right Win");
    else if (k == KEY_MENU) strcpy(dest, "Menu");
    else if (k == KEY_SCRLOCK) strcpy(dest, "Scroll Lock");
    else if (k == KEY_NUMLOCK) strcpy(dest, "Num Lock");
    else if (k == KEY_CAPSLOCK) strcpy(dest, "Caps Lock");
}


//computes angle betwwen two points
//center_2 is the "start" and center_1 is the "goal"
float compute_angle(int center_x1,int center_y1,int center_x2,int center_y2)
{
 float delta_x;
 float delta_y;
 fixed fixtemp;
 fixed ans;

 delta_x = center_x1 - center_x2;
 delta_y = center_y2 - center_y1;


 if(delta_x>=0 && delta_y>=0)
  {
   fixtemp = ftofix(delta_y/delta_x);
   ans = fatan(fixtemp);
   ans = ftofix(64)-ans;
  }
 else if(delta_x>=0 && delta_y<0)
  {
   delta_y=-delta_y;
   fixtemp = ftofix(delta_y/delta_x);
   ans = fatan(fixtemp);
   ans = ans + ftofix(64);
  }
 else if(delta_x<0 && delta_y<0)
  {
   delta_y=-delta_y;
   delta_x=-delta_x;
   fixtemp = ftofix(delta_x/delta_y);
   ans = fatan(fixtemp);
   ans = ans + ftofix(128);
  }
 else if(delta_x<0 && delta_y>=0)
  {
   delta_x=-delta_x;
   fixtemp = ftofix(delta_y/delta_x);
   ans = fatan(fixtemp);
   ans = ans + ftofix(192);
  }


 return fixed_to_degree(ans);


}





//computes shortest distance between two angles
float compute_angle_distance(float angle1, float angle2)
{
  float ans1, ans2;
	
	if(angle1==angle2)
  {
	  return 0;
  }
  else if(angle1>angle2)
  {
		ans1= angle1-angle2;
		angle2+=360;
		ans2= angle2-angle1;
		
		if(ans1<ans2)
			return ans1;
		else
			return ans2;

  }
  else
  {
		ans1= angle2-angle1;
		angle1+=360;
		ans2= angle1-angle2;
		
		if(ans1<ans2)
			return ans1;
		else
			return ans2;
  }


}

//adds 2 vectors...
void add_vectors(float hypot1, float angle1, float hypot2, float angle2, float *new_hypot, float *new_angle)
{
	float dx1,dy1,dx2,dy2;
	float dx,dy;
	fixed temp;


	xyplus(hypot1,angle1,&dx1,&dy1);
	xyplus(hypot2,angle2,&dx2,&dy2);

	dx = dx1+dx2;
	dy = dy1+dy2;

	*new_hypot = sqrt((dx*dx)+(dy*dy));

	temp = asin( ftofix(*new_hypot/dx) );
	*new_angle = fixed_to_degree(temp);
}




//converts a float angle to a allegro fixed.....
fixed degree_to_fixed(float angle)
{
	float ans;
	ans = (angle/360)*256;
	return ftofix(ans);
}

//converts allegros fixed angle to a float
float fixed_to_degree(fixed angle)
{
	float ans;
	ans = fixtof(angle);
	return (ans/256)*360;

}


//gets h and w of an allegro bitmap
int get_bitmap_h(BITMAP*temp)
{

	return temp->h;
}

int get_bitmap_w(BITMAP*temp)
{

	return temp->w;
}

//converts randians to degrees
float radian_to_degree(float r)
{
	return r *(360/(2*PI));
}

//converts randians to degrees
float degree_to_radian(float d)
{
	return d *((2*PI)/360);
}


// Remmember that x and y is the center!!!!
// Like bounding box but rotated  
int check_angle_collision(float x1, float y1, int w1, int h1, float angle1, float x2, float y2, int w2, int h2, float angle2)
{
	int i,j;
	
	float x3,x4,y3,y4,xi,yi;
	
	int x_max1, y_max1, y_min1, x_min1, x_max2, y_max2, y_min2, x_min2;
	int max1, max2;

	float corner1_x[4];
	float corner1_y[4];

	float corner2_x[4];
	float corner2_y[4];

	float temp_corner1_x[4];
	float temp_corner1_y[4];

	float temp_corner2_x[4];
	float temp_corner2_y[4];

	float a1,b1,c1,a2,b2,c2,det_inv,m1,m2;

	int line[4][2]={{0,1},{1,2},{2,3},{3,0}};

	float intersect_min_x=1000000,intersect_min_y=1000000,intersect_max_x=-100000,intersect_max_y=-100000; 
	
	
	//----Begin with a simple test----//
	//get the largest sides possible
	max1 = sqrt(w1*w1 + h1*h1);
	max2 = sqrt(w2*w2 + h2*h2);
	if(!check_collision(x1-max1/2, y1-max1/2, max1, max1, x2-max2/2, y2-max2/2, max2, max2)) return 0;


	
	////-------Make the corners------////
	//for 1:
	temp_corner1_x[0] = -w1/2;
	temp_corner1_y[0] = -h1/2;
	
	temp_corner1_x[1] = w1/2;
	temp_corner1_y[1] = -h1/2;

	temp_corner1_x[2] = w1/2;
	temp_corner1_y[2] = h1/2;

	temp_corner1_x[3] = -w1/2;
	temp_corner1_y[3] = h1/2;
	
	//for 2:
	temp_corner2_x[0] = -w2/2;
	temp_corner2_y[0] = -h2/2;
	
	temp_corner2_x[1] = w2/2;
	temp_corner2_y[1] = -h2/2;

	temp_corner2_x[2] = w2/2;
	temp_corner2_y[2] = h2/2;

	temp_corner2_x[3] = -w2/2;
	temp_corner2_y[3] = h2/2;
	
	
	//--------Rotate the corners------////
	
	for(i=0;i<4;i++)
	{
		corner1_x[i] = (temp_corner1_x[i]*COS(angle1)-temp_corner1_y[i]*SIN(angle1))+x1;
		corner1_y[i] = (temp_corner1_y[i]*COS(angle1)+temp_corner1_x[i]*SIN(angle1))+y1;

		corner2_x[i] = (temp_corner2_x[i]*COS(angle2)-temp_corner2_y[i]*SIN(angle2))+x2;
		corner2_y[i] = (temp_corner2_y[i]*COS(angle2)+temp_corner2_x[i]*SIN(angle2))+y2;
	}
	
	
	
	//----Check if any lines intersect-----//
	
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			x1 = corner1_x[line[i][0]];
			y1 = corner1_y[line[i][0]];
			x2 = corner1_x[line[i][1]];
			y2 = corner1_y[line[i][1]];

			x3 = corner2_x[line[j][0]];
			y3 = corner2_y[line[j][0]];
			x4 = corner2_x[line[j][1]];
			y4 = corner2_y[line[j][1]];

			
			//----Compute the x and y intersection---
			if ((x2-x1)!=0)
				m1 = (y2-y1)/(x2-x1);
			else
				m1 = (float)1e+10;   // close enough to infinity
			
			if ((x4-x3)!=0)
				m2 = (y4-y3)/(x4-x3);
			else   
				m2 = (float)1e+10;   // close enough to infinity
			
				
			a1 = m1;
			a2 = m2;
			b1 = -1;
			b2 = -1;
			
			c1 = (y1-m1*x1);
			c2 = (y3-m2*x3);

			// compute the inverse of the determinate
			det_inv = 1/(a1*b2 - a2*b1);

			// use Kramers rule to compute xi and yi
			xi=((b1*c2 - b2*c1)*det_inv);
			yi=((a2*c1 - a1*c2)*det_inv);


			
			//calculate the max and min point of each line
			x_max1 =x1;
			x_min1 =x2;
			if(x2>x1)
			{
				x_max1=x2;
				x_min1=x1;
			}
			
			y_max1 =y1;
			y_min1 =y2;
			if(y2>y1)
			{
				y_max1=y2;
				y_min1=y1;
			}
			
			
			x_max2 =x3;
			x_min2 =x4;
			if(x4>x3)
			{
				x_max2=x4;
				x_min2=x3;
			}
			
			y_max2 =y3;
			y_min2 =y4;
			if(y4>y3)
			{
				y_max2=y4;
				y_min2=y3;
			}			

			//check if the x points on both lines and if then its  a collison
			if((int)xi<=(int)x_max1 && (int)xi>=(int)x_min1 && (int)yi<=(int)y_max1 && (int)yi>=(int)y_min1 && 
				(int)xi<=(int)x_max2 && (int)xi>=(int)x_min2 && (int)yi<=(int)y_max2 && (int)yi>=(int)y_min2)
			{
				return 1;
			}

			if(((int)xi<=(int)x_max1 && (int)xi>=(int)x_min1 && (int)yi<=(int)y_max1 && (int)yi>=(int)y_min1 )|| 
				((int)xi<=(int)x_max2 && (int)xi>=(int)x_min2 && (int)yi<=(int)y_max2 && (int)yi>=(int)y_min2))
			{	
				if(xi<intersect_min_x)intersect_min_x=xi;
				if(yi<intersect_min_y)intersect_min_y=yi;
				if(xi>intersect_max_x)intersect_max_x=xi;
				if(yi>intersect_max_y)intersect_max_y=yi;
			}

			
		}

		/*rect(screen, intersect_max_x, intersect_max_y, intersect_min_x, intersect_min_y,makecol(0,0,255));
		//textprintf(screen, font,40,40,makecol(0,0,255),"x:%f y:%f",intersect_max_x,intersect_max_y);
		rect(screen,corner1_x[0],corner1_y[0],corner1_x[2],corner1_y[2],makecol(0,255,0));
		readkey();*/


		//Test is square 1 is in the intersections 
		j=0;
		for(i=0;i<4;i++)
		{
			if(corner1_x[i]<intersect_max_x && corner1_y[i]<intersect_max_y && corner1_x[i]>intersect_min_x && corner1_y[i]>intersect_min_y)
				j++;
			
		}
		
		if(j==4)return 1;

		
		//Test is square 2 is in the intersections
		j=0;
		for(i=0;i<4;i++)
		{
			if(corner2_x[i]<intersect_max_x && corner2_y[i]<intersect_max_y && corner2_x[i]>intersect_min_x && corner2_y[i]>intersect_min_y)
				j++;
		}
		if(j==4)return 1;



	return 0;
}




//A VERY simple line collsion test fast but very innaccurate
//if 0 than the lines are 100% not crossed
int simple_line_collision(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	int top_x1,bottom_x1,top_y1,bottom_y1;
	int top_x2,bottom_x2,top_y2,bottom_y2;

	//Find bottom and top of the first line
	if(y1<y2)
	{
		top_y1 = y1;
		bottom_y1 = y2;
	}
	else
	{
		top_y1 = y2;
		bottom_y1 = y1;
	}
	if(x1<x2)
	{
		top_x1 = x1;
		bottom_x1 = x2;
	}
	else
	{
		top_x1 = x2;
		bottom_x1 = x1;
	}

	//Find bottom and top of the second line
	if(y3<y4)
	{
		top_y2 = y3;
		bottom_y2 = y4;
	}
	else
	{
		top_y2 = y4;
		bottom_y2 = y3;
	}
	if(x3<x4)
	{
		top_x2 = x3;
		bottom_x2 = x4;
	}
	else
	{
		top_x2 = x4;
		bottom_x2 = x3;
	}



	if(check_collision(top_x1,top_y1,bottom_x1-top_x1,bottom_y1-top_y1,top_x2,top_y2,bottom_x2-top_x2,bottom_y2-top_y2))
		return 1;


	return 0;
}


//accurate line intersection allgoritm
int check_line_angle_collision(float x, float y, int w, int h, float angle, float line_x1, float line_y1, float line_x2, float line_y2)
{
	int i;
	
	float x1,y1,x2,y2,x3,x4,y3,y4,xi,yi;
	
	int x_max1, y_max1, y_min1, x_min1, x_max2, y_max2, y_min2, x_min2;
	int max;

	float corner_x[4];
	float corner_y[4];

	float temp_corner_x[4];
	float temp_corner_y[4];

	
	float a1,b1,c1,a2,b2,c2,det_inv,m1,m2;

	int a_line[4][2]={{0,1},{1,2},{2,3},{3,0}};

	float intersect_min_x=1000000,intersect_min_y=1000000,intersect_max_x=-100000,intersect_max_y=-100000; 
	
	//make a simple test if the largest posssible turned object intersects with the line...
	max = sqrt(w*w + h*h);	
	if(!simple_line_collision(line_x1,line_y1,line_x2,line_y2,x-max/2,y-max/2,x+max/2,y+max/2))
		return 0;
			
	////-------Make the corners------////
	temp_corner_x[0] = -w/2;
	temp_corner_y[0] = -h/2;
	
	temp_corner_x[1] = w/2;
	temp_corner_y[1] = -h/2;

	temp_corner_x[2] = w/2;
	temp_corner_y[2] = h/2;

	temp_corner_x[3] = -w/2;
	temp_corner_y[3] = h/2;

	
	
	//--------Rotate the corners------////
	
	for(i=0;i<4;i++)
	{
		corner_x[i] = (temp_corner_x[i]*COS(angle)-temp_corner_y[i]*SIN(angle))+x;
		corner_y[i] = (temp_corner_y[i]*COS(angle)+temp_corner_x[i]*SIN(angle))+y;
	}
	
	
	
	//----Check if any lines intersect-----//
	
	for(i=0;i<4;i++)
	{
			x1 = corner_x[a_line[i][0]];
			y1 = corner_y[a_line[i][0]];
			x2 = corner_x[a_line[i][1]];
			y2 = corner_y[a_line[i][1]];

			x3 = line_x1;
			y3 = line_y1;
			x4 = line_x2;
			y4 = line_y2;

			if(!simple_line_collision(x1,y1,x2,y2,x3,y3,x4,y4))
				continue;
						
			//----Compute the x and y intersection---
			if ((x2-x1)!=0)
				m1 = (y2-y1)/(x2-x1);
			else
				m1 = (float)1e+10;   // close enough to infinity
			
			if ((x4-x3)!=0)
				m2 = (y4-y3)/(x4-x3);
			else   
				m2 = (float)1e+10;   // close enough to infinity
			
				
			a1 = m1;
			a2 = m2;
			b1 = -1;
			b2 = -1;
			
			c1 = (y1-m1*x1);
			c2 = (y3-m2*x3);

			// compute the inverse of the determinate
			det_inv = 1/(a1*b2 - a2*b1);

			// use Kramers rule to compute xi and yi
			xi=((b1*c2 - b2*c1)*det_inv);
			yi=((a2*c1 - a1*c2)*det_inv);

			
			//calculate the max and min point of each line
			x_max1 =x1;
			x_min1 =x2;
			if(x2>x1)
			{
				x_max1=x2;
				x_min1=x1;
			}
			
			y_max1 =y1;
			y_min1 =y2;
			if(y2>y1)
			{
				y_max1=y2;
				y_min1=y1;
			}
			
			
			x_max2 =x3;
			x_min2 =x4;
			if(x4>x3)
			{
				x_max2=x4;
				x_min2=x3;
			}
			
			y_max2 =y3;
			y_min2 =y4;
			if(y4>y3)
			{
				y_max2=y4;
				y_min2=y3;
			}
			
			

			//line(screen, x3-map_x+80, y3-map_y, x4-map_x+80, y4-map_y, makecol(0,0,0));

			//check if the x points on both lines and if then its  a collison
			if((int)xi<=(int)x_max1 && (int)xi>=(int)x_min1 && (int)yi<=(int)y_max1 && (int)yi>=(int)y_min1 && 
				(int)xi<=(int)x_max2 && (int)xi>=(int)x_min2 && (int)yi<=(int)y_max2 && (int)yi>=(int)y_min2)
			{
				//circlefill(screen, xi-map_x+80, yi-map_y, 3,makecol(0,0,0));
				return 1;
			}

		}

	return 0;
		




}