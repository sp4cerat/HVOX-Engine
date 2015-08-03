#ifndef OBJ_class
#define OBJ_class

#include <time.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#include <GL\gl.h>
#include <GL\glu.h>
#include "VECTOR.h"

typedef struct {
	vector * v_       ;int v_anz;
	vector * vt       ;int vtanz;
	vector * vn       ;int vnanz;
    int    **f_       ;int f_anz;
} obj3d;

class Fisch 
{
    int reflection;
    int texture_anz;
    int displist[100];
    int usetex;
	obj3d fischi[100];
	int   fischi_anz;
	char  name[300];
	char *objnam;
	int objnamnum;

	bool initialized;

	bool noLightCalc;

public:

    int texture[100];

   void init(char* fname,matrix transform=ID);
   Fisch();
   Fisch(char* fname,bool noLight=false,matrix transform=ID);
   obj3d load(char* cc_name,matrix transformation);
   int show(int dist,int tex);
};

#endif