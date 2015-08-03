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
#define loop(_a,_b,_c) for(_a = _b ; a<_c; _a++ )

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)			// Anyway, so here is our "main()" for windows.  Must Have this for a windows app.
{														
#if 0
	BMP res2;res2.load("hvox-quality/n2_f.bmp");
	BMP res3;res3.load("hvox-quality/n3_f.bmp");
	BMP res4;res4.load("hvox-quality/n4_f.bmp");
	BMP res5;res5.load("hvox-quality/n5_f.bmp");
	BMP res6;res6.load("hvox-quality/n6_f.bmp");

	int a,
		b2=0,
		b3=0,
		b4=0,
		b5=0;
	loop(a,0,res2.width*res2.height)
	{
		int e2 = ( abs ( res6.data[a*3] - res2.data[a*3] ) > 20 ) ? 1 : 0;
		int e3 = ( abs ( res6.data[a*3] - res3.data[a*3] ) > 20 ) ? 1 : 0;
		int e4 = ( abs ( res6.data[a*3] - res4.data[a*3] ) > 20 ) ? 1 : 0;
		int e5 = ( abs ( res6.data[a*3] - res5.data[a*3] ) > 20 ) ? 1 : 0;

		if(e2) b2++; res2.data[a*3]=res2.data[a*3+1]=res2.data[a*3+2]=e2*255;
		if(e3) b3++; res3.data[a*3]=res3.data[a*3+1]=res3.data[a*3+2]=e3*255;
		if(e4) b4++; res4.data[a*3]=res4.data[a*3+1]=res4.data[a*3+2]=e4*255;
		if(e5) b5++; res5.data[a*3]=res5.data[a*3+1]=res5.data[a*3+2]=e5*255;
	}
	float err2 = float(b2) * 100.0f / float(res2.width*res2.height );
	float err3 = float(b3) * 100.0f / float(res2.width*res2.height );
	float err4 = float(b4) * 100.0f / float(res2.width*res2.height );
	float err5 = float(b5) * 100.0f / float(res2.width*res2.height );
	char text[200];
	sprintf(text,"error: [2] %3.3f [3] %3.3f [4] %3.3f [5] %3.3f",err2,err3,err4,err5);

	res2.save("hvox-quality/n2_err.bmp");
	res3.save("hvox-quality/n3_err.bmp");
	res4.save("hvox-quality/n4_err.bmp");
	res5.save("hvox-quality/n5_err.bmp");

	MessageBox(NULL, text, "Error", MB_OK);

	exit(0);
#endif

	memset(&info,0,sizeof(InfoType));

	//WinMain_hInstance=hInstance;

	info.win.width=1024;
	info.win.height=768;
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
