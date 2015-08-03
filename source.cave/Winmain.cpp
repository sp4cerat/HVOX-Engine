#define STRICT
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
//#include <signal.h>

#include "Window.h"
#include "IOTools.h"
#include "ENGINE.h"
#include "Structs.h"

InfoType info;
/*
HINSTANCE WinMain_hInstance;
Window*   WinMain_win;
Keyboard* WinMain_key   ;
//	Joystick* joy   (&info,win.windowhandle);
Mouse*    WinMain_mouse ;
ENGINE*   WinMain_engine;

static void CALLBACK mainLoop( UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 ) 
{
	Window   win   (&info,&WinMain_hInstance);	
	Keyboard key   (&info,win.windowhandle);
//	Joystick joy   (&info,win.windowhandle);
	Mouse    mouse (&info,false,win.windowhandle);
	ENGINE   engine(&info);

	WinMain_win=&win;
	WinMain_key=&key;
	WinMain_mouse=&mouse;
	WinMain_engine=&engine;

	WinMain_engine->init();

	info.time=timeGetTime();
	do
	{		
		int newtime=timeGetTime();
		info.deltatime=newtime-info.time;
		info.time=newtime;
		
		WinMain_engine->run();

//		WinMain_joy->update();
		WinMain_key->update();
		WinMain_mouse->update();
		WinMain_win->update();

		Sleep(1);

	}while( !info.exit );//&& (!mouse.b[0])

	WinMain_engine->quit();


}
  */

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)			// Anyway, so here is our "main()" for windows.  Must Have this for a windows app.
{														

	memset(&info,0,sizeof(InfoType));

	//WinMain_hInstance=hInstance;

	info.win.width=640;
	info.win.height=480;
	info.win.bpp=32;
	info.win.fullscreen=0;

	//timeSetEvent( 1,1,mainLoop, 0, TIME_ONESHOT );
	Window   win   (&info,&hInstance);	
	Keyboard key   (&info,win.windowhandle);
//	Joystick joy   (&info,win.windowhandle);
	Mouse    mouse (&info,false,win.windowhandle);
	ENGINE   engine(&info);

	//while(1)Sleep(1000000);
	engine.init();
	info.time=timeGetTime();
 
	do
	{		
		int newtime=timeGetTime();
		info.deltatime=newtime-info.time;
		info.time=newtime;
		
		engine.run();

//		joy.update();
		key.update();
		mouse.update();
		win.update();

	}while( !info.exit );//&& (!mouse.b[0])

	engine.quit();
   
	return 0 ;										// We return the wParam of the structure "msg".  This is why we chose WinMain() to return an int.  A wParam/lParam is a 32-bit message parameter.  It holds data about the message.  You will see it's uses later. 
}
