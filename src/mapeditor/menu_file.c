#include <allegro.h>
#include <string.h>
#include "../fiend.h"
#include "mapedit.h"

char load_path[100];
char save_path[100];



//clear the current map and start up fresh...
int file_new_proc(void)
{
 int ans;
 ans = alert("Sure you want make a new map?",NULL,NULL,"Yes","Cancel",'\r',0);
 if(ans==1)
 {
  clear_map(map);
  replace_filename(save_path, save_path, "", 99);//no file chosen in savepath now...
  load_npc();//load the npc data
  load_enemy_data();//load the enemy data
  load_item_data();//load the enemy data
  load_global_vars();
  load_global_triggers();
     return D_REDRAW;
 }
 else
  return D_O_K;
}	




//Load a map
int file_load_map(void)
{
 int ans;
 ans = alert("Sure you want a load map?",NULL,NULL,"Yes","Cancel",'\r',0);// are you sure?
 if(ans==2)return D_O_K;

 ans = file_select("LOAD A MAP",load_path,"map");//select map
 if(ans!=0)
 {
  if(exists(load_path)==0) //check if the file exists
  {
    alert("could not load",get_filename(load_path)," ","OK",NULL,'\r',0); 
    return D_REDRAW;
  }

  load_npc();//load the npc info
  load_enemy_data();//load the enemy info
  load_item_data();//load the enemy info
  load_global_vars();
  load_global_triggers();
  load_edit_map(map,load_path);//load the map
  strcpy(save_path,load_path); //the save path becomes the same as load path
  replace_filename(load_path, load_path, "", 99);//get rid of the file name from load path
  return D_REDRAW;
 }
 else
  return D_REDRAW;
}




//save a map
int file_save_proc(void)
{
 int ans;
 char old_save_path[80];
 
 strcpy(old_save_path, save_path);

 ans = file_select("SAVE A MAP",save_path,"map");

 if(ans!=0)
 {
  if(exists(save_path) && strcmp(save_path, old_save_path)!=0)
    if(alert(get_filename(save_path),"Allready exists!","Overwrite?","OK","Cancel",0,0)==2)
	  return D_REDRAW;
  
    
  if(!save_map(map,save_path) || !save_npc() || !save_enemy_data()||!save_item_data() ||!save_global_vars() ||!save_global_triggers())
  {
   alert("could not save",get_filename(save_path)," ","OK",NULL,'\r',0);
  }
  return D_REDRAW;
 }
 else
  return D_REDRAW;
}	




//exit the dialog
int file_exit_proc(void)
{
 int ans;
 ans = alert("Sure you want to quit?",NULL,NULL,"Yes","Cancel",'\r',0);
 if(ans==1)
  return D_EXIT;
 else
  return D_O_K;

}
