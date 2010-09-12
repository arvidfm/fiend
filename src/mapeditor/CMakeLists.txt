add_executable(mapeditor ../bmp_array.c ../character.c ../console.c console_funcs.c d_area.c d_brush.c d_enemy.c d_global_trigger.c d_item.c d_light.c d_link.c d_lookat_area.c d_misc.c d_npc.c d_object.c 
d_pathnode.c d_sound.c d_trigger.c ../draw.c ../enemy.c ../fiend.c ../grafik4.c ../item.c ../lightmap.c mapdraw.c mapedit.c ../mapio.c menu_edit.c menu_file.c menu_misc.c menu_options.c ../NPC.c 
../object.c ../picdata.c ../rotate_sprite.c ../sound.c ../tile.c ../trigger.c)

target_link_libraries(mapeditor alleg fmodex)
