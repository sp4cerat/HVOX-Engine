#ifndef _ENGINE_class
#define _ENGINE_class

//#################################################################//
#include "BENCHMARK.h"
#include "BMP.h"
#include "GFX.h"
#include "HOUSE.h"
#include "MC.h"
//#################################################################//
#include <pthread.h>
//#include <unistd.h>//nice!!
//#################################################################//

class ENGINE
{
ENGINE();

main_loop();

public:

private:

public:

private:

};

#endif//ENGINE

//#################################################################//
textureImage*	show_texture;
textureImage*	show_texture_l;
textureImage*	show_texture_r;
//#################################################################//
HOUSE* housep;
HOUSE* housep2;
HOUSE* housep4;
HOUSE* housep8;
HOUSE* housep16;
HOUSE* housep32;
HOUSE* housep64;
HOUSE* housep128;
MC* mcp;
//#################################################################//
int updateframetime;
int updateframecount;
int updatefps=0;
//#################################################################//
double stereoperformance=0;//in Milliseconds
//#################################################################//
void voxelGetFps(void)
{
	int timenow;
	time((time_t*)&timenow);
	if(timenow!=updateframetime)
	{
		updateframetime=timenow;
		updatefps=updateframecount;
		updateframecount=0;
	}
	updateframecount++;
}
//#################################################################//
int xscale=2;//4;

int newhx=0;
int newhy=0;
int newhz=0;

void houseupdate(HOUSE* houseptr)
{
	voxelGetFps();

	if(!houseptr->setPos(newhx/2,newhy/2,newhz/2)){return;}
	int i;
    for(int j=0;j!=90;j++)
	    for(int i=40;i>=0;i--)
	    {
		    int sx=(((j*75335)^75633)%1000)-500;
		    int sy=i*20-(((j*86734)^23467)%100)+50;
		    int sz=(((j*45782)^96677)%1000)-500;
		    int dy=80;
		    int dx=2000/(abs(i)*((((j*34567)^64336)%20)+10)+20)-10;
		    int dz=dx;
		    sz-=dx/2;
		    sx-=dx/2;
		    houseptr->shape(1,0,xscale*sx,xscale*sy,xscale*sz,xscale*(sx+dx),xscale*(sy+dy),xscale*(sz+dz));
        }

    for(int j=0;j!=100;j++)
    {
	    int sx=(((j*87642)^94347)%600)-300;
	    int sy=(((j*34784)^27874)%250)-150;
	    int sz=(((j*96449)^12367)%600)-300;
	    int dy=(((j*36580)^64562)%80)+70;
	    int dx=dy;
	    int dz=dx;
	    dy=-sy+20;
	    houseptr->shape(1,0,xscale*sx,xscale*sy,xscale*sz,xscale*(sx+dx),xscale*(sy+dy),xscale*(sz+dz));
    }

    for(int j=0;j!=900;j++)
    {
	    int sx=(((j*87642)^94347)%1000)-500;
	    int sy=(((j*34784)^27874)%400)-370;
	    int sz=(((j*96449)^12367)%1000)-500;
	    int dy=(((j*36580)^64562)%50)+50;
	    int dx=dy;
	    int dz=dx;
	    houseptr->shape(1,1,xscale*sx,xscale*sy,xscale*sz,xscale*(sx+dx),xscale*(sy+dy),xscale*(sz+dz));
    }
	houseptr->getPolys();
}


void *voxelUpdate(void * arg)
{
//	nice(19);
	
	int scount=0;
	while(1)
	{
		houseupdate(housep128);
		houseupdate(housep64);
		houseupdate(housep32);
		houseupdate(housep16);
		houseupdate(housep8);
//		houseupdate(housep4);
//		houseupdate(housep2);
//		houseupdate(housep);
	}
}
//#################################################################//
int main (int argc, char **argv)
{
	INIT_MATRIX_VECTOR_CALCULATION();

	GFX gfx(1024,768,32,false); // create window + initialize OpenGL
	
	int nehe=gfx.loadTex("Data/ground3.bmp");

	running=true;
	
	//int xscale=2;
	int xsize=xscale*32+2;
	HOUSE house(xsize,xsize,xsize,NULL,NULL,0,1/*scale1*/);housep=&house;
	HOUSE house2(xsize,xsize,xsize,NULL,NULL,0,2/*scale1*/);housep2=&house2;
	HOUSE house4(xsize,xsize,xsize,NULL,NULL,0,4/*scale1*/);housep4=&house4;
	HOUSE house8(xsize,xsize,xsize,NULL,NULL,0,8/*scale1*/);housep8=&house8;
	HOUSE house16(xsize,xsize,xsize,NULL,NULL,0,16/*scale1*/);housep16=&house16;
	HOUSE house32(xsize,xsize,xsize,NULL,NULL,0,32/*scale1*/);housep32=&house32;
	HOUSE house64(xsize,xsize,xsize,NULL,NULL,0,64/*scale1*/);housep64=&house64;
	HOUSE house128(xsize,xsize,xsize,NULL,NULL,0,128/*scale1*/);housep128=&house128;
	
	house128.setChild(&house64);
	house64.setChild(&house32);
	house32.setChild(&house16);
	house16.setChild(&house8);
	house8.setChild(&house4);
	house4.setChild(&house2);
	house2.setChild(&house);

	houseupdate(housep64);
	houseupdate(housep32);
	houseupdate(housep16);
	houseupdate(housep8);
	houseupdate(housep4);
	houseupdate(housep2);
	houseupdate(housep);
	
	house.getPolys();
	house2.getPolys();
	house4.getPolys();
	house8.getPolys();
	house16.getPolys();
	house32.getPolys();
	house64.getPolys();
//	house128.getPolys();

	
	pthread_t 		voxel_thread;
	if (pthread_create(&voxel_thread,		// thread struct             
			   NULL,//&voxel_thread_attr,		// default thread attributes 
			   voxelUpdate,			// start routine             
			   NULL))//&voxel_thread_attr_sched))				// Param
	{	
		pthread_join(voxel_thread,NULL);
	}
	
	SCENE 
	*scene1=house.scene,
	*scene2=house2.scene,
	*scene3=house4.scene,
	*scene4=house8.scene,
	*scene5=house16.scene,
	*scene6=house32.scene,
	*scene7=house64.scene,
	*scene8=house128.scene;



	while ( !  gfx.scene() )
	{
		newhx=int(-gfx.mov.x/2);
		newhy=int(-gfx.mov.y/2);
		newhz=int(-gfx.mov.z/2);
		
		gfx.setTex(0);
		
		int x,y,z;
		gfx.zBuffer(3);
		
		gfx.drawFog(0);
		gfx.drawSky();
		gfx.drawFog(4);
		gfx.setTex(nehe);
		
		matrix kamera;
		kamera =permutation;
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(kamera.a);
		glMatrixMode(GL_MODELVIEW);
		matrix view=ID;
		view*=vector((float)gfx.height/gfx.width,1,1);
		view<<gfx.rot;
		view+=gfx.mov;
		glLoadMatrixf(view.a);
		gfx.framecachecount=0;
        
//		scene8=house128.scene;
//		gfx.drawVoxels(scene8,0);
//		scene7=house64.scene;
//		gfx.drawVoxels(scene7,0);
		scene6=house32.scene;
		gfx.drawVoxels(scene6,0);
		scene5=house16.scene;
		gfx.drawVoxels(scene5,0);
		scene4=house8.scene;
		gfx.drawVoxels(scene4,1);
		scene3=house4.scene;
//		gfx.drawVoxels(scene3,1);
      	scene2=house2.scene;
//		gfx.drawVoxels(scene2,1);
		scene1=house.scene;
//		gfx.drawVoxels(scene1,1);
		
		view-=vector(gfx.mov.x,0,gfx.mov.z);
		glLoadMatrixf(view.a);
//		gfx.Water(gfx.mov,vector(0.1f,0.4f,0.25f));
		gfx.Water(gfx.mov,vector(0.4f,0.2f,0.1f));
//		gfx.Hangs(house32.mc->tmp_point, house32.mc->pointcount);
//		gfx.Plants(house32.mc->tmp_point, house32.mc->pointcount);
		
		gfx.zBuffer(0);

		gfx.text(0.6f,-1, 0.025f,0.06f,  1,1,0,1, "key %d",gfx.keycode  );
		gfx.text(0.6f,-1+0.07f, 0.025f,0.06f,  1,1,0,1, "Polys %d",gfx.polycount);
		gfx.text(0.6f,-1+0.14f, 0.025f,0.06f,  1,1,0,1, "MPolys/s %f VoxelFPS %d",((float)gfx.mpolycount)/1000000,updatefps);
		gfx.text(0.6f,-1+0.21f, 0.025f,0.06f,  1,1,0,1, "xyz %d %d %d ",(int)gfx.mov.x,(int)gfx.mov.y,(int)gfx.mov.z);
		gfx.text(0.6f,-1+0.28f, 0.025f,0.06f,  1,1,0,1, "VoxBoxFps %d",updatefps);
		gfx.text(0.6f,-1+0.35f, 0.025f,0.06f,  1,1,0,1, "Recycled %d",house8.mc->recycledcount);
		gfx.text(0.6f,-1+0.42f, 0.025f,0.06f,  1,1,0,1, "Cached %d",gfx.cachedtstrips);
	};

	printf("end loop\n");
	return 0;
}
//#################################################################//
