///////////////////////////////////////////
#define STRICT
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
///////////////////////////////////////////
#include "iotools.h"
///////////////////////////////////////////
Keyboard::Keyboard(InfoType *globalInfo,void* windowhandle)
{

  if(globalInfo==NULL) return;
  if(windowhandle==NULL) return;

  info=globalInfo;


  enabled =SUCCEEDED( DirectInput8Create( 
                        GetModuleHandle(NULL), 
                        DIRECTINPUT_VERSION,  
					    IID_IDirectInput8, 
					    (VOID**)&g_pDI, NULL ) );
  if (!enabled)return;

  enabled =SUCCEEDED( g_pDI->CreateDevice( 
                        GUID_SysKeyboard, 
						&g_pKeyboard, 
						NULL ) ) ;
  if (!enabled)return;

  enabled =SUCCEEDED( g_pKeyboard->SetDataFormat(&c_dfDIKeyboard) ) ;

  if (!enabled)return;

  enabled =SUCCEEDED( g_pKeyboard->SetCooperativeLevel( 
						*((HWND*)windowhandle),
						DISCL_NONEXCLUSIVE|DISCL_FOREGROUND ));

  for ( int i=0;i<256;i++) b[i]=false;
  for (     i=0;i<256;i++) keytab [i]=128    ;
}
///////////////////////////////////////////
Keyboard::~Keyboard()
{
    SAFE_RELEASE( g_pKeyboard );
    SAFE_RELEASE( g_pDI );
}
///////////////////////////////////////////
void Keyboard::update(  )
{
    if( !enabled ) return ;
	g_pKeyboard->Acquire();
	g_pKeyboard->GetDeviceState(sizeof(keytab),&keytab);
    for ( int i=0;i<256;i++) 
	{
	 b_before	[i]=b[i];
	 b			[i]=(keytab [i] & 128)==128;
	 info->key[i].pressed=b[i];
	 info->key[i].changed=b[i]^b_before[i];
	}

	if(b[DIK_ESCAPE])info->exit=true;
};
///////////////////////////////////////////
Mouse::Mouse(InfoType *globalInfo,bool showcursor,void* windowhandle)
{
  info=globalInfo;

  enabled =SUCCEEDED( DirectInput8Create( 
                        GetModuleHandle(NULL), 
                        DIRECTINPUT_VERSION,  
					    IID_IDirectInput8, 
					    (VOID**)&g_pDI, NULL ) );
  if (!enabled)return;

  enabled =SUCCEEDED( g_pDI->CreateDevice( GUID_SysMouse, &g_pMouse, NULL ) ) ;

  if (!enabled)return;

  enabled =SUCCEEDED( g_pMouse->SetDataFormat( &c_dfDIMouse2 ) );

  if (!enabled)return;

  enabled =SUCCEEDED( g_pMouse->SetCooperativeLevel( 
						*((HWND*)windowhandle),
						DISCL_EXCLUSIVE|DISCL_FOREGROUND ) );

  for ( int i=0;i<256;i++) b[i]=false;

  x=y=z=dx=dy=dz=0;

  ShowCursor(showcursor);
}
///////////////////////////////////////////
Mouse::~Mouse()
{
    ShowCursor(true);
    SAFE_RELEASE( g_pMouse );
    SAFE_RELEASE( g_pDI );
}
///////////////////////////////////////////
void Mouse::update()
{
	DIMOUSESTATE2 dims2;

    if( !enabled ) return ;

	g_pMouse->Acquire();
    g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );

	if (abs(dims2.lX)>1000) return;
	if (abs(dims2.lY)>1000)	return;

    for ( int i=0;i<8;i++)
	{
	 b_before	[i]= b[i];
	 b			[i]=(dims2.rgbButtons[i]&128)==128;
	 info->mouse.button[i].pressed=b[i];
	 info->mouse.button[i].changed=b[i]^b_before[i];
	}

	if(b[0])
	{
		dx =dims2.lX;	info->mouse.dx=dx;
		dy =dims2.lY;	info->mouse.dy=dy;
		dz =dims2.lZ;	info->mouse.dz=dz;
		x+=dx      ;	info->mouse.x=x;
		y+=dy      ;	info->mouse.y=y;
		z+=dz      ;	info->mouse.z=z;
	}
	else
	{
		info->mouse.dx=dx=0;
		info->mouse.dy=dy=0;
	}
}
///////////////////////////////////////////
Joystick *self;
///////////////////////////////////////////
BOOL CALLBACK EnumAxesCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
    HWND hDlg = (HWND)pContext;
    DIPROPRANGE diprg; 
    diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
    diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    diprg.diph.dwHow        = DIPH_BYID; 
    diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
    diprg.lMin              = -JOY_PRECISION ; 
    diprg.lMax              = +JOY_PRECISION ; // Set the range for the axis
	if( FAILED( self->g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )		
		return DIENUM_STOP;
    return DIENUM_CONTINUE;
}
///////////////////////////////////////////
BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
{
    HRESULT hr;
    hr = self->g_pDI->CreateDevice( pdidInstance->guidInstance, &self->g_pJoystick, NULL );
    if( FAILED(hr) ) 
	return DIENUM_CONTINUE;
    return DIENUM_STOP;
}
///////////////////////////////////////////
Joystick::Joystick(InfoType *globalInfo,void* windowhandle)
{
	info=globalInfo;
	self=this;

	DIDEVCAPS			g_diDevCaps;

	enabled =SUCCEEDED( DirectInput8Create( 
						GetModuleHandle(NULL), 
						DIRECTINPUT_VERSION,  
						IID_IDirectInput8, 
						(VOID**)&g_pDI, NULL ) );
	if (!enabled)return;

	enabled =SUCCEEDED( g_pDI->EnumDevices( 
						DI8DEVCLASS_GAMECTRL,  
						EnumJoysticksCallback,
						NULL, DIEDFL_ATTACHEDONLY ));
	if (!enabled)return;								

	enabled=( NULL != g_pJoystick );

	if (!enabled)return;

	enabled =SUCCEEDED( g_pJoystick->SetDataFormat( &c_dfDIJoystick2 ));

	if (!enabled)return;

	enabled =SUCCEEDED(g_pJoystick->SetCooperativeLevel( 
						*((HWND*)windowhandle), 
						DISCL_EXCLUSIVE | DISCL_FOREGROUND )) ;
	if (!enabled)return;

	g_diDevCaps.dwSize = sizeof(DIDEVCAPS);

	enabled =SUCCEEDED(g_pJoystick->GetCapabilities(&g_diDevCaps) );

	if (!enabled)return;

	enabled =SUCCEEDED(g_pJoystick->EnumObjects( 
						EnumAxesCallback, (VOID*)*((HWND*)windowhandle), DIDFT_AXIS ));
	if (!enabled)return;

    for ( int i=0;i<256;i++) b[i]=false;

	x=y=z=dx=dy=dz=0;
};
///////////////////////////////////////////
Joystick::~Joystick()
{
    SAFE_RELEASE( g_pJoystick );
    SAFE_RELEASE( g_pDI );
}
///////////////////////////////////////////
void Joystick::update()
{
	HRESULT     hr;
	DIJOYSTATE2 js;           // DInput joystick state 

	if (!enabled)return;

	if( FAILED( g_pJoystick->Acquire() )) return;

	if( FAILED( g_pJoystick->Poll())) return;

	if( FAILED( g_pJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &js ) ) )return ;

    for ( int i=0;i<128;i++) 
	{
 	 b_before	[i]=b[i];
	 b			[i]=(js.rgbButtons[i] & 128) == 128 ;
	 info->joy.button[i].pressed=b[i];
	 info->joy.button[i].changed=b[i]^b_before[i];
	}
			
	dx=(float)js.lX/JOY_PRECISION ;
	dy=(float)js.lY/JOY_PRECISION ;
	dz=(float)js.lZ/JOY_PRECISION ;

	x+=dx  ;
	y+=dy  ;
	z+=dz  ;

	info->joy.dx=dx;
	info->joy.dy=dy;
	info->joy.dz=dz;

	info->joy.x=x;
	info->joy.y=y;
	info->joy.z=z;

	info->joy.enabled=true;
}

///////////////////////////////////////////
