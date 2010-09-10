//***************************************************************************
//**
//** Alias25 (C) 2000
//**
//** File: CONSOLE_FUNCS.C
//**
//** Date: 6/12/00
//**
//** Author: Pierre Renaux
//**
//** Desc: Console functions.
//**
//***************************************************************************
//===========================================================================
//    IMPLEMENTATION HEADERS
//===========================================================================
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
// Name: test()
// Desc: Test function to show how write a console function =)
//---------------------------------------------------------------------------
static int test(void)
{
    int i, argc = csl_argc();

    // Add text to the console, the 1st parameter is the number of new line
    // code to insert BEFORE the text (this is due to the text up to down
    // drawing). So to insert a blank line you must set it to 2.
    csl_textout(1, "--- Test Function ---");

    csl_textoutf(2, "ArgC: %d", argc);

    for(i = 0; i < argc; i++)
        csl_textoutf(1, "Argv[%d]: %s", i, csl_argv(i));


    csl_textout(2, "---------------------");

    // Return default value, all is right =)
    return CSLMSG_O_K;

    // return CSLMSG_CLOSE to close the console
    //        CSLMSG_QUIT  to make csl_start returning CSLMSG_QUIT
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
    blit(csl_dbuff, screen, 0, csl_dbuff->h-csl_y, 80, 0, csl_dbuff->w, csl_y);
    release_screen();
}

//---------------------------------------------------------------------------
// Name: register_console_funcs_and_vars()
// Desc: Register the console functions and variables
//---------------------------------------------------------------------------
void register_console_funcs_and_vars(void)
{
    csl_add_func("test", test);
}

//***************************************************************************
//**
//**    END MODULE CONSOLE_FUNCS.C
//**
//***************************************************************************
