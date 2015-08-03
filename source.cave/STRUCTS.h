// ################################################################# //
// ================================================================= //
// ----------------------------------------------------------------- //
//                         Structures                                //
// ----------------------------------------------------------------- //
// ================================================================= //
// ################################################################# //
#ifndef _STRUCTS_def
#define _STRUCTS_def

#include "Vector.h"
	   
typedef struct{
	char pressed;
	char changed;
}ButtonType;

typedef struct{
	ButtonType button[10];
	float dx;int x;
	float dy;int y;
	float dz;int z;
	char enabled;
}MouseType;

typedef struct{
	ButtonType button[10];
	float dx;int x;
	float dy;int y;
	float dz;int z;
	char enabled;
}JoyType;
	  
typedef struct{
	int width;
	int height;
	int bpp;
	int fullscreen;
}WindowType;
	  
typedef struct{
	vector pos;
	vector rot;
}ViewType;
	  
// Window Info for all
typedef struct{
	WindowType	win;
	ButtonType 	key[256];
	MouseType	mouse;
	JoyType		joy;
	ViewType	view;
	int			time,deltatime;
	char		exit;
}InfoType;

typedef struct
{
	vector pos;
	vector add;
	int id;
}
ENTITY;


// ################################################################# //
#endif
// ################################################################# //

