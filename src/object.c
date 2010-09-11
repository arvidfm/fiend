////////////////////////////////////////////////////
// This file contains loading and drawing
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <stdio.h>

#include "grafik4.h"
#include "object.h"
#include "fiend.h"
#include "draw.h"
#include "rotate_sprite.h"


#define MAX_OBJECTS 300

//Info
OBJECT_INFO *object_info;

//variables
int num_of_objects=0;

////////////////////////////////////////
///////// UPDATIING /////////////////////
/////////////////////////////////////////



////////////////////////////////////////
///////// DRAWING /////////////////////
/////////////////////////////////////////


void draw_fiend_object(BITMAP *dest,OBJECT_INFO *temp,int x, int y,int action,int frame, float angle)
{
	int num = temp->animation[action].frame[frame];

	float angle_add,the_angle;
	float x_add, y_add;
	float d_x,d_y;
	float length;
	float new_x=0;
	float new_y=0;
	int pic;

	if(!temp->door)//the object is normal obejct
	{
		
		if(temp->angles)//rotate it
		{
			if(temp->angles == 1)
			{
				pic = ((int)angle)/90;
				if(pic == 4) pic =0;
			}
			else
			{
				pic = ((int)angle)/45;
				if(pic == 8) pic =0;
			}

			if(temp->additive)
			{
				draw_additive_sprite(dest, temp->pic[num][pic].data, x-temp->pic[num][pic].data->w/2, y-temp->pic[num][pic].data->h/2);
			}
			else if(temp->trans)
			{
				set_trans_blender(0,0,0,temp->trans);
				draw_trans_rle_sprite(dest, temp->rle_pic[num][pic],x-temp->rle_pic[num][pic]->w/2,y-temp->rle_pic[num][pic]->h/2);
			}
			else
			{
				draw_rle_sprite(dest, temp->rle_pic[num][pic], x-temp->rle_pic[num][pic]->w/2,y-temp->rle_pic[num][pic]->h/2);
			}
			
		}
		else //just draw it
		{
			if(temp->additive)
			{
				draw_additive_sprite(dest, temp->pic[num][0].data,x-temp->pic[num][0].data->w/2,y-temp->pic[num][0].data->h/2);
			}
			else if(temp->trans)
			{
				set_trans_blender(0,0,0,temp->trans);
				draw_trans_sprite(dest, temp->pic[num][0].data,x-temp->pic[num][0].data->w/2,y-temp->pic[num][0].data->h/2);
			}
			else
			{
				draw_sprite(dest, temp->pic[num][0].data,x-temp->pic[num][0].data->w/2,y-temp->pic[num][0].data->h/2);
			}
		}
	}
	else //the object is a door
	{
		//the normal doors
		if(temp->door==1 || temp->door==2 || temp->door==3 || temp->door==4)
		{
			num = 0;

			//allegro_message("%d",(int)angle_add);

			if(temp->door==1)
				angle_add = ((float)frame/100)*180;
			else if(temp->door==2)
			{
				angle_add = ((float)frame/100)*180;
				if(angle_add>90)angle_add=90;
			}
			else if(temp->door==3)
			{
				angle_add = ((float)frame/100)*-180;
				
			}
			else if(temp->door==4)
			{
				angle_add = ((float)frame/100)*-180;
				if(angle_add<-90)angle_add=-90;
			}
			
			
			//sqrt(temp->door_x*temp->door_x+temp->door_y*temp->door_y);

			if(temp->door==1 || temp->door==2)
			{
				the_angle = add_angle(angle,angle_add);
			
				d_x = (temp->door_x*COS(angle)+temp->door_y*SIN(angle));
				d_y = (temp->door_y*COS(angle)-temp->door_x*SIN(angle));

				x_add = (d_x*COS(angle_add)+d_y*SIN(angle_add))-d_x;
				y_add = (d_y*COS(angle_add)-d_x*SIN(angle_add))-d_y;
			}
			else
			{
				angle_add = add_angle(0, angle_add);
				
				the_angle = add_angle(angle,angle_add);
								
				d_x = (temp->door_x*COS(angle)+temp->door_y*SIN(angle));
				d_y = (temp->door_y*COS(angle)-temp->door_x*SIN(angle));

				x_add = (d_x*COS(angle_add)+d_y*SIN(angle_add))-d_x;
				y_add = (d_y*COS(angle_add)-d_x*SIN(angle_add))-d_y;
			}

			if(temp->additive)
				rotate_fiend_sprite(dest, temp->pic[num][0].data,x-x_add,y+y_add,the_angle,FIEND_DRAW_MODE_ADDITIVE);
			else
				rotate_sprite(dest, temp->pic[num][0].data,x-x_add-temp->pic[num][0].data->w/2,y+y_add-temp->pic[num][0].data->h/2,degree_to_fixed(the_angle));//FIEND_DRAW_MODE_TRANS);
		}
		
		//the sliding door
		if(temp->door==5)
		{
			num=0;
			
			length =((float)frame/100)*temp->w;

			xyplus(length,add_angle(angle,90),&new_x,&new_y);
			
			if(temp->additive)
				rotate_fiend_sprite(dest, temp->pic[num][0].data,x+new_x,y+new_y,angle,FIEND_DRAW_MODE_ADDITIVE);
			else
				rotate_fiend_sprite(dest, temp->pic[num][0].data,x+new_x,y+new_y,angle,FIEND_DRAW_MODE_TRANS);

		}



	}
}





/////////////////////////////////////
///////// LOADING AND SAVING /////////
///////////////////////////////////////


//Load object graphics
int load_objects(void)
{
	
	BMP_ARRAY *temp_data;
	BITMAP *bmp;
	
	FILE *f;

	char buffer[50];
	char sprite_file_name[300][30];
    char info_file_name[300][30];
	char *file_path ="graphic/objects/";
	char final_path[40];
	int i,j,k,temp;
	
	object_info = calloc(sizeof(OBJECT_INFO),MAX_OBJECTS);
	
	f = fopen("graphic/objects/objects.txt", "r");      //Load the Info file
	
	if(f==NULL)
	{sprintf(fiend_errorcode,"couldn't load objects.txt");return 1;} //error testing...
	
	
	while(fscanf(f, "%s %s\n", sprite_file_name[num_of_objects], info_file_name[num_of_objects])!=EOF)
	{
		num_of_objects++;	
	}

	fclose(f);

	
	for(i=0;i<num_of_objects;i++)
	{
		
		//----------BEGIN GET INFO-----------------------
		strcpy(final_path, file_path);
		strcat(final_path, info_file_name[i]);    //Get the info.

		f = fopen(final_path, "r");
		if(f==NULL){sprintf(fiend_errorcode,"couldn't load %s",final_path); return 1;}//error testing...

		
		fscanf(f,"%s %d",buffer,&object_info[i].num_of_frames);//num of frames
				
				  
		fscanf(f,"%s %s\n",buffer, object_info[i].name);//get the name

		//get the object type
		fscanf(f,"%s %d %s %d\n", buffer, &object_info[i].can_be_killed, buffer, &object_info[i].can_be_pushed);
		
		//get the object type
		fscanf(f,"%s %s\n", buffer, &object_info[i].folder);
		
		//get witdh, height, number of angles, if solid and energy 
		fscanf(f,"%s %d %s %d %s %d %s %d %s %d\n",buffer, &object_info[i].w, buffer, &object_info[i].h, buffer, &object_info[i].angles, buffer, &object_info[i].solid, buffer, &object_info[i].energy);

		fscanf(f,"%s %d %s %d %s %d\n",buffer, &object_info[i].door, buffer, &object_info[i].door_x, buffer, &object_info[i].door_y);

		fscanf(f,"%s %d %s %d\n",buffer, &object_info[i].additive, buffer, &object_info[i].trans);

		//get the number of animations
		fscanf(f,"%s %d\n",buffer, &object_info[i].num_of_animations);
		
		//read the animation in fo first you get the name of the animation
		//then you read the frames til a '-1' is encounterd
		for(j=0;j<object_info[i].num_of_animations;j++)
		{
			fscanf(f,"%s %d %s %s %s %s %s %d\n",buffer,&object_info[i].animation[j].solid, buffer, object_info[i].animation[j].name, buffer, object_info[i].animation[j].sound, buffer, &object_info[i].animation[j].loop_sound);
			k=-1;
			do
			{
				k++;
				fscanf(f,"%d ",&object_info[i].animation[j].frame[k],buffer);
			}while(object_info[i].animation[j].frame[k]!=-1);
			
		}

		fclose(f);
		//-------END GET INFO---------------

		
		
		//--------BEGIN GET DATA------------------
		strcpy(final_path, file_path);
		strcat(final_path, sprite_file_name[i]);   //Get the tile data .

														 
	    temp_data = load_bmp_array(final_path,object_info[i].num_of_frames);//load the graphic
		if(temp_data==NULL){ sprintf(fiend_errorcode,"couldn't load %s",final_path); return 1;}//error testing...
		
		
		for(j=0;j<object_info[i].num_of_frames;j++)//For each of the diffrent pics....
		{
			//if the object is not a normal object just crop the data...
			if(!object_info[i].angles || object_info[i].door)
			{
				crop_picdata(temp_data[j].dat, &object_info[i].pic[j][0]);
			}
			else if(object_info[i].additive)
			{

				//make a large bitmap to fit the roated bitmap
				temp = sqrt(temp_data[j].dat->w*temp_data[j].dat->w + temp_data[j].dat->h*temp_data[j].dat->h);
				bmp = create_bitmap(temp, temp);
				
				if(object_info[i].angles ==1)
				{
					for(k=0;k<4;k++)
					{
						clear_to_color(bmp,makecol(0,0,0));

						rotate_sprite(bmp,temp_data[j].dat,temp/2-temp_data[j].dat->w/2,temp/2-temp_data[j].dat->h/2,degree_to_fixed(k*90));
						crop_picdata(bmp, &object_info[i].pic[j][k]);
					}
					destroy_bitmap(bmp);
				}
				else
				{
					for(k=0;k<8;k++)
					{
						clear_to_color(bmp,makecol(0,0,0));

						rotate_sprite(bmp,temp_data[j].dat,temp/2-temp_data[j].dat->w/2,temp/2-temp_data[j].dat->h/2,degree_to_fixed(k*45));
						crop_picdata(bmp, &object_info[i].pic[j][k]);
					}
					destroy_bitmap(bmp);
				}

			}
			else//if normal do some calcs
			{
				//make a large bitmap to fit the roated bitmap
				temp = sqrt(temp_data[j].dat->w*temp_data[j].dat->w + temp_data[j].dat->h*temp_data[j].dat->h);
				bmp = create_bitmap(temp, temp);
				
				if(object_info[i].angles ==1)
				{
					for(k=0;k<4;k++)
					{
						clear_to_color(bmp,makecol(255,0,255));

						rotate_sprite(bmp,temp_data[j].dat,temp/2-temp_data[j].dat->w/2,temp/2-temp_data[j].dat->h/2,degree_to_fixed(k*90));
						crop_picdata(bmp, &object_info[i].pic[j][k]);

						object_info[i].rle_pic[j][k] = get_rle_sprite(object_info[i].pic[j][k].data);
						destroy_bitmap(object_info[i].pic[j][k].data);
					}
					destroy_bitmap(bmp);
				}
				else
				{
					for(k=0;k<8;k++)
					{
						clear_to_color(bmp,makecol(255,0,255));

						rotate_sprite(bmp,temp_data[j].dat,temp/2-temp_data[j].dat->w/2,temp/2-temp_data[j].dat->h/2,degree_to_fixed(k*45));
						crop_picdata(bmp, &object_info[i].pic[j][k]);
						
						object_info[i].rle_pic[j][k] = get_rle_sprite(object_info[i].pic[j][k].data);
						destroy_bitmap(object_info[i].pic[j][k].data);
					}
					destroy_bitmap(bmp);
				}

			}
		}

		unload_bmp_array(temp_data);

				
		//----------END GET DATA-------------------
	
	}

	
	return 0;
}





//release all the objects from memory
void release_objects(void)
{
	int i,j,k;

	for(i=0;i<num_of_characters;i++)
		for(j=0;j<object_info[i].num_of_frames;j++)
			if(object_info[i].additive || object_info[i].trans || !object_info[i].angles || object_info[i].door)
			{
				destroy_bitmap(object_info[i].pic[j][0].data);
			}
			else
			{
				for(k=0;k<8;k++)
					destroy_rle_sprite(object_info[i].rle_pic[j][k]);
			}
					
	free(object_info);
}





