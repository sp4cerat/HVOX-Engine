#ifndef _MC_class
#define _MC_class

#include <windows.h>
#include <mmsystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VECTOR.h"

#define _TEX_COORD_SCALE_ 10
#define MC_MAX_STRIPLENGTH 200000
#define SCALEX SCALE_FACTOR
//#define _ARRAY_OBJECT_


typedef struct
{
	int id[4];	//index
	int link[4];	//link index to next quad
	short int texture;	//
	short int type;	//direction (0-5)
	int used;
	int dst;//destination after reuse; -1=del ; 2 pass
}
SURFACE;

typedef struct
{
	vector p;	//usual
	vector q;	//temporary for blurring; afterwards normal vector
	int color; //color + used to normalize the n vector
	int id[6];	//index link between points for smoothing
	int poly[6];	//link to polygon
	int idmax;
	ivector pos;
	void* parent;// link to parent point for smoothing
	int dst; //destination for reuse; -1=not resused; 2 pass
	int voxel; //link to original voxel
}
MC_POINT;

typedef struct
{
	int start,count;
}
VA_CACHE_ELEM;

typedef struct
{
	vector 	points;	  //12 - 12
//	int		nop1;
//    float   tex,tey;
	int		color;	  //4  - 28
//	int		nop2;
}
T_ELEM;//GL_T2F_C4UB_V3F

typedef struct
{
	int	 	*dst;//link to cached lists
	int	 	*indexc;
	int	 	*indexs;
	int	 	*indexid;//id of strip
}
TSTRIPINDEX;

//#################################################################//
class XMEM
{
	public:

	XMEM(int elemsize,int indexstepsize,void** resultPtr);
	void check(int newindex);
	~XMEM();

	void*	mem;
	void**	outerPtr;
	int 	index;
	int 	stepsizeorig;
	int		stepsize;
	int		blocksize;
};
//#################################################################//
typedef struct
{
	bool	swapping;// swapping in progress
	bool	drawing; // drawing in progress
	bool	changed; // -> recaching 
	bool	childChanged; // -> recaching 
	bool	va_cache;// -> chache created?
	//int     arrayID;
	
	int	 	*v_dst;//link to cached lists
	int	 	*v_indexs;
	int	 	*v_cached;//array chached already?
	int	 	*v_uncached;//chached array removed list
	
	VA_CACHE_ELEM *va_cacheList;	//vertex array cache
	XMEM        *m_va_cacheList;	//vertex array cache mem control
	int            va_cacheCount;

	T_ELEM  *v;
	XMEM	*m_v;

	int	 	*v_quads;//link to the quads 
	XMEM	*m_quads;
	ivector *v_pointids;
	XMEM	*m_pointids;

	int 	v_count;

	int 	v_indexcount;
	int 	v_uncachedcount;
	int 	v_cachedcount;
	int 	v_indexcachedcount;
	
	void*	child; /*type:SCENE*/
	void*	parent; /*type:SCENE*/
	int		minx,miny,minz;
	int		maxx,maxy,maxz;
	int		posx,posy,posz;

	MC_POINT *sourcepoints;
	int		sourcepointscount;

	vector	*entities;		//entities position
	vector	*entitiesNV;	//entities normal vector
	int  	*entitiesID;	//entities ID
	int		entitiesCount;
}
SCENE;
//#################################################################//
class MC
{
public:

	MC();
	MC(int sizex,int sizey,int sizez,int scalefactor,char* data_link);
	~MC();
	void update();
	void getPolys();
	void getVertexArray();
	void getSmoothness(int intensity);
	void getSurfaces();
	void getLinks();
	void getColors();
	void getColors2();
	void swapScene();
	int getSingleColor(float intensity,float y)// return 32bit color rgba
	{
		int cc=(int(intensity*127+128)),rr,gg,bb;
		if(cc>255)cc=255;
		if(cc<0)cc=0;

/*if(intensity>=0)
		{
			rr=cc;//*2/3+80;
			gg=cc;//*2/3+80;
			bb=cc;//*2/3+80;
		}
		else
		{
			rr=cc;
			gg=cc/2;
			bb=cc/3;
		}
        //rr=gg=bb=255;
		int no=int(16*log(scale)/log(2));
*/
	//	if(intensity>0)

			int no=(int(log(scale)/log(2)));

			rr=(cc*no)/128;
			bb=(cc*no/2)/64;
			rr=cc*(129-no)/128;
			gg=rr*0.75;
			bb=rr*0.5;

			rr=gg=bb=cc;
			//rr=rr*5/(no*3+5);
			//gg=gg*5/(no*2+5);
			//rr+=(7-no)*3;
			//gg+=(7-no)*3;
			//bb-=no*20;

/*
			rr/=2;
			gg/=2;
			bb/=2;
*/
			if(rr>255)rr=255;
			if(gg>255)gg=255;
			if(bb>255)bb=255;
			if(rr<0  )rr=0  ;
			if(gg<0  )gg=0  ;
			if(bb<0  )bb=0  ;

		return (rr+gg*256+bb*65536);
	}

private:

	void recycle(void);
	void getSmoothedPartentPart1();
	void getSmoothedPartentPart2();
	void copyArrays(SCENE* target,SCENE* source,int source_start,int length);
	void insertPointBetween(int id1,int id2);
	void insertLinkBetween(int id1,int id2);
	int  getMiddleOfs(int id1,int id2);
	void getCoordFromOfs(int id2,ivector* v);
	void getPerlin();
	void getNormals();
	void getNormalInQuad(int id[4]);
	void getEntities(int max);
	int getOfsFromCoord(ivector *a);

public:

	int timing[10];

	// Voxel Data
	int sx,sy,sz,scale;//Size
	int posx,posy,posz;
	char *data;//x*y*z; link to cube containing bricks 
	char *data_src;//x*y*z; link to cube containing bricks 
	
	ivector addsrc;
	ivector adddst;
	ivector mov;
	ivector clipstart,clipend;
	
	int	pointcount;
	int	surfacecount;
	int	indexcount;

	bool preparingDone;
	bool flippingDone;
	
	int actualscene;	// 1or2
	SCENE	scene1,scene2,*scene,*tmp_scene;
	MC* parent;
	MC* child;
	
	int recycledcount;

	MC_POINT 	*tmp_point;
	XMEM		*tmp_point_mem;
	
	MC_POINT 	realpoints;
	MC_POINT 	*realpointscount;
	
	int 		general_strip_id; // counting value to identify lists
	
	SURFACE	    *tmp_surface;
	XMEM		*tmp_surface_mem;

	int 		*tmp_voxels;//links to all points
	int		    *tmp_index;

};
//#################################################################//
#endif
