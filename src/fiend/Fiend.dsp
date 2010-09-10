# Microsoft Developer Studio Project File - Name="Fiend" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Fiend - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Fiend.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Fiend.mak" CFG="Fiend - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Fiend - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Fiend - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Fiend - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 fmodvc.lib alleg.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "Fiend - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 fmodvc.lib alld.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Fiend - Win32 Release"
# Name "Fiend - Win32 Debug"
# Begin Source File

SOURCE=.\ai.c
# End Source File
# Begin Source File

SOURCE=.\astar.c
# End Source File
# Begin Source File

SOURCE=..\bmp_array.c
# End Source File
# Begin Source File

SOURCE=..\character.c
# End Source File
# Begin Source File

SOURCE=.\collision.c
# End Source File
# Begin Source File

SOURCE=..\console.c
# End Source File
# Begin Source File

SOURCE=.\console_funcs.c
# End Source File
# Begin Source File

SOURCE=..\draw.c
# End Source File
# Begin Source File

SOURCE=.\draw_level.c
# End Source File
# Begin Source File

SOURCE=..\draw_polygon.c
# End Source File
# Begin Source File

SOURCE=.\effect.c
# End Source File
# Begin Source File

SOURCE=..\enemy.c
# End Source File
# Begin Source File

SOURCE=.\enemy_update.c
# End Source File
# Begin Source File

SOURCE=..\fiend.c
# End Source File
# Begin Source File

SOURCE=.\fiend.ICO
# End Source File
# Begin Source File

SOURCE=..\grafik4.c
# End Source File
# Begin Source File

SOURCE=.\intro.c
# End Source File
# Begin Source File

SOURCE=.\inventory.c
# End Source File
# Begin Source File

SOURCE=..\item.c
# End Source File
# Begin Source File

SOURCE=..\lightmap.c
# End Source File
# Begin Source File

SOURCE=.\link.c
# End Source File
# Begin Source File

SOURCE=.\los.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=..\mapio.c
# End Source File
# Begin Source File

SOURCE=.\menu.c
# End Source File
# Begin Source File

SOURCE=.\message.c
# End Source File
# Begin Source File

SOURCE=..\misc.c
# End Source File
# Begin Source File

SOURCE=.\missile.c
# End Source File
# Begin Source File

SOURCE=.\notes.c
# End Source File
# Begin Source File

SOURCE=..\NPC.c
# End Source File
# Begin Source File

SOURCE=.\npc_update.c
# End Source File
# Begin Source File

SOURCE=..\object.c
# End Source File
# Begin Source File

SOURCE=.\particle.c
# End Source File
# Begin Source File

SOURCE=..\picdata.c
# End Source File
# Begin Source File

SOURCE=.\player.c
# End Source File
# Begin Source File

SOURCE=..\rotate_sprite.c
# End Source File
# Begin Source File

SOURCE=.\save_menu.c
# End Source File
# Begin Source File

SOURCE=.\savegame.c
# End Source File
# Begin Source File

SOURCE=..\sound.c
# End Source File
# Begin Source File

SOURCE=.\soundplay.c
# End Source File
# Begin Source File

SOURCE=..\tile.c
# End Source File
# Begin Source File

SOURCE=..\trigger.c
# End Source File
# Begin Source File

SOURCE=.\trigger_cond.c
# End Source File
# Begin Source File

SOURCE=.\trigger_event.c
# End Source File
# Begin Source File

SOURCE=.\trigger_update.c
# End Source File
# Begin Source File

SOURCE=.\update.c
# End Source File
# Begin Source File

SOURCE=.\weapon.c
# End Source File
# End Target
# End Project
