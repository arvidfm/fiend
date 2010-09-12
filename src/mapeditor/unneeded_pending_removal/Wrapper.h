/* Yuck!  Code in a header file.  Oh well... */

/* WinMain wrapper for the example programs, by George Foot */

#define WIN32_LEAN_AND_MEAN

#define BITMAP WIN_BITMAP
#include <windows.h>
#undef BITMAP
#undef RGB

#include <winalleg.h>

#include <allegro.h>

#define main __gf_wrapper_main_func

int main (int argc, char *argv[]);


int WINAPI WinMain (
	HINSTANCE hInst, 
	HINSTANCE hPrev, 
	LPSTR lpCmdLine, 
	int nCmdShow)
{
	if (WinAllegro_Init (hInst) == 0) {
		int result;
		result = main (WinAllegro_GetArgc(), WinAllegro_GetArgv());
		allegro_exit();
		WinAllegro_Exit();
		return result;
	} else {
		WinAllegro_Exit();
		MessageBox (
			NULL,
			"WinAllegro initialisation failed."
			"For more information look in allegro.log",
			"WinAllegro Error",
			MB_ICONASTERISK | MB_OK
		);
		return -1;
	}
}


