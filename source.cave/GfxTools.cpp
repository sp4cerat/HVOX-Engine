///////////////////////////////////////////
#define  STRICT
#include <time.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <malloc.h>
///////////////////////////////////////////
#include "STRUCTS.h" // important Structs+Vars
///////////////////////////////////////////
#include "GfxTools.h"
///////////////////////////////////////////
GfxTools* self;
///////////////////////////////////////////
HWND GfxTools::OpenWindow(int x,int y,void* instance)
{		
	HINSTANCE hInstance=*((HINSTANCE*)instance);			   

//  LRESULT (CALLBACK GfxTools::*WP)(HWND, UINT, WPARAM, LPARAM);
//	WP=&GfxTools::WndProc;
//	void* a=&WP;
//	WNDPROC *b=(WNDPROC*)a;
//	WndProc2=&GfxTools::WndProc;

    HWND        hwnd;										// HWND - a HWND is a handle to a window.  This is used to keep track of a certain window.  Programs can have multiple windows.  We only have one though.
    WNDCLASSEX  wndclass;									// WNDCLASSEX - This variable will hold all the information about the window (The name, icon, cursor, color, menu bar...)
    wndclass.cbSize        = sizeof (wndclass);				// Here we set the size of the wndclass. You will see this a lot in windows, it's kinda odd.  We use the "sizeof()" function to tell windows the size of our class.
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;		// The style we want is Verticle-Redraw and Horizontal-Redraw
    wndclass.lpfnWndProc   = &WndProc;						// Here is where we assing our CALLBACK function.  Remember up above our function "WndProc"?  This just tells windows which function it needs to call to check for window messages.
    wndclass.cbClsExtra    = 0;								// We don't want to allocate any extra bytes for a class (useless for us)
    wndclass.cbWndExtra    = 0;								// Another useless thing for us.  I believe these last 2 are initialized to 0 anyway.
    wndclass.hInstance     = hInstance;						// We assign our hInstance to our window.  Once again, You can have several instances of a program, so this keeps track of the current one.
	wndclass.hIcon         = 0;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName  = NULL;							// We don't have a menu, so let's set this to NULL.
    wndclass.lpszClassName = "Galaxy";				// Here we set a name for our class, to distinguish it against others.  We need to use this name for later when we create the window.					
    wndclass.hIconSm       = LoadIcon (NULL, IDI_WINLOGO);	// We want the icon to be the windows logo.  This is the icon in the top left corner of the window.
	RegisterClassEx (&wndclass);							// We need to register our windows class with the windows OS.  We also need to pass the memory address of wndclass, so we use the "&".
															// Now we actually create the window.
															// CreateWindow() returns a handle to the window, which we store in our HWND "hwnd".
    hwnd = CreateWindow ("Galaxy",							// window class name - This tells CreateWindow() to use our class up above.
						 "Galaxy",		  					// window's Title    - This will be the name on the title bar of the window.
						 WS_OVERLAPPEDWINDOW,				// window style		 - This flag tells windows to create a typical window, (options like resize, minimize, close, etc).
						 CW_USEDEFAULT,						// initial x position- The top left corner X value of the window in screen coordinates.  This flag let's windows choose for us.
						 CW_USEDEFAULT,						// initial y position- The top left corner Y value of the window in screen coordinates.  This flag let's windows choose for us.
						 x,									// initial x size	 - The bottom right corner X value of the window in screen coordinates.  This flag let's windows choose for us.
						 y,								    // initial y size	 - The bottom right corner Y value of the window in screen coordinates.  This flag let's windows choose for us.
						 NULL,								// This is the parent window handle.  Since we don't have a parent window, we set this to NULL
						 NULL,								// This is the window menu handle, but since we don't have a menu, we set this to NULL.
						 hInstance,						    // This is the programs instance handle.  We just pass it our hInstance from our WinMain().  By the way, Windows OS passes the info to WinMain(). It's all taken care of for us.
						 NULL);								// If we wanted to pass in a variable or structure to the "WndProc", we would do it here.  (I just prefer using global variables though).  I just pass in NULL.

    ShowWindow (hwnd, 1/*iCmdShow*/);						// This shows our window.  We give it our handle to our window, which now has all our windows info, and our WinMain() variable iCmdShow.
    		 												// Here is our main loop.  This will continue to go until GetMessageReturns WM_QUIT, which will close the program.  This would happen if we closed the window.
    UpdateWindow (hwnd);									// This pretty much paints our window to the screen.

	return hwnd;
}
///////////////////////////////////////////
LRESULT CALLBACK GfxTools::WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)											// This checks what the message is.  Below is some of the message that windows might return.  There of course is HUNDREDS.
    {														// The only message that you NEED to deal with, is the WM_DESTROY.  The rest you don't have to check for.  I am just showing some examples on how to check for a message.
		case WM_CREATE:										// This message is sent when the window is created.
			// We would want to put our initialization code here...
			break;											// We break from the switch statement.

		case WM_SIZE:										// This Message happens when we resize the window.
			 self->Resize(LOWORD (lParam), HIWORD (lParam) ,32,false);// 
			break;											// Break from the switch statement

		case WM_PAINT:										// This message is sent to the WndProc when the window needs to be repainted.  This might be if we moved the window, resized it, or maximized it, or another window was over it.
			// Put our code here to paint what should be in the window.	
		    break;											// Break from the switch statement

		case WM_CLOSE:									// This message is sent when the user closes the window.
		case WM_DESTROY:									// This message is sent when the user closes the window.
			 self->running=false;
		//	PostQuitMessage(0);								// You must call this function or else you will need to do control-alt-delete and manually close the program from the programs queue.  0 = WM_QUIT.
															// This function actually puts a message on the message queue.  SendMessage() sends a direct message, which is acted upon immediately.  just for your information.
			break;											// Break from the switch statement
	}	
	return DefWindowProc (hwnd, iMsg, wParam, lParam);		// The DefWindowProc function calls the default window procedure to provide default processing for any window messages that an application does not process. 
															// This function ensures that every message is processed. DefWindowProc is called with the same parameters received by the WndProc. 
}															// End of the WndProc
///////////////////////////////////////////
void GfxTools::setVideoMode(int x,int y,int bpp){
    OSVERSIONINFO   vinfo;
    int             mode;
    DEVMODE         dm;
    memset(&dm, 0,    sizeof(dm));
    dm.dmSize       = sizeof(dm);
    dm.dmPelsWidth  = scr_x  =x;
    dm.dmPelsHeight = scr_y  =y;
    dm.dmBitsPerPel = scr_bpp=bpp;
    dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
    if (ChangeDisplaySettings( &dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) 
	  {MessageBox(NULL, "SetVideoMode - ChangeDisplaySettings failed.\n" "Switching to windowed mode.", "WARNING", MB_OK);  }
}
///////////////////////////////////////////
bool GfxTools::setVideoFormat(void)   
{
	PIXELFORMATDESCRIPTOR pfd =
		{sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
				2,                       // version number
				PFD_DRAW_TO_WINDOW |     // support window
				PFD_SUPPORT_OPENGL |     // support OpenGL
				PFD_DOUBLEBUFFER,        // double buffered
				PFD_TYPE_RGBA,           // RGBA type
				24,                      // 24-bit color depth
				0, 0, 0, 0, 0, 0,        // color bits ignored
				0,                       // no alpha buffer
				0,                       // shift bit ignored
				0,                       // no accumulation buffer
				0, 0, 0, 0,              // accum bits ignored
				32,                      // 32-bit z-buffer
				0,                       // no stencil buffer
				0,                       // no auxiliary buffer
				PFD_MAIN_PLANE,          // main layer
				0,                       // reserved
				0, 0, 0                  // layer masks ignored
		};
    int     pfm; 
    RECT    rect;
    hDC = GetDC(hWnd);
    pfm = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pfm, &pfd);
    DescribePixelFormat(hDC, pfm, sizeof(pfd), &pfd);
    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);
    return TRUE; 
}
///////////////////////////////////////////
GfxTools::~GfxTools(void)  
{
    ShowCursor(TRUE);
    ChangeDisplaySettings( 0, 0 );
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hGLRC);
	ReleaseDC(hWnd, hDC);
}
///////////////////////////////////////////
void GfxTools::Resize(int x,int y,int bpp,bool fullscreen)
{
	if (fullscreen)
     setVideoMode(x,y,bpp);

	glViewport (0,0,x,y);

	scr_bpp	=bpp;
	scr_x	=x;	   
	scr_y	=y;
}
///////////////////////////////////////////
GfxTools::GfxTools(InfoType *info,void* instance)
{
	self=this;

	HINSTANCE hInstance=*((HINSTANCE*)instance);

    //ShowCursor(fullscreen^1);

	hWnd		= OpenWindow(x,y,instance);
	windowhandle=&hWnd;

	if (fullscreen)
     setVideoMode(scr_x,scr_y,scr_bpp);

	setVideoFormat();
}
///////////////////////////////////////////
