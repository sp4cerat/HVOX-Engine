//#################################################################//
#include "HOUSE.h"
//#################################################################//
//#################################################################//
HOUSE::HOUSE()
{
	parent=child=NULL;
    scene=NULL ;
}
//#################################################################//
bool HOUSE::setPos(int x,int y,int z)
{  //x=y=z=0;    

//	x+=scale/1;
//	y+=scale/1;
//	z+=scale/1;
	
	x/=scale;
	y/=scale;
	z/=scale;
/*
	if(parent!=NULL)
	{
		//parent->newposx=x/4;
		//parent->newposy=y/4;
		//parent->newposz=z/4;
		//parent->setPos(x/2,y/2,z/2);
	}
*/	
	x*=2;
	y*=2;
	z*=2;

	if(!firstrun)
	if(posx/8==x/8)
	if(posy/8==y/8)
	if(posz/8==z/8)return false;
	
	posx_old=posx;
	posy_old=posy;
	posz_old=posz;
	
	posx=x;posy=y;posz=z;
		
	mc->posx=x;
	mc->posy=y;
	mc->posz=z;
	
	minx=-sx*scale/2+x*scale;
	miny=-sy*scale/2+y*scale;
	minz=-sz*scale/2+z*scale;
	maxx=sx*scale/2+x*scale;
	maxy=sy*scale/2+y*scale;
	maxz=sz*scale/2+z*scale;
	
	int dx=abs(posx_old-posx);
	int dy=abs(posy_old-posy);
	int dz=abs(posz_old-posz);
		
	int copyx=sx-dx;
	int copyy=sy-dy;
	int copyz=sz-dz;
	
	if((copyx<=0)||(copyy<=0)||(copyz<=0)||(firstrun))
	{
		memset(data,0,sx*sy*sz);
		for(int i=0;i!=4;i++)
		{
			clipstart[i].x=0;clipend[i].x=0;
			clipstart[i].y=0;clipend[i].y=0;
			clipstart[i].z=0;clipend[i].z=0;
		}
		clipend[0].x=sx;
		clipend[0].y=sy;
		clipend[0].z=sz;
		
		return true;
	}
	
	int i,j,k;
	int add_x_src=0;
	int add_y_src=0;
	int add_z_src=0;
	int add_x_dst=0;
	int add_y_dst=0;
	int add_z_dst=0;

	if(posx>posx_old)add_x_src=dx;else add_x_dst=dx;
	if(posy>posy_old)add_y_src=dy;else add_y_dst=dy;
	if(posz>posz_old)add_z_src=dz;else add_z_dst=dz;

	clipstart[3].x=add_x_dst;
	clipstart[3].y=add_y_dst;
	clipstart[3].z=add_z_dst;

	clipend[3].x=add_x_dst+copyx;
	clipend[3].y=add_y_dst+copyy;
	clipend[3].z=add_z_dst+copyz;

	// Clip Box 1
	if(add_x_dst==0)
		{clipstart[0].x=copyx;clipend[0].x=copyx+dx;}
	else
		{clipstart[0].x=0;clipend[0].x=dx;}
	clipstart[0].y=add_y_dst;clipend[0].y=add_y_dst+copyy;
	clipstart[0].z=add_z_dst;clipend[0].z=add_z_dst+copyz;

	// Clip Box 2
	if(add_y_dst==0)
		{clipstart[1].y=copyy;clipend[1].y=copyy+dy;}
	else
		{clipstart[1].y=0;clipend[1].y=dy;}
	clipstart[1].x=0;clipend[1].x=sx;
	clipstart[1].z=add_z_dst;clipend[1].z=add_z_dst+copyz;

	// Clip Box 3
	if(add_z_dst==0)
		{clipstart[2].z=copyz;clipend[2].z=copyz+dz;}
	else
		{clipstart[2].z=0;clipend[2].z=dz;}
	clipstart[2].x=0;clipend[2].x=sx;
	clipstart[2].y=0;clipend[2].y=sy;

	//<
	int sxsy=sx*sy;

	// Copy
	memset(data_tmp,0,sx*sy*sz);

	mc->mov.x=posx_old-posx;
	mc->mov.y=posy_old-posy;
	mc->mov.z=posz_old-posz;
		
	mc->clipstart.x=add_x_src;
	mc->clipstart.y=add_y_src;
	mc->clipstart.z=add_z_src;
		
	mc->clipend.x=add_x_src+copyx;
	mc->clipend.y=add_y_src+copyy;
	mc->clipend.z=add_z_src+copyz;
		
	for(k=0;k!=copyz-0;k++)
	for(j=0;j!=copyy-0;j++)
	for(i=0;i!=copyx-0;i++)
	{
		int srcofs=
			(i+add_x_src)+
			(j+add_y_src)*sx+
			(k+add_z_src)*sxsy;
		int dstofs=
			(i+add_x_dst)+
			(j+add_y_dst)*sx+
			(k+add_z_dst)*sxsy;
		data_tmp[dstofs]=data[srcofs];
	}
	// Bufferswap
	char* tmp=data_tmp;
	data_tmp=data;
	data=tmp;

	mc->data_src=data;
	
	return true; // pos changed
	
}
//#################################################################//
void HOUSE::setParent(HOUSE* parenthouse)
{
	parent=parenthouse;
	parenthouse->child=this;
	
	mc->parent=parenthouse->mc;
	mc->scene->parent=parenthouse->mc;
	mc->tmp_scene->parent=parenthouse->mc;
	parenthouse->mc->child=mc;
}
//#################################################################//
void HOUSE::setChild(HOUSE* childhouse)
{
	child=childhouse;
	childhouse->parent=this;

	mc->child=childhouse->mc;
	childhouse->mc->parent=mc;

	mc->scene->child=&childhouse->mc->scene1;
	mc->tmp_scene->child=&childhouse->mc->scene1;
}
//#################################################################//
void HOUSE::getPolys()
{

	mc->getPolys();
	
//	if(child!=NULL)child->getPolys();

	mc->flippingDone=false;
    mc->preparingDone=true;

	while((!mc->flippingDone)&&(!info->exit))Sleep(1);

	if(info->exit) exit(0);
	

}
//#################################################################//
void HOUSE::reset()
{   
	firstrun=true;
    clear();
}
//#################################################################//
bool HOUSE::flip()
{
	if(!mc->preparingDone)return false;

	mc->swapScene();
	scene=mc->scene;

    mc->preparingDone=false;
	mc->flippingDone=true;

	firstrun=false;
	return true;
}
//#################################################################//
HOUSE::HOUSE(InfoType *newInfo,int sizex,int sizey,int sizez,ivector *connnectors,int *con_types,int con_count,int scalefactor)
{
	firstrun=true;

	info=newInfo;

	posx=
	posy=
	posz=0;

	sx=sizex;
	sy=sizey;
	sz=sizez;
	scale=scalefactor;
	data=(char*)malloc((sizex)*(sizey)*(sizez));
	data_tmp=(char*)malloc((sizex)*(sizey)*(sizez));
	
	parent=child=NULL;
	mc=new MC(sx,sy,sz,scale,data);
	scene=mc->scene;
	
	setPos(0,0,0);
	
}
//#################################################################//
void HOUSE::clear()
{
	memset(data,0,sx*sy*sz);
}
//#################################################################//
HOUSE::~HOUSE()
{
}
//#################################################################//
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
/*
void HOUSE::block(int x1,int y1,int z1,int x2,int y2,int z2,char* )
{
 if(x2<=x1)return;
 if(y2<=y1)return;
 if(z2<=z1)return;

 if(x2<minx)return;
 if(y2<miny)return;
 if(z2<minz)return;

 if(maxx<x1)return;
 if(maxy<y1)return;
 if(maxz<z1)return;
 
 for(int x=x1;x<x2;x++)
 for(int y=y1;y<y2;y++)
 for(int z=z1;z<z2;z++)
 {
 
 }
}
*/
void HOUSE::shape(int type,int mode,int x1,int y1,int z1,int x2,int y2,int z2)
{/*
x1=(x1/8)*8;
y1=(y1/8)*8;
z1=(z1/8)*8;
x2=(x2/8)*8;
y2=(y2/8)*8;
z2=(z2/8)*8;
/*if(mode==1)
{
	x1-=7;
	y1-=7;
	z1-=7;
	x2+=7;
	y2+=7;
	z2+=7;
}  */

 if(x2<=x1)return;
 if(y2<=y1)return;
 if(z2<=z1)return;

 if(x2<minx)return;
 if(y2<miny)return;
 if(z2<minz)return;

 if(maxx<x1)return;
 if(maxy<y1)return;
 if(maxz<z1)return;

// if(child!=NULL)child->shape(type,mode,x1,y1,z1,x2,y2,z2);
 
 x1-=minx;x1/=scale;
 y1-=miny;y1/=scale;
 z1-=minz;z1/=scale;
 x2-=minx;x2/=scale;
 y2-=miny;y2/=scale;
 z2-=minz;z2/=scale;

/*
 x1-=minx-1*scale/2;x1/=scale;
 y1-=miny-1*scale/2;y1/=scale;
 z1-=minz-1*scale/2;z1/=scale;
 x2-=minx-1*scale/2;x2/=scale;
 y2-=miny-1*scale/2;y2/=scale;
 z2-=minz-1*scale/2;z2/=scale;
*/
 shape_raw(type,mode,x1,y1,z1,x2,y2,z2,0);
 shape_raw(type,mode,x1,y1,z1,x2,y2,z2,1);
 shape_raw(type,mode,x1,y1,z1,x2,y2,z2,2);
  
}
 
// after coordinate transformation
void HOUSE::shape_raw(int type,int mode,int x1,int y1,int z1,int x2,int y2,int z2,int block)
{
 if(x2<clipstart[block].x)return;
 if(y2<clipstart[block].y)return;
 if(z2<clipstart[block].z)return;
 
 if(clipend[block].x<x1)return;
 if(clipend[block].y<y1)return;
 if(clipend[block].z<z1)return;
 
 float midx=(x1+x2)/2;
 float midy=(y1+y2)/2;
 float midz=(z1+z2)/2;
 float sizx=x2-x1;
 float sizy=y2-y1;
 float sizz=z2-z1;

 if(x1<clipstart[block].x)x1=clipstart[block].x;
 if(y1<clipstart[block].y)y1=clipstart[block].y;
 if(z1<clipstart[block].z)z1=clipstart[block].z;
 
 if(clipend[block].x<x2)x2=clipend[block].x;
 if(clipend[block].y<y2)y2=clipend[block].y;
 if(clipend[block].z<z2)z2=clipend[block].z;
 
 if(x2<=x1)return;
 if(y2<=y1)return;
 if(z2<=z1)return;

 bool inside;
 
 float sizcmp=(sizx*sizx+sizy*sizy+sizz*sizz)/4;
 
 int offset,x,y,z;
 
 int offset_add=1+(1+1*sy)*sx;
 int xsqr=0,ysqr=0,zsqr=0;
                    
 for(x=x1;x!=x2;x++)
 {
 	if(type==1){xsqr=int((x-midx)*2*256/(sizx));xsqr*=xsqr;}
	
	 for(y=y1;y!=y2;y++)
	 {
	 	if(type==1){ysqr=int((y-midy)*2*256/(sizy));ysqr=ysqr*ysqr+xsqr;}
		
		 for(z=z1;z!=z2;z++)
		 {
		  inside=false;
			/*
			# TYPE 0 : CUBE
			# TYPE 1 : BALL
			# TYPE 2 : CYLINDER X
			# TYPE 3 : CYLINDER Y
			# TYPE 4 : CYLINDER Z
			*/
		  if(type==0)
			inside=true;
		  if(type==1)
/*		  if(
			sqr((float)(x-midx)*2/(sizx))+
			sqr((float)(y-midy)*2/(sizy))+
			sqr((float)(z-midz)*2/(sizz))<1)*/
			{
			  zsqr=int((z-midz)*2*256/(sizz));zsqr=zsqr*zsqr+ysqr;
			  if( zsqr  <= 256*256)
				inside=true;
			}
			/*	
			# MODE 0 : ADD
			# MODE 1 : SUB
			# MODE 2 : XOR
			# MODE 3 : AND
			*/

		 if(inside)
		 {
			  offset=x+(y+z*sy)*sx;//+offset_add;

			  if(mode==0)
			   data[offset]=type+1;
			  if(mode==1)
			   data[offset]=0;
			  if(mode==2)
			  {
			   if (data[offset]!=0)
				data[offset]=0;
			   else data[offset]=type+1;
			  }
		 }else
		  if(mode==3)
		  {
			  offset=x+(y+z*sy)*sx;//+offset_add;
				data[offset]=0;
		  }

		 }
	 } 
 }
}
//#################################################################//
