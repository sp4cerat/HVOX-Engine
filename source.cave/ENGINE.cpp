///////////////////////////////////////////
#include "ENGINE.h"
///////////////////////////////////////////
#define PBUF_RESOLUTION 1024
///////////////////////////////////////////
    bool globalUpdate=true;

	int xscale;

	int newhx=0;
	int newhy=0;
	int newhz=0;

	int updateframetime;
	int updatefps;
	int updateframecount;

	HOUSE* housep[ClipBoxCount];

	InfoType*	ENGINE::info;
	bool		ENGINE::threadRunning; // voxelUpdateThread
	int 		ENGINE::instances=0; // voxelUpdateThread

	ENGINE*		ENGINE::self;
    int         ENGINE::randSize = 16384;
    int         ENGINE::randArray [ 16384 ];
///////////////////////////////////////////
ENGINE::ENGINE(InfoType *globalInfo){
	info=globalInfo;
	INIT_MATRIX_VECTOR_CALCULATION();
}
///////////////////////////////////////////
ENGINE::~ENGINE(){};
///////////////////////////////////////////
void ENGINE::renderWaterTex()
{
}
///////////////////////////////////////////
int ENGINE::init(){

	if(threadRunning)return 0;

	self=this;

	threadRunning=true;

	gfx=new GFX(info);
	gfx->initGL();

	xscale=2;
	newhx=newhy=newhz=0;

	info->view.pos.x=614.34;
	info->view.pos.y=20;
	info->view.pos.z=-170;

	info->view.rot.x=0.26;
	info->view.rot.y=-0.54;
	info->view.rot.z=0;

	groundtexture=gfx->loadTex("Data/terrain/sand.bmp");

	shootTex=gfx->loadTex("Data/shoot.bmp");

	terrainTex0=gfx->loadTex("Data/terrain/earth.bmp");
	terrainTex1=gfx->loadTex("Data/terrain/grass.bmp");
	terrainTex2=gfx->loadTex("Data/terrain/sand.bmp");
	terrainTexDetail0=gfx->loadTex("Data/terrain/detail1grey.bmp");
	terrainTexDetail1=gfx->loadTex("Data/terrain/detail2.bmp");
	terrainTexDetail2=gfx->loadTex("Data/terrain/detail3.bmp");
	terrainTexSelect=gfx->loadTex("Data/terrain/bigselect.bmp",GFX::TEX_NORMALIZE);

	int xsize=xscale*32+2;

	//housep=new HOUSE(xsize,xsize,xsize,NULL,NULL,0,1);

    int t;

    for ( t = 0 ; t!=ClipBoxCount  ; t++ )
    {
        housep[t]=new HOUSE(info,xsize,xsize,xsize,NULL,NULL,0,1<<(t));
        if (t>0)
        {
            housep[t]->setChild(housep[t-1]);
        }
        scene [t] = housep[t]->scene;
        housep[t]->updated = false;
    }
	
	LPDWORD lpThreadId=NULL;
	Threadhandle=
	CreateThread(NULL,0,voxelUpdate,NULL,0,(LPDWORD )(&lpThreadId));

	entityCount=0;
	for(t=0;t<MAX_ENTITIES;t++)
	{
		entity[t].id=-1;
		entity[t].pos=vector(0,0,0);
	}

	shootCount=0;
	for(t=0;t<MAX_ENTITIES;t++)
	{
		shoot[t].id=-1;
		shoot[t].pos=vector(0,0,0);
		shoot[t].add=vector(0,0,0);
	}

	loadEntities("entities.cave1.lst");

	weapon=new Fisch("weapon",true);

    wireframe = 0;
    skybox  = 1;
    useTexture  = 1;
    useFog  = 1;

    getSeed();
}
///////////////////////////////////////////
void ENGINE::shootHandler(){
}
void ENGINE::shootPainter(matrix view){
}
///////////////////////////////////////////
void ENGINE::getSeed()
{
    int a = timeGetTime();
    int b = timeGetTime();

    for ( int i = 0 ; i != randSize ; i++ )
    {   
        randArray[i] = i*11 - 3453621 + a ^ 353632;
        a = a*23+26348345+i*3;
        randArray[i] += b;
    }
}
///////////////////////////////////////////
int ENGINE::run (){

//	pbufferEnable( PBUF_RESOLUTION ,PBUF_RESOLUTION );

	info->view.rot.x+=(float)info->mouse.dx/2500;
	info->view.rot.y+=(float)info->mouse.dy/2500;

	vector add=vector(0,0,0);//walk

	for(int n=0;n<8;n++)// 1..9
	if(info->key[2+n].pressed)
	if(info->key[2+n].changed){ 
		entity[entityCount].pos=info->view.pos;
		entity[entityCount].pos.x*=-1;
		entity[entityCount].pos.y*=-1;
		entity[entityCount].pos.z*=-1;
		entity[entityCount].id=n;
		entityCount++;
	}
	if(info->key[14].changed)//DEL
	if(info->key[14].pressed)
	if(entityCount>0)
	{
		entityCount--;
		entity[entityCount].id=-1;
	}

    float accel=1;

	if(info->key[183].pressed)
	if(info->key[183].changed)gfx->screenshot();

    if(info->key[ 17].pressed)
	if(info->key[ 17].changed){ wireframe^=1; }     //W

    if(info->key[ 31].pressed)
	if(info->key[ 31].changed){ skybox^=1; }        //S

    if(info->key[ 20].pressed)
	if(info->key[ 20].changed){ useTexture^=1; }    //T

    if(info->key[ 33].pressed)
	if(info->key[ 33].changed){ useFog^=1; }        //F

    if(info->key[ 19].pressed)
	if(info->key[ 19].changed){ getSeed(); globalUpdate=true;}        //R

	if(info->key[ 54].pressed){ 
            accel=10; 
            //housep[0]->updated = false;
            //housep[1]->updated = false;
    }
	if(info->key[205].pressed){ add.x-=accel*(float)info->deltatime/144.63f; }
	if(info->key[203].pressed){ add.x+=accel*(float)info->deltatime/144.63f; }
	if(info->key[200].pressed){ add.z-=accel*(float)info->deltatime/144.63f; }
	if(info->key[208].pressed){ add.z+=accel*(float)info->deltatime/144.63f; }
	matrix tmp=ID;
	tmp>>info->view.rot;
	add*=tmp;
	info->view.pos+=add;

	gfx->clear();
	gfx->framecachecount=0;

	newhx=int(-info->view.pos.x/2);
	newhy=int(-info->view.pos.y/2);
	newhz=int(-info->view.pos.z/2);
	
	gfx->setTex(0);
	
	int x,y,z;

	matrix kamera;
	kamera =permutation;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(kamera.a);
	glMatrixMode(GL_MODELVIEW);

	gfx->zBuffer(3);
//	gfx->drawFog(0);

    if ( skybox )
	    gfx->drawSky(false);

	gfx->zBuffer(3);

	gfx->drawFog(4);
	matrix view=ID;
	view*=vector(10,10,10);
	view*=vector((float)info->win.height/info->win.width,1,1);
	view<<info->view.rot;
	view+=info->view.pos;
	glLoadMatrixf(view.a);

	glBindTexture(GL_TEXTURE_2D,groundtexture);					// Bind To The Blur Texture

	glLoadMatrixf(view.a);

	gfx->drawFog(0);

    if ( useFog )
        gfx->drawFog(1);


  
//	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glBlendFunc(GL_DST_ALPHA,GL_ONE);

    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D,0);					

    if ( useTexture )
    {
        glBindTexture(GL_TEXTURE_2D,groundtexture);					
	    gfx->setTerrainTex(
			    terrainTex0,
			    terrainTex1,
			    terrainTex2,
			    terrainTexDetail0,
			    terrainTexDetail1,
			    terrainTexDetail2,
			    terrainTexSelect);
    }

    int minBox = 0;
    if(info->key[ 54].pressed)
    {
        //minBox = 2;
    }

    int start=ClipBoxCount-1;
    while((housep[start]->updated)&&(start>minBox))start--;
   	gfx->drawVoxels(scene[start],1,start++);
    for ( ; start<ClipBoxCount ; start++ )
    	gfx->drawVoxels(scene[start],0,start);

  /*
	gfx->setTerrainTex(
			0,
			0,
			0,
			0,
			0,
			0,
			0);*/
//	glBindTexture(GL_TEXTURE_2D,-1);					// Bind To The Blur Texture
	gfx->clearMultiTex();

    if ( wireframe )
    {
	    glEnable(GL_BLEND);
	    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	    //glBlendFunc(GL_DST_ALPHA,GL_ONE);

        int start=ClipBoxCount-1;
        while((housep[start]->updated)&&(start>minBox))start--;

   	    gfx->drawVoxels(scene[start],1,start++,GL_LINE_STRIP);

        for ( ; start<ClipBoxCount ; start++ )
    	    gfx->drawVoxels(scene[start],0,start,GL_LINE_STRIP);

	    glDisable(GL_BLEND);
    }

	gfx->clearMultiTex();

	matrix view2=view;
	view2-=vector(info->view.pos.x,0,info->view.pos.z);
	glLoadMatrixf(view2.a);
	gfx->zBuffer(0);
	gfx->drawFog(0);
	int k=0;
	for(k=0;k<256;k++)
	 if(info->key[k].pressed)
	  break;

    int t=0;
    for (t=0;t<ClipBoxCount;t++)
    {
    	housep[t]->flip();
    	scene[t]=housep[t]->scene;
    }

	gfx->clearMultiTex();
    
	//gfx->text(0.45f,-1+0.00f, 0.025f,0.06f,  1,1,0,1, "Key %d E %d",k,entityCount);
	gfx->text(0.45f,-1+0.07f, 0.025f,0.06f,  1,1,0,1, "Polys %d",gfx->polycount);
    gfx->polycount=0;
	//gfx->text(0.45f,-1+0.14f, 0.025f,0.06f,  1,1,0,1, "MPolys/s %f",((float)gfx->polycount*gfx->fps)/(1000000));//,updatefps);
	//gfx->text(0.45f,-1+0.21f, 0.025f,0.06f,  1,1,0,1, "p %3.0f %3.0f %3.0f ",info->view.pos.x,info->view.pos.y,info->view.pos.z);
	//gfx->text(0.45f,-1+0.28f, 0.025f,0.06f,  1,1,0,1, "r %3.2f %3.2f %3.2f ",info->view.rot.x,info->view.rot.y,info->view.rot.z);
	//gfx->showFps();
    

	if (info->key[97].changed){
		gfx->screenshot();
	}
	return 0;
}
///////////////////////////////////////////
void ENGINE::saveEntities(char* filename){
	FILE* fn;
	
	if ((fn = fopen (filename,"wb")) == NULL) return;

	for(int t=0;t!=entityCount;t++)
		fprintf(fn,"id %d pos %3.3f %3.3f %3.3f\n",
			entity[t].id,entity[t].pos.x,entity[t].pos.y,entity[t].pos.z);

	fclose(fn);
}
///////////////////////////////////////////
void ENGINE::loadEntities(char* filename){

	FILE* fn;

	if(filename==NULL)		return ;
	if((char)filename[0]==0)	return ;

	if ((fn = fopen(filename, "rb")) == NULL) return;

	char line[1000];
	int t=0;
	while(fgets( line, 1000, fn ) != NULL)
	{
		if(sscanf(line,"id %d pos %f %f %f",
			&entity[t].id,
			&entity[t].pos.x,
			&entity[t].pos.y,
			&entity[t].pos.z)==4) t++;
	}
	fclose(fn);

	entityCount=t;
}

///////////////////////////////////////////
int ENGINE::quit(){

	saveEntities("entities.cave1.lst");

	while(threadRunning)Sleep(100);
	Sleep(1000);	
	return 0;
}
///////////////////////////////////////////
bool ENGINE::houseupdate(HOUSE* houseptr,bool forceUpdate)
{
	//voxelGetFps();
	Sleep(1);

    if(forceUpdate) houseptr->reset();


	if(!houseptr->setPos(newhx/2,newhy/2,newhz/2))
		return false;


	if(houseptr->scale>=16)
	{
		int i,j;

		Sleep(1);
        
		houseptr->shape(0,0,  -3000,-3000,-3000,  2000,2000,2000);

		Sleep(1);

        int index=0;

		
		for(j=0;j!=4000;j++)
		{
		//	int sx=(((j*52334)^23568)%4000)-2000;
		//	int sy=(((j*74357)^85467)%270)-70;
		//	int sz=(((j*15865)^23466)%4000)-2000;
		//	int dy=(((j*74746)^45643)%120)+30;
			int sx=(abs(randArray[(j*9+2) % randSize])%2000)-1000;
			int sy=-20000/(100+abs(randArray[(j*3+1) % randSize])%200);
			int sz=(abs(randArray[(j*5+4) % randSize])%2000)-1000;
			int dy=abs(sy);//(abs(randArray[(j*7+5) % randSize])%50)+30;
			int dx=40;
			int dz=40;
			
			//dy=dy*3/2;
			houseptr->shape(1,1,xscale*sx,xscale*sy,xscale*sz,xscale*(sx+dx),xscale*(sy+dy),xscale*(sz+dz));
            if ((j&511)==0) Sleep(1);
		}
		Sleep(1);

	}							
	houseptr->getPolys();

    houseptr->updated=true;

	Sleep(1);
	return true;
}
///////////////////////////////////////////
DWORD WINAPI ENGINE::voxelUpdate( LPVOID lpParameter )
{

	while(!info->exit)

	{
        if ( globalUpdate == true )
        {
            for (int t=ClipBoxCount-1;t>=0;t--)
            {
    	        houseupdate(housep[t],true);
            }
            globalUpdate = false;
        }

        int minBox = 0;
        //if(info->key[ 54].pressed) minBox = 2;

        for (int t=ClipBoxCount-1;t>=minBox;t--)
        {
    	    houseupdate(housep[t]);

    	  //  if (t!=minBox   )houseupdate(housep[minBox  ]);
    	  //  if (t!=minBox+1 )houseupdate(housep[minBox+1]);
        }
	}

	threadRunning=false;
	Sleep(1000);
	//ExitThread(0);
	exit(0);
	return 0;
}
///////////////////////////////////////////
void ENGINE::voxelGetFps(void)
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
///////////////////////////////////////////

