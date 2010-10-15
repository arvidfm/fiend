add_executable(fiend WIN32 main.c ai.c astar.c ../bmp_array.c ../character.c collision.c ../console.c console_funcs.c ../draw.c draw_level.c ../draw_polygon.c effect.c ../enemy.c enemy_update.c ../fiend.c
		../grafik4.c intro.c inventory.c ../item.c ../lightmap.c link.c los.c ../mapio.c menu.c message.c ../misc.c missile.c notes.c ../NPC.c npc_update.c ../object.c particle.c ../picdata.c
		player.c ../rotate_sprite.c save_menu.c savegame.c ../sound.c soundplay.c ../tile.c ../trigger.c trigger_cond.c trigger_event.c trigger_update.c update.c weapon.c)

target_link_libraries(fiend ${ALLEGRO_LIBRARY} ${FMOD_LIBRARY})
