#ifndef _HOUSE_class
#define _HOUSE_class

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VECTOR.h"
#include "MC.h"
#include "Structs.h"

class HOUSE;
class HOUSE
{
public:

	HOUSE();
	HOUSE(InfoType *newInfo,int sizex,int sizey,int sizez,ivector *connnectors,int *con_types,int con_count,int scalefactor);
	~HOUSE();
	/*
	# TYPE 0 : CUBE
	# TYPE 1 : BALL
	# TYPE 2 : CYLINDER X
	# TYPE 3 : CYLINDER Y
	# TYPE 4 : CYLINDER Z
	#
	# MODE 0 : OR   (ADD)
	# MODE 1 : AND (SUB)
	# MODE 2 : XOR
	*/
//	void shape(int type,int mode,int x1,int y1,int z1,int x2,int y2,int z2);
//	void shape_raw(int type,int mode,int x1,int y1,int z1,int x2,int y2,int z2,int block);
	void shape(int type,int mode,int x1,int y1,int z1,int x2,int y2,int z2,char* src=NULL);
	void shape_raw(int type,int mode,int x1,int y1,int z1,int x2,int y2,int z2,int block,char* src=NULL);
	void clear();
	bool setPos(int x,int y,int z);
	void setParent(HOUSE* parenthouse);
	void setChild(HOUSE* childhouse);
	void getPolys();
	void reset();
	bool flip();
	
private:

public:

    bool updated;

	int sx,sy,sz,scale;//Size
	
	char* data;//x*y*z; cube containing bricks 
	char* data_tmp;// "backbuffer"
	int posx,posy,posz;
	int posx_old,posy_old,posz_old;
	//int posx_gt,posy_gt,posz_gt;
	ivector clipstart[4],clipend[4]; // clipping volumes
	
	//ivector box_start[3],box_end[3];
	//ivector min_old,max_old;
	
	//int newposx,newposy,newposz;
	int minx,miny,minz;
	int maxx,maxy,maxz;
	
	int ofs_x,ofs_y,ofs_z;
	
	HOUSE* parent;
	HOUSE* child;
	MC* mc;
	SCENE* scene;

private:

	bool firstrun;	
	InfoType *info;

};

#endif
