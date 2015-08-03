//todo
// set clipping
//#################################################################//
#include "MC.h"
//#################################################################//
MC::MC()
{
	parent=child=NULL;
}
//#################################################################//
int sleepnow=timeGetTime();
#define S_L_E_E_P ;//{Sleep(1);}
//if(timeGetTime()-sleepnow>10){sleepnow=timeGetTime();Sleep(1);}}//if((sleepnow)==0)Sleep(1);sleepnow++;}
//#################################################################//
MC::MC(int sizex,int sizey,int sizez,int scalefactor,char* data_link)
{
	parent=child=NULL;

	general_strip_id=0;
		
	mov=ivector(0,0,0);
	clipstart=ivector(0,0,0);
	clipend=ivector(0,0,0);

	preparingDone=false;
	flippingDone=false;
	
	sx=sizex;
	sy=sizey;
	sz=sizez;
	scale=scalefactor;
	data=(char*)malloc(sx*sy*sz);//data_link;
	data_src=data_link;
	indexcount=surfacecount=pointcount=0;

	for (int i=0;i<10;i++)timing[i] =0;

	tmp_point		=NULL;
	tmp_point_mem   =new XMEM(sizeof(MC_POINT),1024*32,(void**)&tmp_point);
	pointcount		=0;
/*
	for(int a=0;a!=100000;a++)
	{
		tmp_point[a].color=0;
		if((a&1023)==0)
		tmp_point_mem->check(a);
	}
	exit(312);
*/

	tmp_surface	    =NULL;
	tmp_surface_mem	=new XMEM(sizeof(SURFACE),1024*64,(void**)&tmp_surface);
	surfacecount	=0;

	

	tmp_voxels		=(int*)malloc((sizex)*(sizey)*(sizez)*sizeof(int));
	tmp_index		=(int*)malloc(256*1024*sizeof(int));


	memset((void*)&scene1,0,sizeof(SCENE));
	memset((void*)&scene2,0,sizeof(SCENE));

	scene1.entitiesCount=0;
	scene1.entities=(vector*)malloc(10000*sizeof(vector));
	scene1.entitiesNV=(vector*)malloc(10000*sizeof(vector));
	scene1.entitiesID=(int*)malloc(10000*sizeof(int));
	scene1.v_indexs=(int*)malloc(256*1024*sizeof(int));
	scene1.va_cacheList=(VA_CACHE_ELEM*)malloc(256*1024*sizeof(VA_CACHE_ELEM));
	scene1.v_quads=NULL;
	scene1.m_quads=new XMEM(sizeof(int),1024*64,(void**)&scene1.v_quads);
	scene1.v_pointids=NULL;
	scene1.m_pointids=new XMEM(sizeof(ivector),1024*64,(void**)&scene1.v_pointids);
	scene1.v=NULL;
	scene1.m_v=new XMEM(sizeof(T_ELEM),1024*64,(void**)&scene1.v);
	scene1.v_count=0;
	scene1.v_indexcount=0;
	scene1.v_indexcachedcount=0;
	scene1.v_cachedcount=0;
	scene1.posx=0;
	scene1.posy=0;
	scene1.posz=0;
	scene1.drawing=false;
	scene1.swapping=false;
//	scene1.arrayID=-1;
	scene1.childChanged=false;
	
	//scene1.va_cacheList  =NULL;
	//scene1.m_va_cacheList=new XMEM(sizeof(VA_CACHE_ELEM),1024*8,(void**)&scene1.va_cacheList);
	//scene1.va_cacheCount =0;

	scene2.entitiesCount=0;
	scene2.entities=(vector*)malloc(10000*sizeof(vector));
	scene2.entitiesNV=(vector*)malloc(10000*sizeof(vector));
	scene2.entitiesID=(int*)malloc(10000*sizeof(int));
	scene2.v_indexs=(int*)malloc(256*1024*sizeof(int));
	scene2.va_cacheList=(VA_CACHE_ELEM*)malloc(256*1024*sizeof(VA_CACHE_ELEM));
	scene2.v_quads=NULL;
	scene2.m_quads=new XMEM(sizeof(int),1024*64,(void**)&scene2.v_quads);
	scene2.v_pointids=NULL;
	scene2.m_pointids=new XMEM(sizeof(ivector),1024*64,(void**)&scene2.v_pointids);
	scene2.v=NULL;
	scene2.m_v=new XMEM(sizeof(T_ELEM),1024*64,(void**)&scene2.v);
	scene2.v_count=0;
	scene2.v_indexcount=0;
	scene2.v_indexcachedcount=0;
	scene2.v_cachedcount=0;
	scene2.posx=0;
	scene2.posy=0;
	scene2.posz=0;
	scene2.drawing=false;
	scene2.swapping=false;
//	scene2.arrayID=-1;
	scene2.childChanged=false;

	if(
		(tmp_voxels==NULL) ||
		(tmp_index==NULL)||
		(scene1.entities==NULL)||
		(scene1.entitiesNV==NULL)||
		(scene1.entitiesID==NULL)||
		(scene1.v_indexs==NULL)||
		(scene1.va_cacheList==NULL)||
		(scene2.entities==NULL)||
		(scene2.entitiesNV==NULL)||
		(scene2.entitiesID==NULL)||
		(scene2.v_indexs==NULL)||
		(scene2.va_cacheList==NULL)
		)
	MessageBox(NULL, "Unexpected Exit", "Out Of Memory", MB_OK);

	//scene2.va_cacheList  =NULL;
	//scene2.m_va_cacheList=new XMEM(sizeof(VA_CACHE_ELEM),1024*8,(void**)&scene2.va_cacheList);
	//scene2.va_cacheCount =0;



	//char test[100];sprintf(test,"%d MB used",mem);
	//MessageBox(NULL, test, "huhu", MB_OK);


	actualscene=1;
	scene=&scene1;
	tmp_scene=&scene2;

	swapScene();
}
//#################################################################//
MC::~MC()
{

//	if(tmp_voxels)free(tmp_voxels);
//	if(tmp_surface)free(tmp_surface);
//	if(tmp_point)free(tmp_point);
//	if(tmp_quad)free(tmp_quad);
}
//#################################################################//
void MC::swapScene()
{
	//while(scene1.drawing||scene2.drawing){ S_L_E_E_P };

	//scene1.swapping=true;
	//scene2.swapping=true;

	if(actualscene==2)
	{	scene=&scene1;tmp_scene=&scene2;actualscene=1;}
	else
	{	scene=&scene2;tmp_scene=&scene1;actualscene=2;}
	
	scene->posx=tmp_scene->posx;
	scene->posy=tmp_scene->posy;
	scene->posz=tmp_scene->posz;
	
	if(parent!=NULL)
	{
	
	parent->scene->posx=posx/2;
	parent->scene->posy=posy/2;
	parent->scene->posz=posz/2;
	parent->scene1.posx=posx/2;
	parent->scene1.posy=posy/2;
	parent->scene1.posz=posz/2;
	parent->scene2.posx=posx/2;
	parent->scene2.posy=posy/2;
	parent->scene2.posz=posz/2;	 
	
	parent->scene->childChanged=true;
	parent->scene2.childChanged=true;
	}
	
	scene->changed=true;
////	scene->arrayID=tmp_scene->arrayID;
	
	scene1.swapping=false;
	scene2.swapping=false;
    //S_L_E_E_P 

	
}
//#################################################################//
int MC::getMiddleOfs(int id1,int id2)
{
				int x=id1%sx;
				int y=(id1/sx)%sy;
				int z=(id1/(sx*sy))%sz;

				x+=id2%sx;
				y+=(id2/sx)%sy;
				z+=(id2/(sx*sy))%sz;

				x/=2;y/=2;z/=2;

				return (x+sx*(y+sy*z));
}
//#################################################################//
void MC::insertLinkBetween(int id1,int id2)
{
				int i;
				int o1=tmp_voxels[id1];
				int o2=tmp_voxels[id2];
//				if(o1==-1)exit(1);
//				if(o2==-1)exit(2);

				bool link=true;
				int  max=tmp_point[o1].idmax;

				if(max>0)
				for (i=0;i<max;i++)
					if(tmp_point[o1].id[i]==o2) link=false;

				if (link) if (max<6) 
					 tmp_point[o1].id[max++]=o2;
					 
				tmp_point[o1].idmax=max;

				link=true;
				max=tmp_point[o2].idmax;

				if(max>0)
				for (i=0;i<max;i++)
					if(tmp_point[o2].id[i]==o1) link=false;

				if (link) if (max<6) 
					 tmp_point[o2].id[max++]=o1;
					 
				tmp_point[o2].idmax=max;
}
//#################################################################//
void MC::insertPointBetween(int id1,int id2)
{
				int o1=tmp_voxels[id1];
				int o2=tmp_voxels[id2];
//				if(o1==-1)exit(3);
//				if(o2==-1)exit(4);
				ivector i1;getCoordFromOfs(id1,&i1);
				ivector i2;getCoordFromOfs(id2,&i2);
				ivector im=(i1+i2)/2;
				int id=getOfsFromCoord(&im);

//				if(id<0)exit(2345);


				if(tmp_voxels[id]==-1)
				{ 	
					tmp_point[pointcount]=tmp_point[o1];
					tmp_point[pointcount].p+=tmp_point[o2].p; // point pos
					tmp_point[pointcount].p/=2;
					tmp_point[pointcount].pos=im;//tmp_point[o2].pos; // block pos
					//tmp_point[pointcount].pos.x/=2;
					//tmp_point[pointcount].pos.y/=2;
					//tmp_point[pointcount].pos.z/=2;
					tmp_point[pointcount].idmax=0;
					tmp_point[pointcount].voxel=id;
					tmp_point[pointcount].dst=1;
					tmp_point[pointcount].color=((tmp_point[o1].color&0xfefefe)+(tmp_point[o2].color&0xfefefe))/2;
					tmp_point[pointcount].parent=NULL;
					tmp_voxels[id]=pointcount;	
					pointcount++;
					if((pointcount&1023)==0)
						tmp_point_mem->check(pointcount);
				} 
				insertLinkBetween(id1,id);
				insertLinkBetween(id,id2);
}
//#################################################################//
void MC::getCoordFromOfs(int id2,ivector* v)
{
	v->x=(id2%sx)+posx;v->y=((id2/sx)%sy)+posy;v->z=(id2/(sx*sy))+posz;
}  
//#define OFS2COORD(o,c){c.x=(o%sx)+posx;c.y=((o/sx)%sy)+posy;c.z=(o/(sx*sy))+posz;}
//#################################################################//
int MC::getOfsFromCoord(ivector *a)
{
	return (a->x-posx+(a->y-posy+(a->z-posz)*sy)*sx);
}
//#define COORD2OFS(o,c){c.x=(o%sx)+posx;c.y=((o/sx)%sy)+posy;c.z=(o/(sx*sy))+posz;}
//#################################################################//
void MC::copyArrays(SCENE* target,SCENE* source,int source_start,int length)
{
	if(length<=2)return;

	length=(length/2)*2;

	int childstart=1+8*SCALEX;//*2*2;
	int childend=24*SCALEX;//*2*2;

	int startx=childstart+posx/2;
	int starty=childstart+posy/2;
	int startz=childstart+posz/2;
	
	int	 		*v_indexs			=target->v_indexs;//if(v_indexs==0)return;
	int 		 v_indexcount		=target->v_indexcount;
	int			 target_start		=target->v_count;

	int maxofs=target_start+length;//*4+20;
	target->m_pointids->check(maxofs);

	target->m_v->check(maxofs);
 /*
	target->m_points->check(maxofs);
	target->m_texco->check(maxofs);
	target->m_color->check(maxofs);
	*/
	target->m_quads->check(maxofs);

	int i,j,id[4];
	ivector coord[4];
	
	for(i=0;i<length;i++)
	{
		int x=(source->v_pointids[source_start+i].x-startx)*2;
		int y=(source->v_pointids[source_start+i].y-starty)*2;
		int z=(source->v_pointids[source_start+i].z-startz)*2;
		int ofs=x+(z*sy+y)*sx;

		for (j=0;j<3;j++)
		 id[j]=id[j+1];
		id[3]=ofs;

		if(ofs<0)return;//exit(54345);
		if(x>sx-1)return;
		if(y>sy-1)return;
		if(z>sz-1)return;

		if(tmp_voxels[ofs]==-1)
		{
			tmp_voxels[ofs]=pointcount;
			tmp_point[pointcount].idmax=0;
			tmp_point[pointcount].p    =source->v[source_start+i].points;
			tmp_point[pointcount].pos.x=x+posx;
			tmp_point[pointcount].pos.y=y+posy;
			tmp_point[pointcount].pos.z=z+posz;
			tmp_point[pointcount].parent=NULL;	
			tmp_point[pointcount].color=source->v[source_start+i].color;
			tmp_point[pointcount].dst=-1;
			tmp_point[pointcount].voxel=ofs;
			pointcount++;
			if((pointcount&1023)==0)
				tmp_point_mem->check(pointcount);
		}
		   
		if((i&1)==1)
		{

			//  o 2  
			//  +
			//  o 3  
			//
			insertPointBetween(id[2],id[3]);
			int m1=getMiddleOfs(id[2],id[3]);
//			if(tmp_voxels[m1]==-1)exit(2001);
      

			//insertLinkBetween(m1,id[2]);
			//insertLinkBetween(m1,id[3]);

			if(i>=3)
			{
				//  o + o (1)  0  o1 2    0 0  2  4 4  
				//  o-+-o (3)  m0 o2 m1         
				//  o + o (2)  1  o3 3    2 1  3  5 6  
				
				int m0=getMiddleOfs(id[0],id[1]);      
				int o1=getMiddleOfs(id[0] ,id[2]);
				int o2=getMiddleOfs(m0,m1);
				int o3=getMiddleOfs(id[1] ,id[3]);

				insertPointBetween(id[0],id[2]);
				insertPointBetween(m0,m1);
				insertPointBetween(id[1],id[3]);
//				if(tmp_voxels[o1]==-1)exit(2002);
//				if(tmp_voxels[o2]==-1)exit(2003);
//				if(tmp_voxels[o3]==-1)exit(2004);
				
				//insertLinkBetween(m0,id[0]);
				//insertLinkBetween(m0,id[1]);
				insertLinkBetween(o2,o1);
				insertLinkBetween(o2,o3);

				// write arrays		
				getCoordFromOfs(o1,&target->v_pointids[target_start+i*2-4+0]);
				getCoordFromOfs(o2,&target->v_pointids[target_start+i*2-4+1]);
				getCoordFromOfs(o2,&target->v_pointids[target_start+i*2-4+0+length*2-2]);
				getCoordFromOfs(o3,&target->v_pointids[target_start+i*2-4+1+length*2-2]);
			}
			// write arrays
			getCoordFromOfs(id[2],&target->v_pointids[target_start+i*2-2+0]);
			getCoordFromOfs(m1   ,&target->v_pointids[target_start+i*2-2+1]);
			getCoordFromOfs(m1   ,&target->v_pointids[target_start+i*2-2+0+length*2-2]);
			getCoordFromOfs(id[3],&target->v_pointids[target_start+i*2-2+1+length*2-2]);

		}	
	}

	v_indexs[v_indexcount+0]=target_start;
	v_indexs[v_indexcount+1]=target_start+(length*2-2);
	v_indexs[v_indexcount+2]=target_start+(length*2-2)*2;

	(target->v_indexcount)+=2;
	(target->v_count)+=length*4-4;

	maxofs=target->v_count;
	target->m_pointids->check(maxofs);
	target->m_v->check(maxofs);
//	target->m_points->check(maxofs);
//	target->m_texco->check(maxofs);
//	target->m_color->check(maxofs);
}
//#################################################################//
void MC::getSmoothedPartentPart2()
{
	int vcount=tmp_scene->v_count;
	int maxofs=vcount+1024;
	int i;

	tmp_scene->m_pointids->check(maxofs);
//	tmp_scene->m_points->check(maxofs);
//	tmp_scene->m_texco->check(maxofs);
//	tmp_scene->m_color->check(maxofs);

	tmp_scene->m_v->check(maxofs);

	ivector		*v_pointids	=tmp_scene->v_pointids;
//	vector 		*v_points	=tmp_scene->v_points;
//	vector 		*v_texco	=tmp_scene->v_texco;
//	int  		*v_color	=tmp_scene->v_color;

	T_ELEM		*v			=tmp_scene->v;

//	if(vcount!=tmp_scene->v_indexs[tmp_scene->v_indexcount])exit(543219);

	int id;
	float x,y,z;
	for (i=0;i<vcount;i++)
	{
		//if((i&16383)==0)
		//S_L_E_E_P 

		/*
		int ofs=getOfsFromCoord(&v_pointids[i]);
		if(ofs<0)exit(121212);		
		int id=tmp_voxels[ofs];
		if(id<0)exit(232323);
		 */
		id=tmp_voxels[getOfsFromCoord(&v_pointids[i])];

		tmp_scene->v[i].points=tmp_point[id].p;
		tmp_scene->v[i].color=tmp_point[id].color;
		x=tmp_point[id].p.x;
		y=tmp_point[id].p.y;
		z=tmp_point[id].p.z;

//		tmp_scene->v[i].tex=x/(1*_TEX_COORD_SCALE_);
//		tmp_scene->v[i].tey=z/(1*_TEX_COORD_SCALE_);				
	}	
}
//#################################################################//
void MC::getSmoothedPartentPart1()
{
  int a,i,x,y,z;
  int offset;
  int offset_parent;
  int offset_parent_add	=
			((posx  )/2-parent->posx)*1+
			((posy  )/2-parent->posy)*sx+
			((posz  )/2-parent->posz)*sx*sy;

	int mula=1;
	int mulb=sx;
	int mulc=sx*sy;

	//clear links from points to voxels
//	memset(tmp_voxels,-1,sx*sy*sz*4);
//	memset(tmp_points,0,pointcount*sizeof(MC_POINT));

//	pointcount=0;
//	surfacecount=0;

	// copy vertex arrays

	SCENE* scene=parent->scene; // parent Scene

//	if(parent->scene==NULL)exit(1234567);

	int childstart=1+8*SCALEX;//*2*2;
	int childend=24*SCALEX;//*2*2;
	
	int childstartx=childstart+posx/2;
	int childstarty=childstart+posy/2;
	int childstartz=childstart+posz/2;

	int childendx=childend+posx/2;
	int childendy=childend+posy/2;
	int childendz=childend+posz/2;

	tmp_scene->v_count=0;
	tmp_scene->v_indexcount=0;
	tmp_scene->v_indexcachedcount=0;

	if(scene->v_indexcount==0)return;

	for(i=0;i<scene->v_indexcount;i++)
	{
		int	startid=scene->v_indexs[i];
		int length=0;

		if((i&511)==0)	S_L_E_E_P //check

		bool incube_old=false;
		bool incube=false;
		
//		copyArrays(tmp_scene,scene,startid, scene->v_indexs[i+1]-scene->v_indexs[i]);

//		if(0)
		for(int j= scene->v_indexs[i];j< scene->v_indexs[i+1] ;j+=2)
		{
			incube_old=incube;
			if(
			((scene->v_pointids[j].x>=childstartx)&&(scene->v_pointids[j].x<=childendx)&&
			(scene->v_pointids[j].y>=childstarty)&&(scene->v_pointids[j].y<=childendy)&&
			(scene->v_pointids[j].z>=childstartz)&&(scene->v_pointids[j].z<=childendz))
			&&
			((scene->v_pointids[j+1].x>=childstartx)&&(scene->v_pointids[j+1].x<=childendx)&&
			(scene->v_pointids[j+1].y>=childstarty)&&(scene->v_pointids[j+1].y<=childendy)&&
			(scene->v_pointids[j+1].z>=childstartz)&&(scene->v_pointids[j+1].z<=childendz))
			)
				incube=true;
			 else
				incube=false;

			if(!incube)
			if(incube_old)
			{
			 if(length>=4)
			 {
				copyArrays(tmp_scene,scene,startid, length);
			 }
			 length=0;
			}

			if(incube)
			if(incube_old)
			{
				length+=2;
			}
			
			if(incube)
			if(!incube_old)
			{
				startid=j;
				length=2;
			}
		}

		if(length>=4)
		{
			copyArrays(tmp_scene,scene,startid, length);
		}
  }
	
	int endx=childend+posx/2;
	int endy=childend+posy/2;
	int endz=childend+posz/2;

	int dx=(childendx-childstartx)*2;
	int dy=(childendy-childstarty)*2;
	int dz=(childendz-childstartz)*2;
	

	//ivector		*v_pointids	=tmp_scene->v_pointids;
	//vector 		*v_points	=tmp_scene->v_points;
	S_L_E_E_P
	for (i=0;i!=pointcount;i++)
	{
		int x=tmp_point[i].pos.x-posx;
		int y=tmp_point[i].pos.y-posy;
		int z=tmp_point[i].pos.z-posz;
        if((x==0)||(y==0)||(z==0)||(x==dx)||(y==dy)||(z==dz))
 		 tmp_point[i].parent=(void*)1;

		//if((i&16383)==0)
		//S_L_E_E_P 
	} 
}
//#################################################################//
void MC::getPolys()
{

  recycle();
  S_L_E_E_P 


// 0 extract polys
// 1 subdivide
// 2 smooth 
// 3 normals
// 4 perlin
// 5 stripify
  
timing[6]=timeGetTime();

  if(scale<=START_SUBDIV)//5343965
  {
	timing[1]=timeGetTime();
	getSmoothedPartentPart1();
	timing[1]=timeGetTime()-timing[1];

	    S_L_E_E_P 

	if(scale>1)
	{

		timing[2]=timeGetTime();
        getSmoothness(1);
		timing[2]=timeGetTime()-timing[2];


		timing[3]=timeGetTime();
	    getNormals();
		timing[3]=timeGetTime()-timing[3];
	        S_L_E_E_P 

		timing[4]=timeGetTime();
	    getPerlin();
		timing[4]=timeGetTime()-timing[4];

	}else
    {
		timing[2]=timeGetTime();
	    getSmoothness(1);
		timing[2]=timeGetTime()-timing[2];
	    S_L_E_E_P 
		timing[3]=timeGetTime();
		getNormals();
		timing[3]=timeGetTime()-timing[3];
    }

    S_L_E_E_P 

	timing[5]=timeGetTime();
    getColors2();
    S_L_E_E_P 
    getSmoothedPartentPart2();
	timing[5]=timeGetTime()-timing[5];

	S_L_E_E_P 

	timing[6]=timeGetTime()-timing[6];
    return;
  }
  
	timing[0]=timeGetTime();
	getSurfaces();
	timing[0]=timeGetTime()-timing[0];

	S_L_E_E_P 
	timing[2]=timeGetTime();

	if(scale==1) getSmoothness(7);
	if(scale==2) getSmoothness(7);
	if(scale==4) getSmoothness(6);
	if(scale==8) getSmoothness(5);
	if(scale==16) getSmoothness(4);
	if(scale==32) getSmoothness(3);
	if(scale==64) getSmoothness(2);
	if(scale==128) getSmoothness(1);

	timing[2]=timeGetTime()-timing[2];
	S_L_E_E_P 

	timing[3]=timeGetTime();
	getColors();
	timing[3]=timeGetTime()-timing[3];

	timing[5]=timeGetTime();
		S_L_E_E_P 
	getLinks();
		S_L_E_E_P 
	getVertexArray();
	timing[5]=timeGetTime()-timing[5];

	timing[6]=timeGetTime()-timing[6];
	//printf("a surfaces:%d\n",surfacecount);
	//printf("b points:%d\n",pointcount);
	//printf("c indeces %d\n",indexcount);
}
//#################################################################//
void MC::getSurfaces()
{

	memcpy(data,data_src,sx*sy*sz);


	#define OVERSMPL 1

	
	int i;
	int xyzline[6],xyznow,xnew,ynew,znew,xyz[6],offset;
	int offset_parent;
	int x,y,z,val;

	//for(i=0;i!=(sx)*(sy)*(sz);i++)	tmp_voxels[i]=-1;
	//memset(tmp_voxels,-1,sx*sy*sz*4);
	//S_L_E_E_P 
	
	//memset(tmp_voxels,);
	
	int xyzmul[3][3]=
	{
	{1,sx*sy,sx},
	{sx*sy,sx,1},
	{sx,1,sx*sy}
	};
	
	int pox=0;
	int poy=0;
	int poz=0;
	
	if(parent!=NULL)// copy voxels from parent to fit box
	{
	        pox=parent->posx;
	        poy=parent->posy;
	        poz=parent->posz;
		
		int offset_parent_add	=
			((posx  )/2-parent->posx)*1+
			((posy  )/2-parent->posy)*sx+
			((posz  )/2-parent->posz)*sx*sy
			
			;
		int mula=1;
		int mulb=sx;
		int mulc=sx*sy;
		for(int k=0;k!=3;k++)
		{
			//S_L_E_E_P 
		for(int l=0;l!=2;l++)
		{
			if(l==0)z=0; else z=sz-3;
			mula=xyzmul[k][0];
			mulb=xyzmul[k][1];
			mulc=xyzmul[k][2];
			for(x=0;x!=sx-2;x++)
			for(y=0;y!=sy-2;y++)
			{
				offset			=
					x*mula+
					y*mulb+
					z*mulc;
				offset_parent	=
					((x+1)/2+sx/4)*mula+
					((y+1)/2+sy/4)*mulb+
					((z+1)/2+sz/4)*mulc+offset_parent_add;
				data[offset]=parent->data[offset_parent];
			}
		}
		}
	//S_L_E_E_P 
	}


    int offset_x1;
    int offset_y1;
    int offset_z1;

	int changes=0;

	//if(0)
	do
	{

		changes=0;
		
		for(z=0;z<sz-2;z+=OVERSMPL)//-2
			for(y=0;y<sy-2;y+=OVERSMPL)
				for(x=0;x<sx-2;x+=OVERSMPL)
				{
					offset=x+(y+z*sy)*sx;
					offset_x1=offset+1;
					offset_y1=offset+sx;
					offset_z1=offset+sx*sy;

					if(data[offset]==0)		if(data[offset+1]!=0)
					if(data[offset_y1]!=0)	if(data[offset_y1+1]==0)
					data[offset]=changes=1;
				
					if(data[offset]!=0)		if(data[offset+1]==0)
					if(data[offset_y1]==0)	if(data[offset_y1+1]!=0)
					data[offset+1]=changes=1;
				
					if(data[offset]==0)		if(data[offset+1]!=0)
					if(data[offset_z1]!=0)	if(data[offset_z1+1]==0)
					data[offset]=changes=1;
				
					if(data[offset]!=0)		if(data[offset+1]==0)
					if(data[offset_z1]==0)	if(data[offset_z1+1]!=0)
					data[offset+1]=changes=1;
				
					if(data[offset]==0)		if(data[offset+sx]!=0)
					if(data[offset_z1]!=0)	if(data[offset_z1+sx]==0)
					data[offset]=changes=1;
				
					if(data[offset]!=0)		if(data[offset+sx]==0)
					if(data[offset_z1]==0)	if(data[offset_z1+sx]!=0)
					data[offset+sx]=changes=1;
				

				}

		for(z=0;z<sz-2;z+=OVERSMPL)//-2
			for(y=0;y<sy-2;y+=OVERSMPL)
				for(x=0;x<sx-2;x+=OVERSMPL)
				{
					offset=x+(y+z*sy)*sx;

					int a,b,c,count;

					for(a=0;a<2;a++)
					for(b=0;b<2;b++)
					for(c=0;c<2;c++)
					if(data[offset+a+(b+c*sy)*sx]==0)count ++;
					
					if(count==2)			
					{
						if((data[offset     ]==0)&&(data[offset+1+sx+sx*sy]==0))	data[offset]=changes=1; else
						if((data[offset   +1]==0)&&(data[offset  +sx+sx*sy]==0))	data[offset+1]=changes=1; else
						if((data[offset+sx  ]==0)&&(data[offset+1   +sx*sy]==0))	data[offset+sx]=changes=1; else
						if((data[offset+sx+1]==0)&&(data[offset     +sx*sy]==0))	data[offset+sx+1]=changes=1;
					}
				
				}

	} while (changes*0==1);
			

	for(z=0;z<sz-3;z+=OVERSMPL)//-2
	{
	    if((z&1)==0)S_L_E_E_P 
		for(y=0;y<sy-3;y+=OVERSMPL)
		{
			for(x=0;x<sx-3;x+=OVERSMPL)
			if(pointcount<200000-6)
			if(surfacecount<200000-6)
			{
				offset=x+(y+z*sy)*sx;
				offset_x1=offset+OVERSMPL*1;
				offset_y1=offset+OVERSMPL*sx;
				offset_z1=offset+OVERSMPL*sx*OVERSMPL*sy;

				//data[offset]|=data[offset_x1]|data[offset_y1]|data[offset_z1];

				xyznow=xnew=ynew=znew=false;
			
				//if(offset<0)exit(4534);
				//if(offset>sx*sy*sz)exit(14534);

				if(data[offset ]!=0) xyznow=true;
				if(data[offset_x1]!=0) xnew=true;
				if(data[offset_y1]!=0) ynew=true;
				if(data[offset_z1]!=0) znew=true;
				
				if(x==0)ynew=znew=xyznow;
				if(y==0)xnew=znew=xyznow;
				if(z==0)ynew=xnew=xyznow;

				if(x==sx-2)ynew=znew=xyznow;
				if(y==sy-2)xnew=znew=xyznow;
				if(z==sz-2)ynew=xnew=xyznow;

				xyz[0]=xnew&&(!xyznow);//x poly yz front
				xyz[1]=(!xnew)&&xyznow;//x poly yz back
				xyz[2]=ynew&&(!xyznow);//y poly xz front
				xyz[3]=(!ynew)&&xyznow;//y poly xz back
				xyz[4]=znew&&(!xyznow);//z poly xy front
				xyz[5]=(!znew)&&xyznow;//z poly xy back
				
				int voxelinit_pairs[6][4][3]=
				{
					{{1,0,0},{1,0,1},{1,1,1},{1,1,0}},//0
					{{1,1,0},{1,1,1},{1,0,1},{1,0,0}},//1
					{{1,1,0},{1,1,1},{0,1,1},{0,1,0}},//2
					{{0,1,0},{0,1,1},{1,1,1},{1,1,0}},//3
					{{0,0,1},{0,1,1},{1,1,1},{1,0,1}},//4
					{{0,0,1},{1,0,1},{1,1,1},{0,1,1}} //5
				};
				
				for(i=0;i!=6;i++)
				{
					if(xyz[i])
					{
						int onew=0,idnew=0,idold=0;
						
						// check for recycled surface
						int recycled=0;
						for(int t=0;t!=4;t++)
						{
							int psx=voxelinit_pairs[i][t][0]*OVERSMPL;
							int psy=voxelinit_pairs[i][t][1]*OVERSMPL;
							int psz=voxelinit_pairs[i][t][2]*OVERSMPL;
							onew=offset+psx+(psy+psz*sy)*sx;
							int pointid=tmp_voxels[onew];

							if(pointid!=-1)//  point present
								if(tmp_point[pointid].dst!=-1)
									recycled++;
						}
						
						if(recycled!=4) // add new surface ?
						{
							tmp_surface[surfacecount].texture=0;
							tmp_surface[surfacecount].type=i;
							tmp_surface[surfacecount].link[0]=
							tmp_surface[surfacecount].link[1]=
							tmp_surface[surfacecount].link[2]=
							tmp_surface[surfacecount].link[3]=-1;

							for(int t=0;t!=5;t++)
							{
								int psx=voxelinit_pairs[i][t&3][0]*OVERSMPL;
								int psy=voxelinit_pairs[i][t&3][1]*OVERSMPL;
								int psz=voxelinit_pairs[i][t&3][2]*OVERSMPL;
								onew=offset+psx+(psy+psz*sy)*sx;

								if(tmp_voxels[onew]==-1)//insert  point
								{
									/*vector p;	//usual
									vector q;	//blurring vector
									int id[6];	//index
									int idmax;*/
									//create point
									tmp_voxels[onew]=pointcount;
									tmp_point[pointcount].voxel=onew;
									tmp_point[pointcount].dst=-1;
									tmp_point[pointcount].idmax=0;
									tmp_point[pointcount].color=0;
									tmp_point[pointcount].id[0]=-1;// 6 sides of the voxel
									tmp_point[pointcount].id[1]=-1;
									tmp_point[pointcount].id[2]=-1;
									tmp_point[pointcount].id[3]=-1;
									tmp_point[pointcount].id[4]=-1;
									tmp_point[pointcount].id[5]=-1;
									tmp_point[pointcount].p.x=2*(x+psx-sx/2+posx)*scale;
									tmp_point[pointcount].p.y=2*(y+psy-sy/2+posy)*scale;
									tmp_point[pointcount].p.z=2*(z+psz-sz/2+posz)*scale;
									tmp_point[pointcount].pos.x=x+psx+posx;
									tmp_point[pointcount].pos.y=y+psy+posy;
									tmp_point[pointcount].pos.z=z+psz+posz;
									tmp_point[pointcount].parent=NULL;
									//connect parent
									int createlink=0;
									if((x+psx==1)
									||(x+psx==sx-3)
									||(y+psy==1)
									||(y+psy==sy-3)
									||(z+psz==1)
									||(z+psz==sz-3))
									if(parent!=NULL)
									{      
										offset_parent	=
											((x+psx+1+posx  )/2+sx/4-pox)*1+
											((y+psy+1+posy  )/2+sy/4-poy)*sx+
											((z+psz+1+posz  )/2+sz/4-poz)*sx*sy;

										int parentpointid=parent->tmp_voxels[offset_parent];
										if(parentpointid!=-1)
										{
										tmp_point[pointcount].p=
											parent->tmp_point[parentpointid].p;
										tmp_point[pointcount].color=
											parent->tmp_point[parentpointid].color;
										tmp_point[pointcount].parent=(void*)1;//&parent->tmp_point[parentpointid].p;
										}
									}
									pointcount++;
									if((pointcount&1023)==0)
										tmp_point_mem->check(pointcount);
								}
								idold=idnew;
								idnew=tmp_voxels[onew];

								if(t!=0)
								{
									tmp_surface[surfacecount].used=0;
									tmp_surface[surfacecount].id[t-1]=idold;
									tmp_surface[surfacecount].link[t-1]=-1;

									int idmax1=tmp_point[idnew].idmax;
									int idmax2=tmp_point[idold].idmax;
									int u=0;
									if(idmax1!=0)
									while((u!=idmax1)&&(tmp_point[idnew].id[u]!=idold))u++;
									//if(idmax2!=6)//overflow
									//if(idmax1!=6)//overflow
									if(tmp_point[idnew].id[u]!=idold)
									{
										tmp_point[idnew].id[idmax1]=idold;
										tmp_point[idold].id[idmax2]=idnew;
										tmp_point[idnew].poly[idmax1]=surfacecount;
										tmp_point[idold].poly[idmax2]=surfacecount;
										tmp_point[idnew].idmax++;
										tmp_point[idold].idmax++;
									}
									else
									{
										int connection[6][6]=
										{
										{1,0,1,1,0,0},//0
										{0,1,1,1,0,0},//1
										{1,1,1,0,0,0},//2
										{1,1,0,1,0,0},//3
										{0,0,0,0,1,0},//4
										{0,0,0,0,0,1} //5
										};
										int id2=tmp_point[idnew].poly[u];
										int type1=tmp_surface[surfacecount].type;
										int type2=tmp_surface[id2].type;
										if(connection[type1][type2]==1)
										{
											int sid1=(t-1+0)&3;
											int sid2=(t-1+2)&3;
											if(tmp_surface[id2].id[sid2]==tmp_surface[surfacecount].id[(sid2+3)&3])
											if(tmp_surface[id2].link[sid2]==-1)
											if(tmp_surface[surfacecount].link[sid2]==-1)
											{
											 tmp_surface[id2].link[sid2]=surfacecount;
											 tmp_surface[surfacecount].link[sid1]=id2;
											}
										}
									}//connect point or surface
								}//t!=0
							}//t=0..4
							surfacecount++;  
							if((surfacecount&1023)==0)
								tmp_surface_mem->check(surfacecount);
						}//new surface end
					}
				}
			}
		}
	}
}
//#################################################################//
void MC::getColors()
{
	int i,j;

	int per_d[4][4]=
	{
	{2,3,1,0},//0
	{3,0,2,1},//1
	{0,1,3,2},//2
	{1,2,0,3} //3
	};
	int* per;// permutation for different directions
	
	vector n;
	
	

	for(i=0;i<pointcount;i++)
	{
	//if((i&16383)==0)S_L_E_E_P 
	if(tmp_point[i].parent==NULL)//&&(tmp_point[i].pos.x!=1))
	if(tmp_point[i].dst==-1)
	{
	tmp_point[i].color=0;
	tmp_point[i].q.x=
	tmp_point[i].q.y=
	tmp_point[i].q.z=0;
	}
	}
	
	vector d1,d2;

	S_L_E_E_P

	for(i=0;i<surfacecount;i++)
	{
		//if((i&511)==0)
		//S_L_E_E_P 

		int id[4];
		id[0]=tmp_surface[i].id[0];
		id[1]=tmp_surface[i].id[1];
		id[2]=tmp_surface[i].id[2];
		id[3]=tmp_surface[i].id[3];
		
		bool calc_necessary=false;
		for (j=0;j!=4;j++)
		if(tmp_point[id[j]].parent==NULL)
		if(tmp_point[id[j]].dst==-1)
			calc_necessary=true;

		if(calc_necessary)
		{
			d1=(tmp_point[id[2]].p-tmp_point[id[1]].p);
			d2=(tmp_point[id[2]].p-tmp_point[id[3]].p);
			if((d1.x!=0)||(d1.y!=0)||(d1.z!=0))
			if((d2.x!=0)||(d2.y!=0)||(d2.z!=0))
			{
					n=d1^d2;
					n=!n;

					for (j=0;j!=4;j++)
					if(tmp_point[id[j]].parent==NULL)//&&(tmp_point[i].pos.x!=1))
					if(tmp_point[id[j]].dst==-1)
					{
						tmp_point[id[j]].q+=n;
						tmp_point[id[j]].color++;
					}
			}
		}
	}
	S_L_E_E_P
	
	for(i=0;i<pointcount;i++)
	{
	//if((i&4095)==0)S_L_E_E_P 
	if(tmp_point[i].parent==NULL)//&&(tmp_point[i].pos.x!=1))
	if(tmp_point[i].color!=0)
	if(tmp_point[i].dst==-1)
	{
		tmp_point[i].q/=tmp_point[i].color;
		tmp_point[i].color=getSingleColor((tmp_point[i].q.y),tmp_point[i].p.y);
		//if(tmp_point[i].idmax==6)tmp_point[i].color=0x00ff00;
	}
	}
}
//#################################################################//
inline void MC::getNormalInQuad(int id[4])
{
	vector n,d1,d2,d3;  // 0  2

                   // 1  3
//	d1=tmp_point[id[0]].p-tmp_point[id[3]].p;
//	d2=tmp_point[id[1]].p-tmp_point[id[2]].p;
	d1=tmp_point[id[0]].p-tmp_point[id[1]].p;
	d2=tmp_point[id[0]].p-tmp_point[id[2]].p;

	d3 = d2-d1;


	float len1 = sqrt(d1.x*d1.x+d1.y*d1.y+d1.z*d1.z);
	float len2 = sqrt(d2.x*d2.x+d2.y*d2.y+d2.z*d2.z);
	float len3 = sqrt(d3.x*d3.x+d3.y*d3.y+d3.z*d3.z);
	if((len1<0.05) ||(len2<0.05)||(len3<0.05))
	{
		d1 = tmp_point[id[2]].p-tmp_point[id[0]].p; 
		d2 = tmp_point[id[3]].p-tmp_point[id[1]].p; 
	}
/*	len1 = sqrt(d1.x*d1.x+d1.y*d1.y+d1.z*d1.z);
	len2 = sqrt(d2.x*d2.x+d2.y*d2.y+d2.z*d2.z);
	if((len1<0.05) ||(len2<0.05))
	{
		d1 = tmp_point[id[2]].p-tmp_point[id[3]].p; 
		d2 = tmp_point[id[2]].p-tmp_point[id[0]].p; 
	}*/

	n=d2^d1;
	n=!n;

	for (int i=0;i!=4;i++)
		tmp_point[id[i]].q+=n;
}
//#################################################################//
void MC::getNormals()
{
  int i,j;
  
	for(i=0;i<pointcount;i++)
	{
//		tmp_point[i].color=0xffff;
		tmp_point[i].q.x=0;
		tmp_point[i].q.y=0;
		tmp_point[i].q.z=0;
	}
	S_L_E_E_P 
		
	int id[4];
	for(i=0;i<tmp_scene->v_indexcount;i++)
	{
		//if((i&1023)==0)S_L_E_E_P 
		for(j= tmp_scene->v_indexs[i];j< tmp_scene->v_indexs[i+1]-2;j+=2)
		{
			for (int t=0;t<4;t++)
			{
				id[t]=tmp_voxels[getOfsFromCoord(&tmp_scene->v_pointids[j+t])];
			}
			getNormalInQuad(id);
		}
	}
	int max;
	vector n;

	S_L_E_E_P 

	for(i=0;i<pointcount;i++)
    if(tmp_point[i].parent==NULL)
	{
		//if((i&16383)==0)S_L_E_E_P 
		tmp_point[i].q=!tmp_point[i].q;
	}

}
//#################################################################//
void MC::getColors2()
{
	int i,j;
	int max;
	vector n;

	for(i=0;i<pointcount;i++)
    if(tmp_point[i].parent==NULL)
	{
		if((i&32767)==0)S_L_E_E_P 

		n=tmp_point[i].q;

#if SHADE_FLAT
		int c = float(128.0f+63.0f*(n.y+n.x));
		c=c+c*256+c*65536;
#else
		int c = getSingleColor((n.y),tmp_point[i].p.y);
#endif
		tmp_point[i].color=c;//getSingleColor((n.y),tmp_point[i].p.y);
	}
}		
//#################################################################//
void MC::getPerlin()
{
	//return;
	int i,j;
	int max;
	vector n;
	float div;

	for(i=0;i<pointcount;i++)
	if(tmp_point[i].dst==1)
    if(tmp_point[i].parent==NULL)
	{
	  div=728;
      //if(scale==8) div=255; 
	    if(scale>=8)div=255;
//    if(n.y-0.5f<0)
		//if((i&16383)==0)S_L_E_E_P 

		n=tmp_point[i].q;
		int a1=abs(int((tmp_point[i].pos.x+221)*4532));
		int b1=abs(int((tmp_point[i].pos.y+326)*3461));
		int c1=abs(int((tmp_point[i].pos.z+362)*5231));
		int t;
		t=(a1^b1^c1)+(23627241*(scale+12546367));
		t=t+(64331*(scale+367));
		t&=255;
		float tf=(t-190)*scale;
		tmp_point[i].p+=n*(tf/div);//* (  ((float)(t*scale)) /65536);
	}
    
}
//#################################################################//
void MC::getEntities(int max)
{
	tmp_scene->entitiesCount=0;

	if(pointcount==0) return;
	if(max==0) return;

	if ( pointcount<max ) max=pointcount;

	int entitiesCount=0;

	for(int i=0;i!=pointcount;i++)
	{
		int ofs=i;//((i+21435)^4357321)%pointcount;

		int x=tmp_point[ofs].pos.x;
		int y=tmp_point[ofs].pos.y;
		int z=tmp_point[ofs].pos.z;
		int 
		check=abs(((x*2345521)^(y*235151)^(z*876331))+123521);
		check+=abs((x/345)^(y/352)^(z/321));

		if(y>0)
		if((check&255)==0)
		if(tmp_point[ofs].dst==-1)
		{
     		int 
			id =abs(x*645221+y*23441+z*65761+234621);
     		id+=abs(x/2+y/4+z/3);
     		id+=abs(x/411+y/541+z/327);
			tmp_scene->entities		[entitiesCount]=tmp_point[ofs].p;
			tmp_scene->entitiesID	[entitiesCount]=id;
			tmp_scene->entitiesNV	[entitiesCount]=tmp_point[ofs].q;
			entitiesCount++;
			if(entitiesCount==max)
			{
				tmp_scene->entitiesCount=entitiesCount;
				return;
			}
		}
	}
	tmp_scene->entitiesCount=entitiesCount;
}
//#################################################################//
void MC::getVertexArray()
{
//	int	 	*v_indexc=tmp_scene->v_indexc;if(v_indexc==0)return;
	int	 	*v_indexs=tmp_scene->v_indexs;if(v_indexs==0)return;
	int	 	*v_cached=tmp_scene->v_cached;
	ivector *v_pointids=tmp_scene->v_pointids;if(v_pointids==0)return;
	
//	vector 	*v_points=tmp_scene->v_points;if(v_points==0)return;
//	vector 	*v_texco=tmp_scene->v_texco;if(v_texco==0)return;
//	int		*v_color=tmp_scene->v_color;if(v_color==0)return;
	T_ELEM  *v=tmp_scene->v;

	int		*v_quads=tmp_scene->v_quads;
	int		v_count=tmp_scene->v_count;
	int 	v_indexcount=tmp_scene->v_indexcount;

	vector q[4],n;
	int per_d[4][4]=
	{
	{2,3,1,0},//0
	{3,0,2,1},//1
	{0,1,3,2},//2
	{1,2,0,3} //3
	};
	int* per;// permutation for different directions
	int i,j;
	float x,y,z;

	tmp_scene->v_count=v_count=0;
	
	for(i=0;i!=indexcount;i++){

		if((i&2047)==0)S_L_E_E_P 

		tmp_scene->m_pointids->check(v_count+1024);
		tmp_scene->m_v->check(v_count+1024);
		/*
		tmp_scene->m_points->check(v_count+1024);
		tmp_scene->m_texco->check(v_count+1024);
		tmp_scene->m_color->check(v_count+1024);*/
		tmp_scene->m_quads->check(v_count+1024);

		int surfid=tmp_index[i];
		int direction=0;
		per=per_d[0];

		while((tmp_surface[surfid].link[direction]==-1)&&(++direction!=4));
		if(direction!=4)
			per=per_d[direction];


//		v_cached[i]=0;
//		v_indexc[i]=0;
		v_indexs[i]=v_count;

#if SHADE_FLAT
		int tmpid=tmp_surface[surfid].id[0];
		tmp_point[tmpid].q.x=0;
		tmp_point[tmpid].q.y=0;
		tmp_point[tmpid].q.z=0;
		getNormalInQuad(tmp_surface[surfid].id);
		int coloo = float(128.0f+63.0f*(tmp_point[tmpid].q.y+tmp_point[tmpid].q.x));
		coloo = coloo+coloo*256+coloo*65536;
#endif
		for(j=0;j!=4;j++)
		{
			int id=tmp_surface[surfid].id[per[j]];
			x=tmp_point[id].p.x;
			y=tmp_point[id].p.y;
			z=tmp_point[id].p.z;
					
			tmp_scene->v[v_count].points=tmp_point[id].p;
			tmp_scene->v_pointids[v_count]=tmp_point[id].pos;
//			tmp_scene->v[v_count].tex=x/(1*_TEX_COORD_SCALE_);
//			tmp_scene->v[v_count].tey=z/(1*_TEX_COORD_SCALE_);
#if SHADE_FLAT
			tmp_scene->v[v_count].color=coloo;//tmp_point[id].color;//cc;//(((int)this*7414546)&0xffffff)+(0*345353&0x6f6f6f);
			tmp_point[id].color=coloo;
#else
			tmp_scene->v[v_count].color=tmp_point[id].color;//cc;//(((int)this*7414546)&0xffffff)+(0*345353&0x6f6f6f);
#endif

//			tmp_scene->v[v_count].color=tmp_point[id].color;//=cc;//(((int)this*7414546)&0xffffff)+(0*345353&0x6f6f6f);//0xffffff;//x*2+y*600+z*90000;
			tmp_scene->v_quads[v_count/2]=surfid;
									 
			++v_count;
//			++v_indexc[i];
		}
		
		if(direction!=4)
		{
			int cmpid=tmp_surface[surfid].used;

			surfid=tmp_surface[surfid].link[direction];
			while((surfid!=-1)&&(tmp_surface[surfid].used==cmpid))
			{
#if SHADE_FLAT
				int tmpid=tmp_surface[surfid].id[0];
				tmp_point[tmpid].q.x=0;
				tmp_point[tmpid].q.y=0;
				tmp_point[tmpid].q.z=0;
				getNormalInQuad(tmp_surface[surfid].id);
				int colo = float(128.0f+63.0f*(tmp_point[tmpid].q.y+tmp_point[tmpid].q.x));
				colo = colo+colo*256+colo*65536;
#endif
				for(j=2;j!=4;j++)
				{
					int id=tmp_surface[surfid].id[per[j]];
					x=tmp_point[id].p.x;
					y=tmp_point[id].p.y;
					z=tmp_point[id].p.z;

					tmp_scene->v[v_count].points=tmp_point[id].p;
					tmp_scene->v_pointids[v_count]=tmp_point[id].pos;
//					tmp_scene->v[v_count].tex=x/(1*_TEX_COORD_SCALE_);
//					tmp_scene->v[v_count].tey=z/(1*_TEX_COORD_SCALE_);

#if SHADE_FLAT
					tmp_scene->v[v_count].color=colo;//tmp_point[id].color;//cc;//(((int)this*7414546)&0xffffff)+(0*345353&0x6f6f6f);
					tmp_point[id].color=colo;
#else
					tmp_scene->v[v_count].color=tmp_point[id].color;//cc;//(((int)this*7414546)&0xffffff)+(0*345353&0x6f6f6f);
#endif
					tmp_scene->v_quads[v_count/2]=surfid;
										 
					++v_count;
//					++v_indexc[i];
				}
				surfid=tmp_surface[surfid].link[direction];
			}
		}
		v_indexs[i+1]=v_count;
	}

	tmp_scene->v_count=v_count;
	tmp_scene->v_indexcount=indexcount;
	
	tmp_scene->m_pointids->check(v_count);

	tmp_scene->m_v->check(v_count);
/*
	tmp_scene->m_points->check(v_count);
	tmp_scene->m_texco->check(v_count);
	tmp_scene->m_color->check(v_count);
*/	
	tmp_scene->m_quads->check(v_count);
}
//#################################################################//
void MC::getLinks()
{
	int i;
	/*
	for(i=0;i<surfacecount;i++)
	{
		tmp_surface[i].used=0;
	}
	*/
	
	for(i=0;i<surfacecount;i++)
	{
	if((i&1023)==0)S_L_E_E_P 
	if(tmp_surface[i].used==0)
	{
		tmp_surface[i].used=indexcount+10;
		tmp_index[indexcount]=i;
		int direction=0;
		int actualsurface=tmp_surface[i].link[direction];
		while((actualsurface==-1)&&(++direction!=4))
			{actualsurface=tmp_surface[i].link[direction];}
			
		int striplength=0;
		if(direction!=4)
		while((actualsurface!=-1)&&(tmp_surface[actualsurface].used==0)&&(++striplength!=MC_MAX_STRIPLENGTH))
		{
			tmp_surface[actualsurface].used=indexcount+10;
			actualsurface=tmp_surface[actualsurface].link[direction];
		}
		indexcount++;
	}
	}
}
//#################################################################//
void MC::getSmoothness(int intensity)
{
//	return;
	vector v;int i,j;
	int px,py,pz,factor;
	for(int k=0;k!=intensity;k++)
	{
	    S_L_E_E_P
		for(i=0;i<pointcount;i++)
		{
		if((tmp_point[i].idmax!=0)&&(tmp_point[i].parent==NULL))//&&(tmp_point[i].pos.x==1))
		{
				v.x=v.y=v.z=0;
				for(j=0;j<tmp_point[i].idmax;j++)
					v+=tmp_point[tmp_point[i].id[j]].p;
				v+=tmp_point[i].p;
				v/=tmp_point[i].idmax+1;
				tmp_point[i].q=v;
		}
		else
			tmp_point[i].q=tmp_point[i].p;

		 if((i&8191)==0)S_L_E_E_P 
		}
		S_L_E_E_P 
			
		for(i=0;i<pointcount;i++)
			tmp_point[i].p=tmp_point[i].q;
        }
}
//#################################################################//
void MC::recycle(void)//|<>
{
	// Empty voxel links
	//memset(tmp_point,0,pointcount*sizeof(MC_POINT));
	memset(tmp_voxels,-1,sx*sy*sz*4);
	tmp_scene->v_count=0;
	tmp_scene->v_indexcount=0;
	pointcount=0;
	surfacecount=0;
	indexcount=0;

	return;
	
	/*
	if(pointcount==0)return;

	// mov : movement by n voxels
	// clip : clipping volume of recycleable voxels

	int 		v_count=tmp_scene->v_count;
	int 		v_indexcount=tmp_scene->v_indexcount;
	int 		v_cachedcount=tmp_scene->v_cachedcount;
	int 		v_uncachedcount=tmp_scene->v_uncachedcount;
//	int	 	*v_indexc=tmp_scene->v_indexc;
	int	 	*v_indexs=tmp_scene->v_indexs;
	int	 	*v_cached=tmp_scene->v_cached;
	int	 	*v_uncached=tmp_scene->v_uncached;
	int		*v_dst=tmp_scene->v_dst;
	
	ivector 	*v_pointids=tmp_scene->v_pointids;
	vector 	*v_points=tmp_scene->v_points;
	vector 	*v_texco=tmp_scene->v_texco;
	int		*v_color=tmp_scene->v_color;
	int		*v_quads=tmp_scene->v_quads;
	//
	int 		v_count2=scene->v_count;
	int 		v_indexcount2=scene->v_indexcount;
	int 		v_cachedcount2=scene->v_cachedcount;
	int 		v_uncachedcount2=scene->v_uncachedcount;
//	int	 	*v_indexc2=scene->v_indexc;if(v_indexc2==0)return;
	int	 	*v_indexs2=scene->v_indexs;if(v_indexs2==0)return;
	int	 	*v_cached2=scene->v_cached;
	int	 	*v_uncached2=scene->v_uncached;
	int		*v_dst2=tmp_scene->v_dst;
	
	ivector 	*v_pointids2=scene->v_pointids;if(v_pointids2==0)return;
	vector 	*v_points2=scene->v_points;if(v_points2==0)return;
	vector 	*v_texco2=scene->v_texco;if(v_texco2==0)return;
	int		*v_color2=scene->v_color;if(v_color2==0)return;
	int		*v_quads2=tmp_scene->v_quads;

	
	ivector pos;
	
	int i,j;
	
	// for polygons which were connected to the parent!!
	if(clipend.x>sx-3)clipend.x=sx-3;
	if(clipend.y>sy-3)clipend.y=sy-3;
	if(clipend.z>sz-3)clipend.z=sz-3;
	
	if(clipstart.x<3)clipstart.x=3;
	if(clipstart.y<3)clipstart.y=3;
	if(clipstart.z<3)clipstart.z=3;
	
	// Empty voxel links
	memset(tmp_voxels,-1,sx*sy*sz*4);
	
//printf("recycle 1 Empty voxel links\n");
	//search recycleable points
	int newpointmax=0;
	for(i=0;i<pointcount;i++)
	{	
		tmp_point[i].dst=-1;
		pos=tmp_point[i].pos;
	}
	// Search recycleable surfaces
	int newsurfacemax=0;
	for(i=0;i<surfacecount;i++)
	{
		tmp_surface[i].dst=-1;
		tmp_surface[i].used=0;
		//int id[4];	//index
		
		int recyclecount=0;
		for(j=0;j!=4;j++)
		if(tmp_point[tmp_surface[i].id[j]].dst!=-1)
			recyclecount++;

		if(recyclecount==4)
		{
			tmp_surface[i].dst=newsurfacemax;
			newsurfacemax++;
		}
	}
	// Update Links between Points
	// Update Links between Points and Surfaces
	for(i=0;i<pointcount;i++)
	if(tmp_point[i].dst!=-1)
	{	
		// point - point 
		for(j=0;j!=tmp_point[i].idmax;j++)
		{
			int id=tmp_point[i].id[j];
			if(id!=-1)
			tmp_point[i].id[j]=tmp_point[id].dst;
		}
		// point - surface 
		for(j=0;j!=6;j++)
		{
			int id=tmp_point[i].poly[j];
			if(id!=-1)
			tmp_point[i].poly[j]=tmp_surface[id].dst;
			else
			tmp_point[i].poly[j]=-1;
		}
	}
	
	// Update Links between Quads
	// Update Marks of Quads
	for(i=0;i<surfacecount;i++)
	if(tmp_surface[i].dst!=-1)
	for(j=0;j!=4;j++)
	{
		//	int id[4];	//index to point
		//	int link[4];	//index to quad (up down left right)
		int id=tmp_surface[i].id[j];
		if(id!=-1)
		tmp_surface[i].id[j]=tmp_point[id].dst;

		id=tmp_surface[i].link[j];
		if(id!=-1)
		tmp_surface[i].link[j]=tmp_surface[id].dst;
	}
//printf("recycle 5 ic %d\n",v_indexcount2);

	//search recycleable Triangle Strips
	int newstripmax=0;
	//if(0)
	for(i=0;i<v_indexcount2;i++)
	{
		v_dst2[i]=-1;
		int found=0,check=(v_indexs2[i+1]-v_indexs2[i])/2;
		for(j=v_indexs2[i]/2;j<v_indexs2[i+1]/2;j++)
		{
			if( tmp_surface[v_quads2[j]].dst!=-1)
			{
				found++;
			}
		}
		if(found==check)
		{
			v_dst2[i]=newstripmax;
			newstripmax+=v_indexs2[i+1]-v_indexs2[i];
		}
	}
//printf("recycle 6\n");
	
	//compress recycleable Triangle Strips
	v_indexcount=0;
	//v_uncachedcount=0;
	//if(0)
	for(i=0;i<v_indexcount2;i++)
	{
		if(v_dst2[i]!=-1)
		{
			v_indexs[v_indexcount]=v_dst2[i];
			v_indexs[v_indexcount+1]=v_dst2[i+1];
			//v_indexc[v_indexcount]=v_indexc2[i];

			int src=v_indexs2[i];
			int dst=v_indexs[v_indexcount];
			int cnt=v_indexs2[i+1]-v_indexs2[i];
			memcpy(&v_pointids[dst],&v_pointids2[src],sizeof(ivector)*cnt);
			memcpy(&v_points[dst],&v_points2[src],sizeof(vector)*cnt);
			memcpy(&v_texco[dst],&v_texco2[src],sizeof(vector)*cnt);
			memcpy(&v_color[dst],&v_color2[src],sizeof(int)*cnt);
			memcpy(&v_quads[dst/2],&v_quads2[src/2],sizeof(int)*(cnt/2));
			
	//Mark Quads wich are parts of recycled TStrips
			
			for (int j=src/2;j!=(src/2)+(cnt/2);j++)
				tmp_surface[v_quads2[j]].used=v_indexcount+10;
			
			v_indexcount++;
		}
	}
//printf("recycle 7\n");
	
	// Compress recycled surfaces
	for(i=0;i<surfacecount;i++)
	if(tmp_surface[i].dst!=-1)
	{
		tmp_surface[tmp_surface[i].dst]=tmp_surface[i];
	}
	surfacecount=newsurfacemax;

	// Compress recycled points
	for(i=0;i<pointcount;i++)
	if(tmp_point[i].dst!=-1)
	{
		tmp_point[tmp_point[i].dst]=tmp_point[i];
	}
	pointcount=newpointmax;

	// Uncache scene
	for(i=0;i<v_cachedcount2;i++)
	{
		v_uncached[i]=v_cached2[i];
	}
	v_uncachedcount=v_cachedcount2;
	
	tmp_scene->v_indexcachedcount=0;
	tmp_scene->v_uncachedcount=v_uncachedcount;
	tmp_scene->v_cachedcount=0;
	
	indexcount=v_indexcount;
//	indexcount +
//	surfacecount +
//	pointcount  +
	tmp_scene->v_count=v_count;
	tmp_scene->v_indexcount=v_indexcount;
	
	recycledcount=v_indexcount;
	//printf("recycle finish\n");
*/
}
//#################################################################//
XMEM::XMEM(int elemsize,int indexstepsize,void** resultPtr)
{ 
	outerPtr=resultPtr;
	blocksize=elemsize;
	stepsize=indexstepsize;
	mem=malloc(blocksize*stepsize);
	if(mem==NULL){MessageBox(NULL, "XMEM", "Out Of Memory", MB_OK);exit(123);}
	memset(mem,0,blocksize*stepsize);
	*resultPtr=mem;
	index=stepsizeorig=stepsize;
}
//#################################################################//
void XMEM::check(int newindex)
{
	if(newindex+stepsizeorig < index) return;
	
	stepsize=stepsize*2;
	index=index+stepsize;
	mem=realloc(mem,blocksize*index);
	if(mem==NULL)exit(blocksize*index);
	*outerPtr=mem;
}
//#################################################################//
XMEM::~XMEM()
{
///	free(mem);
}
//#################################################################//
