////////////////////////////////////////////////////
// This file contains trigger update and small trigger funcs
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <stdio.h>
#include <string.h>

#include "../fiend.h"
#include "../grafik4.h"


extern int check_cond(int,int,int);
extern int check_event(int,int,int);

extern int condition_used_something;
extern int condition_item_something;


int with_sound=0;

static char error_string[50];

////////////////////////////////////////////////
//////////////// TRIGGER STUFF ////////////////
///////////////////////////////////////////////


//get the value of an x,y,z variable
//the string can be formated in many ways
//if there is an & before as first character...
//you must use small charcers...
//the "commands:"
//
//lvar [name]				local variable
//gvar [name]				global variable
//enemy [name] [prop]		enemy, prop can be x,y,energy, angle,action
//npc [name] [prop]			npc, prop can be x,y,energy, angle,action
//object [name] [prop]		object, prop can be x,y,energy, angle,action
//player [prop]				player, prop can be x,y,energy,angle
//rnd [x] [y]				generates a random number between x and y
//
//After that you can also wite an operator (+,-,/ or *) and number
//for example: &enemy peter x + 20

int get_var_value(char *string)
{
	char temp_string[40],temp_string2[40];
	char buffer[40];
	int value=0,x,y;

	int arg_num =0;
	int operator_ok = 0;
	
	if(string[0] != '&')
		return atoi(string);

	sscanf(string,"&%s",temp_string);

	if(strcmp(temp_string,"lvar")==0)
	{
		sscanf(string,"%s %s",buffer, temp_string);
		x = get_local_var_num(temp_string);

		if(x<0)return -1;

		value = map->var[x].value;

		arg_num =2;
	}
	else if(strcmp(temp_string,"gvar")==0)
	{
		sscanf(string,"%s %s",buffer, temp_string);
		x = get_global_var_num(temp_string);

		if(x<0)return -1;

		value = global_var[x].value;

		arg_num =2;
	}
	else if(strcmp(temp_string,"rnd")==0)
	{
		sscanf(string,"%s %d %d",buffer, &x, &y);

		value = RANDOM(x,y);

		arg_num =3;
	}
	else if(strcmp(temp_string,"enemy")==0)
	{
		sscanf(string,"%s %s %s",buffer, temp_string, temp_string2);
		x = get_enemy_num(temp_string);

		if(strcmp(temp_string2,"x")==0)
			value = enemy_data[x].x;
		else if(strcmp(temp_string2,"y")==0)
			value = enemy_data[x].y;
		else if(strcmp(temp_string2,"energy")==0)
			value = enemy_data[x].energy;
		else if(strcmp(temp_string2,"angle")==0)
			value = enemy_data[x].angle;

		arg_num =3;
	}
	else if(strcmp(temp_string,"npc")==0)
	{
		sscanf(string,"%s %s %s",buffer, temp_string, temp_string2);
		x = get_npc_num(temp_string);

		if(strcmp(temp_string2,"x")==0)
			value = npc_data[x].x;
		else if(strcmp(temp_string2,"y")==0)
			value = npc_data[x].y;
		else if(strcmp(temp_string2,"energy")==0)
			value = npc_data[x].energy;
		else if(strcmp(temp_string2,"angle")==0)
			value = npc_data[x].angle;

		arg_num =3;
	}
	else if(strcmp(temp_string,"player")==0)
	{
		sscanf(string,"%s %s",buffer, temp_string2);
		
		if(strcmp(temp_string2,"x")==0)
			value = player.x;
		else if(strcmp(temp_string2,"y")==0)
			value = player.y;
		else if(strcmp(temp_string2,"energy")==0)
			value = player.energy;
		else if(strcmp(temp_string2,"angle")==0)
			value = player.angle;

		arg_num =2;
	}
	else if(strcmp(temp_string,"object")==0)
	{
		sscanf(string,"%s %s %s",buffer, temp_string, temp_string2);
		x = get_object_num(temp_string);

		if(strcmp(temp_string2,"x")==0)
			value = map->object[x].x;
		else if(strcmp(temp_string2,"y")==0)
			value = map->object[x].y;
		else if(strcmp(temp_string2,"energy")==0)
			value = map->object[x].energy;
		else if(strcmp(temp_string2,"angle")==0)
			value = map->object[x].angle;

		arg_num =3;
	}


	if(arg_num==2)
		if(sscanf(string,"%s %s %s %d",buffer, buffer, temp_string,&x)==4)operator_ok=1;
	if(arg_num==3)
		if(sscanf(string,"%s %s %s %s %d",buffer, buffer, temp_string,&x)==5)operator_ok=1;

	if(operator_ok)
	{
		if(strcmp(temp_string,"+")==0)
			value+=x;
		else if(strcmp(temp_string,"-")==0)
			value-=x;
		else if(strcmp(temp_string,"*")==0)
			value*=x;
		else if(strcmp(temp_string,"/")==0)
			value/=x;
	}
	
	return value;
}





int get_object_num(char *name)
{
	int i, num=-1;

	for(i=0;i<map->num_of_objects;i++)
	{
		if(strcmp(map->object[i].name, name)==0)
		{
			num=i;
			break;
		}
	}

	if(num<0)
	{
		sprintf(error_string,"Script error: Object %s does not exist",name);
		make_engine_error(error_string);
	}

	return num;
}


int get_area_num(char *name)
{
	int i, num=-1;

	for(i=0;i<map->num_of_areas;i++)
	{
		if(strcmp(map->area[i].name, name)==0)
		{
			num=i;
			break;
		}
	}

	if(num<0)
	{
		sprintf(error_string,"Script error: Area \"%s\" does not exist",name);
		make_engine_error(error_string);
	}

	return num;
}



int get_enemy_num(char *name)
{
	int i, num=-1;

	for(i=0;i<MAX_ENEMY_DATA;i++)
	{
		if(enemy_data[i].used)
			if(strcmp(enemy_data[i].name, name)==0)
			{
				num=i;
				break;
			}
	}
	
	if(num<0)
	{
		sprintf(error_string,"Script error: Enemy \"%s\" does not exist",name);
		make_engine_error(error_string);
	}

	return num;
}

int get_npc_num(char *name)
{
	int i, num=-1;

	for(i=0;i<MAX_NPC_NUM;i++)
	{
		if(npc_data[i].used)
			if(strcmp(npc_data[i].name, name)==0)
			{
				num=i;
				break;
			}
	}

	if(num<0)
	{
		sprintf(error_string,"Script error: NPC \"%s\" does not exist",name);
		make_engine_error(error_string);
	}


	return num;
}

int get_item_num(char *name)
{
	int i, num=-1;

	for(i=0;i<MAX_ITEM_DATA;i++)
	{
		if(strcmp(item_data[i].name, name)==0)
		{
			num=i;
			break;
		}
	}
	
	if(num<0)
	{
		sprintf(error_string,"Script error: Item \"%s\" does not exist",name);
		make_engine_error(error_string);
	}

	return num;
}


int get_global_var_num(char *name)
{
	int i, num=-1;

	for(i=0;i<GLOBAL_VAR_NUM;i++)
	{
		if(strcmp(global_var[i].name, name)==0)
		{
			num=i;
			break;
		}
	}
	
	if(num<0)
	{
		sprintf(error_string,"Script error: Global var \"%s\" does not exist",name);
		make_engine_error(error_string);
	}

	return num;
}

int get_local_var_num(char *name)
{
	int i, num=-1;

	for(i=0;i<LOCAL_VAR_NUM;i++)
	{
		if(strcmp(map->var[i].name, name)==0)
		{
			num=i;
			break;
		}
	}
	
	if(num<0)
	{
		sprintf(error_string,"Script error: Local var \"%s\" does not exist",name);
		make_engine_error(error_string);
	}

	return num;
}







//Check the triggers. If type 0 = only att start, type 1 and higher = once and always triggers
void check_triggers(int type)
{
	int i,j,v;
	int condition_true=0;
	

	//text_mode(0);
	//textprintf(screen,font,80,30,makecol(255,255,255),"%d",type);
		
	//THE LOCAL TRIGGERS FIRST
	for(i=0;i<map->num_of_triggers;i++)
	{
		if( ((type==0 && map->trigger[i].type==0) || (type>0 && map->trigger[i].type>0)) && map->trigger[i].active)
		{
			if(type==0)with_sound=0;
			else with_sound=1;

			condition_used_something=0;
			condition_item_something=0;
			
				
			// BEGIN --check the condition			
			for(j=0;j<MAX_CONDITION_NUM;j++)
			{
				if(map->trigger[i].condition[j].used)
				{
					v = check_cond(i,j,0);
					if(v>0)
						condition_true=1;
					else if(v==0)
						goto trigger_end;//jump to end
					
					if(v<0)
					{
						map->trigger[i].active=0;
						goto trigger_end;
					}
						

				}
			}
			if(!condition_true)continue;
			//END --check the condition

			
			
			//BEGIN --make the events
			for(j=0;j<MAX_EVENT_NUM;j++)
			{
				if(map->trigger[i].event[j].used)
				{
					v = check_event(i,j,0);

					if(v<0)
					{
						map->trigger[i].active=0;
						goto trigger_end;
					}
					
				}
			}
			//END --making events

			//Make trigger not functional if it is check once or check startup type....
			if(map->trigger[i].type<2)map->trigger[i].active=0;
			
						
			//Check if something has been used. If so nothing can be used
			if(condition_used_something)player_has_used=0;
			if(condition_item_something)player_has_used_item=0;
			
			//reset the true thing...
			trigger_end: condition_true=0;
			}
	}
	
	condition_true=0;
	//THEN THE GLOBAL TRIGGERS 
	for(i=0;i<GLOBAL_TRIGGER_NUM;i++)
	{
		if( ((type==0 && global_trigger[i].type==0) || (type>0 && global_trigger[i].type>0)) && global_trigger[i].active)
		{
			if(type==0)with_sound=0;
			else with_sound=1;

			condition_used_something=0;
			condition_item_something=0;
			
				
			// BEGIN --check the condition			
			for(j=0;j<MAX_CONDITION_NUM;j++)
			{
				if(global_trigger[i].condition[j].used)
				{
					v = check_cond(i,j,1);
					if(v>0)
						condition_true=1;
					else if(v==0)
						goto trigger_end2;//jump to end
					
					if(v<0)
					{
						global_trigger[i].active=0;
						goto trigger_end2;
					}
						

				}
			}
			if(!condition_true)continue;
			//END --check the condition

			
			
			//BEGIN --make the events
			for(j=0;j<MAX_EVENT_NUM;j++)
			{
				if(global_trigger[i].event[j].used)
				{
					v = check_event(i,j,1);

					if(v<0)
					{
						global_trigger[i].active=0;
						goto trigger_end2;
					}
				}
			}
			//END --making events

			//Make trigger not functional if it is check once or check startup type....
			if(global_trigger[i].type<2)global_trigger[i].active=0;
			
			
			//Check if something has been used. IF so nothing can be used
			if(condition_used_something)player_has_used=0;
			if(condition_item_something)player_has_used_item=0;
			
			//reset the true thing...
			trigger_end2: condition_true=0;
			}
	}


	if(player_has_used_item)
	{
		i = RANDOM(0, 5);

		if(i==0)
			make_new_message("You can't use this item here.",NULL);
		else if(i==1)
			make_new_message("You can't figure out any logical way to use this item here.",NULL);
		else if(i==2)
			make_new_message("This item can't be used here.",NULL);
		else if(i==3)
			make_new_message("Using this item here is not possible.",NULL);
		else if(i==4)
			make_new_message("This item has no use here.",NULL);
		else if(i==5)
			make_new_message("You can't see how this item can be used here.",NULL);
	}
			

}


