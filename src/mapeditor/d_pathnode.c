#include <allegro.h>

#include <stdio.h>

#include "../fiend.h"
#include "mapedit.h"

static char temp_string[180];

int current_pathnode=0;




//make a new item
void make_new_pathnode(int x, int y)
{
	int i;

	if(map->num_of_path_nodes==MAX_PATHNODE_NUM)
	{
		alert("No more pathnodes can be made!",NULL,NULL,"OK",NULL,0,0);
		return;
	}
	i = map->num_of_path_nodes;
	
	map->num_of_path_nodes++;

	map->path_node[i].used=1;
	
	map->path_node[i].x=x;
	map->path_node[i].y=y;
}




int delete_pathnode(void)
{
	int ans,i;

	ans = alert("Sure you want to delete the pathnode?",NULL,NULL,"Yes","Cancel",'\r',0);
	if(ans==2)return D_O_K;
	
	map->num_of_path_nodes--;

	for(i=current_pathnode;i<map->num_of_path_nodes;i++)
	{
		map->path_node[i].used=map->path_node[i+1].used;
		map->path_node[i].x=map->path_node[i+1].x;
		map->path_node[i].y=map->path_node[i+1].y;
	}
		
	return D_O_K;
}



int pathnode_menu(int num)
{
 int ans;
 MENU temp_menu[]=
 {
	 //text            proc    menu-child     flags      dp
	{ "Delete",  delete_pathnode,      NULL,          0,         NULL},
	{ NULL,         NULL,      NULL,          0,         NULL}
	};
	
	current_pathnode=num;	
	
    ans=do_menu(temp_menu,mouse_x,mouse_y);
	
	
	return D_REDRAW;
}






