////////////////////////////////////////////////////
// This file contains all the commands, vars and
// stuff for the console 
//
// Code by Pierre Renaux and Thomas Grip 
///////////////////////////////////////////////////

#include "../fiend.h"
#include "../console_funcs.h"

//===========================================================================
//    IMPLEMENTATION PRIVATE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//===========================================================================


//===========================================================================
//    IMPLEMENTATION PRIVATE STRUCTURES
//===========================================================================


//===========================================================================
//    IMPLEMENTATION REQUIRED EXTERNAL REFERENCES (AVOID)
//===========================================================================


//===========================================================================
//    IMPLEMENTATION PRIVATE FUNCTION PROTOTYPES
//===========================================================================
static int test(void);

//===========================================================================
//    IMPLEMENTATION PRIVATE DATA
//===========================================================================


//===========================================================================
//    IMPLEMENTATION PRIVATE FUNCTIONS
//===========================================================================


//---------------------------------------------------------------------------
// Name: save_game [filename]
// Desc: Test function to show how write a console function =)
//---------------------------------------------------------------------------
static int csl_save_game(void)
{
    int argc = csl_argc()+1;
	char path[60];

	    
	if(argc!=2)
	{
		csl_textout(1, "syntax: save_game [filename]");
	}
	else
	{
		sprintf(path,"save/%s",csl_argv(1));

		if(!save_game(path,"Console Save"))
		{
			csl_textoutf(1, "couldn't save game to %s",path);
            return CSLMSG_ERROR;
		}
		else
		{
			csl_textoutf(1, "saved game to %s",path );
		}

	}
		
	return CSLMSG_O_K;
    
}

//---------------------------------------------------------------------------
// Name: load_game [filename]
// Desc: Test function to show how write a console function =)
//---------------------------------------------------------------------------
static int csl_load_game(void)
{
    int argc = csl_argc()+1;
	char path[60];

	    
	if(argc!=2)
	{
		csl_textout(1, "syntax: save_game [filename]");
	}
	else
	{
		sprintf(path,"save/%s",csl_argv(1));

		if(!load_game(path))
		{
			csl_textoutf(1, "couldn't load game from %s",path);
            return CSLMSG_ERROR;
		}
		else
		{
			csl_textoutf(1, "loaded game from %s",path );
		}

	}
		
	return CSLMSG_CLOSE;
    
}

//---------------------------------------------------------------------------
// Name: lightning 
// Desc: Sets lightning is on var.
//---------------------------------------------------------------------------
static int csl_lightning(void)
{
    int argc = csl_argc()+1;
	    
	    
	if(argc==1)
	{
		csl_textout(1, "Lightning is set to \"1\".");
		lightning_is_on=1;
	}
	else
	{
		lightning_is_on = atoi(csl_argv(1));
		csl_textoutf(1, "Lightning is set to \"%d\".", lightning_is_on);
	}

		
	return CSLMSG_O_K;
}
//---------------------------------------------------------------------------
// Name: sound_volume 
// Desc: Sets sound volume.
//---------------------------------------------------------------------------
static int csl_sound_volume(void)
{
    int argc = csl_argc()+1;
	    
	    
	if(argc!=2)
	{
		csl_textout(1, "syntax: sound_volume [0 - 255]");
		csl_textoutf(1, "Current sound_volume is \"%d\".",fiend_sound_volume);
	}
	else
	{
		fiend_sound_volume = atoi(csl_argv(1));
		
		if(sound_is_on) {
			//FSOUND_SetSFXMasterVolume(fiend_sound_volume);
			FMOD_CHANNELGROUP *cgroup = NULL;
			FMOD_System_GetMasterChannelGroup(fmod_system, &cgroup);
			FMOD_ChannelGroup_SetVolume(cgroup, ((float)fiend_sound_volume)/256);
		}

		csl_textoutf(1, "Sound_volume is set to \"%d\".", fiend_sound_volume);
	}

		
	return CSLMSG_O_K;
    
}
//---------------------------------------------------------------------------
// Name: music_volume 
// Desc: Sets music volume.
//---------------------------------------------------------------------------
static int csl_music_volume(void)
{
    int argc = csl_argc()+1;
	    
	    
	if(argc!=2)
	{
		csl_textout(1, "syntax: music_volume [0 - 255]");
		csl_textoutf(1, "Current music_volume is \"%d\".",fiend_sound_volume);
	}
	else
	{
		fiend_music_volume = atoi(csl_argv(1));
		
		if(sound_is_on)
			set_fiend_music_volume(fiend_music_volume);

		csl_textoutf(1, "Music_volume is set to \"%d\".", fiend_music_volume);
	}

		
	return CSLMSG_O_K;
    
}

//---------------------------------------------------------------------------
// Name: vsync 
// Desc: Sets vsync is on var.
//---------------------------------------------------------------------------
static int csl_vsync(void)
{
    int argc = csl_argc()+1;
	    
	    
	if(argc==1)
	{
		csl_textout(1, "Vsync is set to \"1\".");
		vsync_is_on=1;
	}
	else
	{
		vsync_is_on = atoi(csl_argv(1));
		csl_textoutf(1, "Vsync is set to \"%d\".", vsync_is_on);
	}

		
	return CSLMSG_O_K; 
}

//---------------------------------------------------------------------------
// Name: help 
// Desc: Shows somw help
//---------------------------------------------------------------------------
static int csl_help(void)
{
    int argc = csl_argc()+1;
	
	csl_textoutf(1, "No help yet!");


	return CSLMSG_O_K;    
}




//===========================================================================
//    INTERFACE DATA
//===========================================================================
//===========================================================================
//    INTERFACE FUNCTIONS
//===========================================================================


//---------------------------------------------------------------------------
// Name: draw_csl_dbuff()
// Desc: Show the console buffer.
//---------------------------------------------------------------------------
void draw_csl_dbuff(BITMAP *csl_dbuff, int csl_y)
{
    acquire_screen();
    if((csl_get_y() != csl_y) && (in_the_game))
    {
        draw_level();
        blit(csl_dbuff, virt, 0, csl_dbuff->h-csl_y, 0, 0, csl_dbuff->w, csl_y);
        blit(virt, screen, 0,0,80,0,480,480);
    }
    else
    {
        blit(csl_dbuff, screen, 0, csl_dbuff->h-csl_y, 80, 0, csl_dbuff->w, csl_y);
    }
    release_screen();
}



//---------------------------------------------------------------------------
// Name: register_console_funcs_and_vars()
// Desc: Register the console functions and variables
//---------------------------------------------------------------------------
void register_console_funcs_and_vars(void)
{
	csl_add_func("save_game", csl_save_game);
	csl_add_func("load_game", csl_load_game);
	csl_add_func("lightning", csl_lightning);
	csl_add_func("help", csl_help);
	csl_add_func("sound_volume", csl_sound_volume);
	csl_add_func("music_volume", csl_music_volume);
	csl_add_func("vsync", csl_vsync);
	
}

//***************************************************************************
//**
//**    END MODULE CONSOLE_FUNCS.C
//**
//***************************************************************************
