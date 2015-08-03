///////////////////////////////////////////
#ifndef _WINDOW_
#define _WINDOW_
///////////////////////////////////////////
#include <gl\gl.h>													// Header File For The OpenGL32 Library
#include <gl\glu.h>													// Header File For The GLu32 Library
#include <gl\glaux.h>												// Header File For The Glaux Library
#include <gl\glext.h>	
///////////////////////////////////////////
#include "STRUCTS.h"
///////////////////////////////////////////
class Window
{
///////////////////////////////////////////
	private:
	///////////////////////////////////////////
		HGLRC   hGLRC; // Device Context
		HDC     hDC;   //
		HWND    hWnd;  //
		HINSTANCE hInstance;
	///////////////////////////////////////////
	public:

		void*	windowhandle;
	///////////////////////////////////////////
		Window(InfoType *info,void* instance);
	    ~Window(void);

		HWND OpenWindow(int x,int y,void* instance);
		static LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
		void Resize(int scr_x,int scr_y,int scr_bpp,bool fullscreen);
		void setVideoMode(int x,int y,int bpp);
		bool setVideoFormat(void);
///////////////////////////////////////////
};
///////////////////////////////////////////
#endif
///////////////////////////////////////////
