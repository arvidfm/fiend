# Microsoft Developer Studio Project File - Name="mapeditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=mapeditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mapeditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mapeditor.mak" CFG="mapeditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mapeditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "mapeditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mapeditor - Win32 Release"

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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib alleg.lib fmodvc.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "mapeditor - Win32 Debug"

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
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib alld.lib fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "mapeditor - Win32 Release"
# Name "mapeditor - Win32 Debug"
# Begin Source File

SOURCE=..\bmp_array.c
# End Source File
# Begin Source File

SOURCE=..\character.c
# End Source File
# Begin Source File

SOURCE=..\console.c
# End Source File
# Begin Source File

SOURCE=.\console_funcs.c
# End Source File
# Begin Source File

SOURCE=.\d_area.c
# End Source File
# Begin Source File

SOURCE=.\d_brush.c
# End Source File
# Begin Source File

SOURCE=.\d_enemy.c
# End Source File
# Begin Source File

SOURCE=.\d_global_trigger.c
# End Source File
# Begin Source File

SOURCE=.\d_item.c
# End Source File
# Begin Source File

SOURCE=.\d_light.c
# End Source File
# Begin Source File

SOURCE=.\d_link.c
# End Source File
# Begin Source File

SOURCE=.\d_lookat_area.c
# End Source File
# Begin Source File

SOURCE=.\d_misc.c
# End Source File
# Begin Source File

SOURCE=.\d_npc.c
# End Source File
# Begin Source File

SOURCE=.\d_object.c
# End Source File
# Begin Source File

SOURCE=.\d_pathnode.c
# End Source File
# Begin Source File

SOURCE=.\d_sound.c
# End Source File
# Begin Source File

SOURCE=.\d_trigger.c
# End Source File
# Begin Source File

SOURCE=..\draw.c
# End Source File
# Begin Source File

SOURCE=..\enemy.c
# End Source File
# Begin Source File

SOURCE=..\fiend.c
# End Source File
# Begin Source File

SOURCE=..\grafik4.c
# End Source File
# Begin Source File

SOURCE=..\item.c
# End Source File
# Begin Source File

SOURCE=..\lightmap.c
# End Source File
# Begin Source File

SOURCE=.\mapdraw.c
# End Source File
# Begin Source File

SOURCE=.\mapedit.c
# End Source File
# Begin Source File

SOURCE=..\mapio.c
# End Source File
# Begin Source File

SOURCE=.\menu_edit.c
# End Source File
# Begin Source File

SOURCE=.\menu_file.c
# End Source File
# Begin Source File

SOURCE=.\menu_misc.c
# End Source File
# Begin Source File

SOURCE=.\menu_options.c
# End Source File
# Begin Source File

SOURCE=..\NPC.c
# End Source File
# Begin Source File

SOURCE=..\object.c
# End Source File
# Begin Source File

SOURCE=..\picdata.c
# End Source File
# Begin Source File

SOURCE=..\rotate_sprite.c
# End Source File
# Begin Source File

SOURCE=..\sound.c
# End Source File
# Begin Source File

SOURCE=..\tile.c
# End Source File
# Begin Source File

SOURCE=..\trigger.c
# End Source File
# End Target
# End Project
