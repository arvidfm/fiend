////////////////////////////////////////////////////
// This file contains the intro and ending
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <stdio.h>

#include "../grafik4.h"


#include "../fiend.h"

DATAFILE *font_intro;

extern int play_menu_sound(char *name, int loop);


void show_gripdesign(void)
{
	BITMAP *bmp;
	BITMAP *bmp2;
	int end=0;
	int time=0;
	
	int alpha=255;


	bmp2 = load_bitmap("graphic/menu/logo.pcx",NULL);
	bmp = create_bitmap(480,480);
	
	stretch_sprite(bmp,bmp2,0,0,480,480);
		
	speed_counter = 0;
	

	while(!key[KEY_ESC] && !end)
	{
		
		while(speed_counter>0)
		{
			
			time++;

			if(time<350 && alpha>0)
				alpha-=2;

			if(alpha<0)alpha=0;

			if(time>350)
				alpha+=4;
			
			if(alpha>255)alpha=255;

			if(time>450)end=1;

			speed_counter--;
		}
		
		set_trans_blender(0,0,0,0);
		draw_lit_sprite(virt,bmp,0,0,alpha);
		vsync();
		blit(virt,screen,0,0,80,0,480,480);
	}

	destroy_bitmap(bmp);
	destroy_bitmap(bmp2);
}


void show_poem(void)
{
  char text1[] = "That is not dead which can eternal lie";
	char text2[] = "And with strange aeons even death may die";
	
	int end=0;
	int time=0;


	
	float alpha1=0;
	float alpha2=0;

	float x1,y1,x2,y2;


	speed_counter = 0;

	x1 = 0;
	y1 = 220;
	x2 = 0;
	y2 = 260;
	while(!key[KEY_ESC] && !end)
	{
		
		while(speed_counter>0)
		{
			
			time++;
			if(time==5)
				play_menu_sound("gong",0);
			if(time<200)
			{
				alpha1+=0.5;
			}
			else if(time<600)
			{
				alpha1+=0.5;
				alpha2+=0.7;
			
			}
			else if(time<800)
			{
				alpha1-=1.6;
				alpha2-=1.6;
			}
			else
			{
				end=1;
			}

			if(alpha1>255)alpha1=255;
			if(alpha1<0)alpha1=0;
			if(alpha2>255)alpha2=255;
			if(alpha2<0)alpha2=0;

	
			speed_counter--;
		}
		clear(virt);
		textout_ex(virt,font_avalon2->dat,text1, x1,y1,makecol(alpha1,alpha1,alpha1), -1);
		textout_ex(virt,font_avalon2->dat,text2, x2,y2,makecol(alpha2,alpha2,alpha2), -1);
	
		blit(virt,screen,0,0,80,0,480,480);
	}

}



void draw_background(BITMAP *dest, BITMAP *src, int x, int y, int alpha)
{
	int nx1,ny1,ny2,nx2;
	
	set_trans_blender(0,0,0,0);
	
	
	if(x<=0)nx1=480+x;
	if(x>0)nx1=-(480-x);
	ny1=y;

	if(y<=0)ny2=480+y;
	if(y>0)ny2=-(480-y);
	nx2=x;
	
	
	if(alpha>0)
	{
		draw_lit_sprite(dest,src,x,y, alpha);
		draw_lit_sprite(dest,src,nx1,ny1, alpha);
		draw_lit_sprite(dest,src,nx2,ny2, alpha);
		draw_lit_sprite(dest,src,nx1,ny2, alpha);
	}
	else
	{
		draw_sprite(dest,src,x,y);
		draw_sprite(dest,src,nx1,ny1);
		draw_sprite(dest,src,nx2,ny2);
		draw_sprite(dest,src,nx1,ny2);
	}
}




void show_intro_text(void)
{
	int end=0;
	int time=0;
	
	int i;
	
	float alpha=0;
	float a_add=1;
	float x=-130;
	float y=200;

	float x_add=0.4;
	int row=0;

	float bx=0,by=0;
	float bx_add=0.2,by_add=0.4;
	float b_alpha=255;
	float b_alpha_add=-2;

	char *story_text[30] = {"The year is 1928.",
							"",
							"You are Nick Cane, a mineral expert at a", 
							"mine-company in Boston. Today your job",
							"is to go to a little town in the woods",
							"called Lauder and do an investegation of",
							"a mine, to see if there is any chance of",
							"finding any valuable minerals there.",
							"",
							"Your comapny arranges a car to drive you",
							"there but since you will arrive quite late",
							"the car won't be back until the morning.",
							"You have therefor arranged so that you",
							"stay the night at George Smith, the man",
							"owning the mine.",
							"",
							"At ten past nine pm you arrive a lauder.",
							"END"};
	

	char text[50]="GRIPDESIGN";

	//BITMAP *bmp;
	//BITMAP *bmp2;
	//BITMAP *pic;

	//bmp = create_bitmap(480,40);
	//bmp2 = create_bitmap(480,480);
	
	//clear_to_color(bmp,makecol(255,0,255));

	//font_intro = load_datafile("graphic/fonts/intro.dat");
	//if(font_intro==NULL){allegro_message("couldn't load font intro");return;}
		
	fiend_fadeout(8);
	
	/*if(!play_fiend_music("Intro.it",0))
		{allegro_message("couldn't load intro.it");return;}

	speed_counter = 0;
	
	pic = load_bitmap("graphic/menu/intro1.pcx",NULL);
	stretch_sprite(bmp2, pic,0,0,480,480);
	destroy_bitmap(pic);

	
	while(!end && !key[KEY_ESC])
	{
		
		while(speed_counter>0)
		{
			
			time++;

			if(time==1750)
			{
				b_alpha_add = -b_alpha_add;
			}
			if(time==2000)
			{
				pic = load_bitmap("graphic/menu/intro2.pcx",NULL);
				stretch_sprite(bmp2, pic,0,0,480,480);
				destroy_bitmap(pic);

				bx_add=0.1;
				by_add=-0.25;

				b_alpha_add = -b_alpha_add;
			}
			
			if(time==4750)
			{
				b_alpha_add = -b_alpha_add;
			}
			if(time==5000)
			{
				pic = load_bitmap("graphic/menu/intro3.pcx",NULL);
				stretch_sprite(bmp2, pic,0,0,480,480);
				destroy_bitmap(pic);

				bx_add=-0.5;
				by_add=-0.2;

				b_alpha_add = -b_alpha_add;
			}
			if(time==7800)
			{
				b_alpha_add = -b_alpha_add;
			}
			

			
			
			if(time==250)
			{
				a_add = -a_add;			
			}
			if(time==500)
			{
				strcpy(text,"PRESENTS");
				a_add = -a_add;
				x = 100;
				x_add=-x_add;
				y = 255;
			}
			if(time==750)
			{
				a_add = -a_add;			
			}
			
			if(time==1000)
			{
				strcpy(text,"FIEND");
				a_add = -a_add;
				x = -100;
				x_add=-x_add;
				y = 220;
			}
			if(time==1400)
			{
				a_add = -a_add;			
			}
			
			if(time==1650)
			{
				strcpy(text,"PROGRAMMING");
				a_add = -a_add;
				x = 100;
				x_add=-x_add;
				y = 200;
			}
			if(time==1900)
			{
				a_add = -a_add;			
			}
			
			if(time==2150)
			{
				strcpy(text,"THOMAS GRIP");
				a_add = -a_add;
				x = -60;
				x_add=-x_add;
				y = 240;
			}
			if(time==2300)
			{
				a_add = -a_add;			
			}
			
			if(time==2550)
			{
				strcpy(text,"PIERRE RENAUX");
				a_add = -a_add;
				x = 70;
				x_add=-x_add;
				y = 230;
			}
			if(time==2700)
			{
				a_add = -a_add;			
			}
			
			
			if(time==2950)
			{
				strcpy(text,"GRAPHICS");
				a_add = -a_add;
				x = -100;
				x_add=-x_add;
				y = 170;
			}
			if(time==3200)
			{
				a_add = -a_add;			
			}
			
			if(time==3450)
			{
				strcpy(text,"THOMAS GRIP");
				a_add = -a_add;
				x = 80;
				x_add=-x_add;
				y = 230;
			}
			if(time==3700)
			{
				a_add = -a_add;			
			}
			
			if(time==3950)
			{
				strcpy(text,"ARON MATTSON");
				a_add = -a_add;
				x = -70;
				x_add=-x_add;
				y = 210;
			}
			if(time==4200)
			{
				a_add = -a_add;			
			}
			
			if(time==4450)
			{
				strcpy(text,"IVAN SOBTCHOUK");
				a_add = -a_add;
				x = 120;
				x_add=-x_add;
				y = 270;
			}
			if(time==4700)
			{
				a_add = -a_add;			
			}
			
			if(time==4950)
			{
				strcpy(text,"GRAPHTALLICA");
				a_add = -a_add;
				x = -30;
				x_add=-x_add;
				y = 220;
			}
			if(time==5200)
			{
				a_add = -a_add;			
			}
			
			if(time==5450)
			{
				strcpy(text,"MUSIC");
				a_add = -a_add;
				x = 130;
				x_add=-x_add;
				y = 200;
			}
			if(time==5700)
			{
				a_add = -a_add;			
			}
			
			if(time==5950)
			{
				strcpy(text,"THOMAS GRIP");
				a_add = -a_add;
				x = -120;
				x_add=-x_add;
				y = 260;
			}
			if(time==6200)
			{
				a_add = -a_add;			
			}
			
			if(time==6450)
			{
				strcpy(text,"SOUND");
				a_add = -a_add;
				x = 140;
				x_add=-x_add;
				y = 150;
			}
			if(time==6700)
			{
				a_add = -a_add;			
			}
			
			if(time==6950)
			{
				strcpy(text,"LACHIE MOUAT");
				a_add = -a_add;
				x = 30;
				x_add=-x_add;
				y = 250;
			}
			if(time==7200)
			{
				a_add = -a_add;			
			}
			if(time==7450)
			{
				strcpy(text,"DEDICATED TO");
				a_add = -a_add;
				x = 100;
				x_add=-x_add;
				y = 150;
			}
			if(time==7700)
			{
				a_add = -a_add;			
			}
			if(time==7950)
			{
				strcpy(text,"HP LOVECRAFT");
				a_add = -a_add;
				x = -50;
				x_add=-x_add;
				y = 200;
			}
			if(time==8200)
			{
				a_add = -a_add;			
			}
			
			if(time==8520)
			{
				end=1;
			}

			x+=x_add;			
			alpha+=a_add;
			if(alpha>255)alpha=255;
			if(alpha<0)alpha=0;

			bx+=bx_add;
			//bx = bx%480;
			by+=by_add;
			//by = by%480;
			
			b_alpha+=b_alpha_add;
			if(b_alpha>255)b_alpha=255;
			if(b_alpha<0)b_alpha=0;

			speed_counter--;
		}
		
		
		clear_to_color(bmp,makecol(255,0,255));
		textout_centre(bmp,font_intro->dat,text, 200,0,makecol(200,200,200));


		draw_background(virt,bmp2,((int)bx)%480,((int)by)%480,b_alpha);
	
		set_trans_blender(0,0,0,alpha);
		draw_trans_sprite(virt,bmp,x,y);
		vsync();
		blit(virt,screen,0,0,80,0,480,480);
	}



	stop_fiend_music();

	destroy_bitmap(bmp);
	destroy_bitmap(bmp2);
	unload_datafile(font_intro);*/


	speed_counter = 0;

	alpha = 0;
	a_add=3;

	while(strcmp(story_text[row],"END")!=0 && !key[KEY_ESC])
	{
		
		while(speed_counter>0)
		{
			
			time++;

			
			if(time%200==0)
			{
				row++;
				if(strcmp(story_text[row],"")==0)row++;
				alpha=0;
			}
			
			alpha+=a_add;
			if(alpha>255)alpha=255;
			if(alpha<0)alpha=0;
			
	
			speed_counter--;
		}
		
		clear(virt);
		textout_ex(virt,font_avalon2->dat,story_text[row], 0,row*25,makecol(alpha,alpha,alpha), -1);

		for(i=0;i<row;i++)
		{
			textout_ex(virt,font_avalon2->dat,story_text[i], 0,i*25,makecol(255,255,255), -1);
		}
		//textout(virt,font_avalon2->dat,text, x2,y2,makecol(alpha2,alpha2,alpha2));
	
		blit(virt,screen,0,0,80,0,480,480);
	}

	while(time<310 && !key[KEY_ESC])
	{
		
		while(speed_counter>0)
		{
			
			time++;
			
	
			speed_counter--;
		}
		
	}

}

void show_ending(void)
{
	char *story_text[30] = {"You wake up in an hospital bed.",
							"A nurse stands beside you.",
							"She says there was a forest fire.", 
							"She says you where the only one alive.",
							"",
							"You know that is not true.",
							"",
							"Your master is with you.",
							"You are one with him now.",
							"The cermony brought you two together.",
							"",
							"Suddenly you feel an urge for human flesh.",
							"",
							"Politly you ask the nurse to come closer...",
							"",
							"END"};
	int end=0;
	int time=0;
	
	int i;

	float alpha=0;
	float a_add=1;
	
	int row=0;

	
	speed_counter = 0;

	alpha = 0;
	a_add=2.5;

	clear(virt);
	
	stop_all_sounds();
	stop_fiend_music();
	
	play_menu_sound("heartbeat",0);

	if(fiend_show_intro)
	{
		while(strcmp(story_text[row],"END")!=0)
		{
		
			while(speed_counter>0)
			{
				
				time++;

			
				if(time%240==0)
				{
					play_menu_sound("heartbeat",0);
					row++;
					if(strcmp(story_text[row],"")==0)row++;
					alpha=0;
				}
			
				alpha+=a_add;
				if(alpha>255)alpha=255;
				if(alpha<0)alpha=0;
			
	
				speed_counter--;
			}
		
			clear(virt);

			textout_ex(virt,font_avalon2->dat,story_text[row], 0,row*25,makecol(alpha,alpha,alpha), -1);

			for(i=0;i<row;i++)
			{
				textout_ex(virt,font_avalon2->dat,story_text[i], 0,i*25,makecol(255,255,255), -1);
			}
			//textout(virt,font_avalon2->dat,text, x2,y2,makecol(alpha2,alpha2,alpha2));
	
			blit(virt,screen,0,0,80,0,480,480);
		}
	

		while(time<310 && !key[KEY_ESC])
		{
			
			while(speed_counter>0)
			{
			
				time++;
			
	
				speed_counter--;
			}
		
		}
	}
	
	end =0;

	alpha = 255;
	
	while(!end)
	{
		
		while(speed_counter>0)
		{
			
			time++;

			if(time>200)alpha-=0.5;

			if(alpha==0)end=1;
			
			if(alpha>255)alpha=255;
			if(alpha<0)alpha=0;
		
			
	
			speed_counter--;
		}
		
		clear(virt);

		textout_centre_ex(virt,font_arial->dat,"T H E   E N D",240,220,makecol(alpha,alpha,alpha), -1);

		blit(virt,screen,0,0,80,0,480,480);
	}


}


