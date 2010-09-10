////////////////////////////////////////////////////
// This file contains the A* pathfinding code 
//
// all code by Thomas Grip 
///////////////////////////////////////////////////



#include <allegro.h>
#include <string.h>
#include <stdio.h>

#include "../fiend.h"
#include "../grafik4.h"

//max nodes on screen at ones.
#define MAX_SCREEN_NODES 40

//screen sixe for nodes.
#define NODE_SCREEN_H 480
#define NODE_SCREEN_W 480


typedef struct THE_NODE 
{
	int num;
	int cost;

	struct THE_NODE* parent;
}NODE;


static NODE *node[MAX_PATHNODE_NUM];
int h_list[MAX_PATHNODE_NUM];

static int node_num=0;

static int global_x;
static int global_y;

static int global_goal_x;
static int global_goal_y;


//check if you can move from start to goal without hitting anything...
static int free_path(int start_x, int start_y, int goal_x,int  goal_y,int w, int h, int check_player)
{
	int i;

	int x_add[5] = {0,-w/2+1,w/2-1,w/2-1,-w/2+1};
	int y_add[5] = {0,-h/2+1,-h/2+1,h/2-1,h/2-1};

	for(i=0;i<5;i++)
	{
		if(!path_is_clear(start_x+x_add[i],start_y+y_add[i],0,goal_x,goal_y,1,check_player))
			return 0;
	}
	
	return 1;
}


//make a new node
static int new_node(int index, int num, NODE *parent)
{
	int h;
	int cost;

	NODE *temp_n;
	
	if(parent==NULL)
		cost = distance(global_x, global_y, map->path_node[num].x,map->path_node[num].y);
	else
		cost = parent->cost + distance(map->path_node[num].x,map->path_node[num].y, map->path_node[parent->num].x,map->path_node[parent->num].y);
	
	h = cost + distance(map->path_node[num].x,map->path_node[num].y,global_goal_x, global_goal_y);
	
	if(h>=h_list[num]) return 0;		
	
	node[index] = (NODE*) malloc(sizeof(NODE));
	
	node[index]->num = num;

	if(parent==NULL)
	{
		node[index]->cost = cost;
		node[index]->parent = NULL;	
		h_list[num] = h;
	}
	else
	{
		node[index]->cost = cost;
		h_list[num] = h;
		
		while(parent->parent!=NULL)
		{
			temp_n = parent->parent;
			parent = temp_n;
		}

		temp_n = (NODE*) malloc(sizeof(NODE));

		temp_n->cost =parent->cost;
		temp_n->num =parent->num;
		temp_n->parent = NULL;

		node[index]->parent = temp_n;
	}
	
	return 1;
}

//delete a node
static void delete_node(NODE *n)
{
	NODE* temp_n;


	temp_n = n->parent;
	free(n);

	while(temp_n!=NULL)
	{
		n = temp_n;
		temp_n = n->parent;
		free(n);
	}
}

//sort the nodes so that the lowest costing is first (node_index == node_num-1)
static void sort_node_list(void)
{
	int i;
	int min_h=9999;
	int best_node;
	NODE * temp_n;

	if(node_num<2) return;

	for(i=0;i<node_num;i++)
	{
		if(h_list[node[i]->num]<min_h)
		{
			best_node = i;
			min_h = h_list[node[i]->num];
		}
	}

	temp_n = node[best_node];
	
	node[best_node] = node[node_num-1];
	node[node_num-1] = temp_n;
}


//finds the x,y of the pathnode that is best to walk at if you want to get to goal.
//returns -1 if you can't get to the goal.
int find_best_xy(int start_x,int  start_y, int goal_x,int goal_y,int w, int h, int *best_x, int *best_y,int check_player)
{
	int i;

	NODE *n;

	//init...
	global_x = start_x;
	global_y = start_y;
	global_goal_x = goal_x;
	global_goal_y = goal_y;

	for(i=0;i<map->num_of_path_nodes;i++)
		h_list[i] = 99999;

	//Check if you can walk straight to the goal
	if(free_path(start_x, start_y,goal_x,goal_y,w,h,check_player))
	{
		*best_x = goal_x;
		*best_y = goal_y;
		return 1;
	}

	//check what nodes you can see and put em in node[]
	for(i=0;i<map->num_of_path_nodes;i++)
	{
		if(check_collision(start_x-240, start_y-240,480,480, map->path_node[i].x,map->path_node[i].y,2,2))
			if(free_path(start_x, start_y,map->path_node[i].x,map->path_node[i].y,w,h,check_player))
			{
				new_node(node_num, i, NULL);
				node_num++;
			}
	}

	//if we have no nodes in list exit...
	if(node_num==0)return 0;

	//Sort the nodes...
	sort_node_list();

	while(1)
	{
		//if we have no nodes in list exit...
		if(node_num==0)return 0;
		
		//set active node..
		n = node[node_num-1];
		node_num--;

		//check if you can get from the current node to the goal, if so quit.
		if(free_path(map->path_node[n->num].x,map->path_node[n->num].y,goal_x, goal_y,w,h,check_player))
		{
			//if the node has any parents then the parent node is the one we want!
			if(n->parent==NULL)
			{
				*best_x = map->path_node[n->num].x;		
				*best_y = map->path_node[n->num].y;
			}
			else
			{
				*best_x = map->path_node[n->parent->num].x;		
				*best_y = map->path_node[n->parent->num].y;
			}
			
			delete_node(n);
			break;
		}

		
		//check wich nodes that you can get to from the current node.
		for(i=0;i<map->num_of_path_nodes;i++)
		{
			if(i != n->num)
				if(check_collision(map->path_node[n->num].x-240, map->path_node[n->num].y-240,480,480, map->path_node[i].x,map->path_node[i].y,2,2))
					if(free_path(map->path_node[n->num].x, map->path_node[n->num].y,map->path_node[i].x,map->path_node[i].y,w,h,check_player))
					{
						if(new_node(node_num, i, n))
							node_num++;
					}
		}

		delete_node(n);

		//Sort the nodes...
		sort_node_list();
		
	}
	
	//delete the nodes left
	for(i=0;i<node_num;i++)
		delete_node(node[i]);

	node_num=0;


	return 1;
}
