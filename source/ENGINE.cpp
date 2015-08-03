///////////////////////////////////////////
#include "ENGINE.h"
#include "rle.h"
///////////////////////////////////////////
#define PBUF_RESOLUTION 1024
///////////////////////////////////////////
	int g_voxel_size_x = 256;
	int g_voxel_size_y = 256;
	int g_voxel_size_z = 256;
	char* g_voxel_data=NULL;	
///////////////////////////////////////////
    bool globalUpdate=true;

	int xscale=SCALE_FACTOR;

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

//	wglSwapIntervalEXT(0);
//	timeBeginPeriod(1);
	RLE rle;
//	rle.load("Landscape.rle");
//	g_voxel_data = rle.uncompress();//get_data(tx,ty,tz);
//	g_voxel_data = 	(char*)rle.load_raw_16(128,128,160,"tooth_128x128x160.raw");
	g_voxel_data = 	(char*)rle.load_raw_8(256,256,128,"Bonsai.raw");
	g_voxel_size_x = rle.sx;
	g_voxel_size_y = rle.sy;
	g_voxel_size_z = rle.sz;

	if(threadRunning)return 0;

	self=this;

	threadRunning=true;

	gfx=new GFX(info);
	gfx->initGL();

	newhx=newhy=newhz=0;

	info->view.pos.x=-786;
	info->view.pos.y= 344;
	info->view.pos.z=-960;

	info->view.pos.x=-131;
	info->view.pos.y= 178;
	info->view.pos.z= -1992;

	info->view.pos.x=-74;
	info->view.pos.y= 274;
	info->view.pos.z= -2151;

	info->view.rot.x=-0.39;
	info->view.rot.y=-0.44;
	info->view.rot.z=0;

	info->view.rot.x=-0.37;
	info->view.rot.y=-0.44;
	info->view.rot.z=0;

	info->view.pos.x= 50;
	info->view.pos.y= 278;
	info->view.pos.z= -2427;
	info->view.rot.x=-0.34;
	info->view.rot.y=-0.48;
	info->view.rot.z=0;

	info->view.pos.x= 101;
	info->view.pos.y= 335;
	info->view.pos.z= -2393;
	info->view.rot.x=-0.38;
	info->view.rot.y=-0.43;
	info->view.rot.z=0;

	info->view.pos.x= 26;
	info->view.pos.y= 214;
	info->view.pos.z= -2337;
	info->view.rot.x=-0.35;
	info->view.rot.y=-0.47;
	info->view.rot.z=0;

	info->view.pos.x= -209;
	info->view.pos.y= -942;
	info->view.pos.z= -1495;
	info->view.rot.x= 0.07;
	info->view.rot.y=-0.59;
	info->view.rot.z=0;

	info->view.pos.x= -716*0;
	info->view.pos.y= -592*0;
	info->view.pos.z= -1056*0;
	info->view.rot.x= 0.0;
	info->view.rot.y=-0.49;
	info->view.rot.z=0;
/*
	info->view.pos.x= -227;
	info->view.pos.y= -1689;
	info->view.pos.z= -176;
	info->view.rot.x= 0.37;
	info->view.rot.y=-0.54;
	info->view.rot.z=0;
*/	
/*
	info->view.pos.x= 512;
	info->view.pos.y= -6075;
	info->view.pos.z= -3127;
	info->view.rot.x=-0.75;
	info->view.rot.y=-0.5;
	info->view.rot.z=0;
	*/
/*
	info->view.pos.x= -323;
	info->view.pos.y= 41;
	info->view.pos.z= -534;
	info->view.rot.x=-1.08;
	info->view.rot.y=-0.47;
	info->view.rot.z=0;
*/
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

    for ( t = 0 ; t<ClipBoxCount  ; t++ )
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

    wireframe = 0;
    osdText = 1;
    skybox  = 1;
    useTexture  = 0;
    useFog  = 1;

    getSeed();
}
///////////////////////////////////////////
void ENGINE::shootHandler(){
}
void ENGINE::shootPainter(matrix view){
}
///////////////////////////////////////////
unsigned int seedVal = 50290763;//33343915;
unsigned int seedVal2 = 50290763;
void ENGINE::getSeed()
{
    int a = seedVal;
	seedVal2 = a;
    int b = a;

    for ( int i = 0 ; i != randSize ; i++ )
    {   
        randArray[i] = i*11 - 3453621 + a ^ 353632;
        a = a*23+26348345+i*3;
        randArray[i] += b;
    }
	seedVal = timeGetTime();
}
///////////////////////////////////////////
int ENGINE::run (){

//	pbufferEnable( PBUF_RESOLUTION ,PBUF_RESOLUTION );

	info->view.rot.x+=(float)info->mouse.dx/3500;
	info->view.rot.y+=(float)info->mouse.dy/3500;

	vector add=vector(0,0,0);//walk

	if(info->key[14].changed)//DEL
	if(info->key[14].pressed)
	if(entityCount>0)
	{
		entityCount--;
		entity[entityCount].id=-1;
	}

    float accel=3;

	if(info->key[183].pressed)
	if(info->key[183].changed)gfx->screenshot();

    if(info->key[ 48].pressed)
	if(info->key[ 48].changed){ osdText^=1; }     //W

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

	if(info->key[ 43].pressed){ 
            accel=50;
            housep[0]->updated = false;
            housep[1]->updated = false;
  //          housep[2]->updated = false;
//            housep[3]->updated = false;
    }
	if(info->key[ 54].pressed){ 
            accel=50;
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
        gfx->drawFog(4);


  
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
    if(info->key[ 43].pressed)
    {
        minBox = 4;
    }

	int rendertime[10]={0,0,0,0,0,0,0,0,0,0};
	int polyc[10]={0,0,0,0,0,0,0,0,0,0};

    int start=ClipBoxCount-1;
    while((housep[start]->updated)&&(start>minBox))start--;

	rendertime[start]=timeGetTime();
	polyc[start]=gfx->polycount;
   	gfx->drawVoxels(scene[start],1,start);
	rendertime[start]=timeGetTime()-rendertime[start];
	polyc[start]=gfx->polycount-polyc[start];

	start++;

    for ( ; start<ClipBoxCount ; start++ )
	{
		polyc[start]=gfx->polycount;
		rendertime[start]=timeGetTime();
    	gfx->drawVoxels(scene[start],0,start);
		rendertime[start]=timeGetTime()-rendertime[start];
		polyc[start]=gfx->polycount-polyc[start];
	}
/*
//    start=ClipBoxCount-1;
    while(	(housep[start]->updated)&&
			(start>minBox)		)
	{
		if (start!=ClipBoxCount-1)
		{
			 if ( abs(housep[start  ]->posx * housep[start  ]->scale
					- housep[start+1]->posx * housep[start+1]->scale) >= 
					  housep[start+1]->sx   * housep[start  ]->scale/4) 
			 if ( abs(housep[start  ]->posy * housep[start  ]->scale
					- housep[start+1]->posy * housep[start+1]->scale) >= 
					  housep[start+1]->sy   * housep[start  ]->scale/4) 
			 if ( abs(housep[start  ]->posz * housep[start  ]->scale
					- housep[start+1]->posz * housep[start+1]->scale) >= 
					  housep[start+1]->sz   * housep[start  ]->scale/4) 
					  
					  ;;//break;
		}
		start--;
	}
*/
	

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

	gfx->clearMultiTex();

    int t=0;
    for (t=0;t<ClipBoxCount;t++)
    {
    	housep[t]->flip();
    	scene[t]=housep[t]->scene;
	}
	int avgtime = 0;
    for (t=0;t<ClipBoxCount;t++) avgtime += housep[t]->mc->timing[6] +  housep[t]->mc->timing[7];
	avgtime /=ClipBoxCount;

    for (t=0;t<ClipBoxCount;t++)
    {
/*	    if (osdText)
            gfx->text(-1,1-0.06f*(float)(t+1), 0.025f,0.06f,  1,1,0,1, "pos %d %d %d size %d",
			housep[t]->posx*housep[t]->scale,
			housep[t]->posy*housep[t]->scale,
			housep[t]->posz*housep[t]->scale,
			housep[t]->scale*64 );*/


	    if (osdText)
            gfx->text(-1,1-0.06f*(float)(t+1), 0.025f,0.06f,  1,1,0,1, "%d %d %d %d %d %d %d %d %d %d",
				housep[t]->mc->timing[0],
				housep[t]->mc->timing[1],
				housep[t]->mc->timing[2],
				housep[t]->mc->timing[3],
				housep[t]->mc->timing[4],
				housep[t]->mc->timing[5],
				housep[t]->mc->timing[6],
				housep[t]->mc->timing[7],
				rendertime[t],
				polyc[t]);
		
    }

    if (osdText)
    {
	    gfx->text(0.45f,-1+0.00f, 0.025f,0.06f,  1,1,0,1, "Key %d %d",k,seedVal2);
	    gfx->text(0.45f,-1+0.07f, 0.025f,0.06f,  1,1,0,1, "Polys %d",gfx->polycount);
	    gfx->text(0.45f,-1+0.14f, 0.025f,0.06f,  1,1,0,1, "MPolys/s %f",((float)gfx->polycount*gfx->fps)/(1000000));//,updatefps);
	    gfx->text(0.45f,-1+0.21f, 0.025f,0.06f,  1,1,0,1, "p %3.0f %3.0f %3.0f ",info->view.pos.x,info->view.pos.y,info->view.pos.z);
	    gfx->text(0.45f,-1+0.28f, 0.025f,0.06f,  1,1,0,1, "r %3.2f %3.2f %3.2f ",info->view.rot.x,info->view.rot.y,info->view.rot.z);
	    gfx->text(0.45f,-1+0.35f, 0.025f,0.06f,  1,1,0,1, "avg CB up %d",avgtime);
	    gfx->showFps();
    }
    

	if (info->key[97].changed){
		gfx->screenshot();
	}
	return 0;
}
///////////////////////////////////////////
void ENGINE::saveEntities(char* filename){
	FILE* fn;

	return;
	
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
	//Sleep(1);

    if(forceUpdate) houseptr->reset();


	if(!houseptr->setPos(newhx/2,newhy/2,newhz/2))
		return false;

//    houseptr->updated=false;

	if(houseptr->scale>=START_SUBDIV)
	{
		houseptr->mc->timing[7] = timeGetTime();

		int i,j;

		//Sleep(1);
		/*
        if (( randArray[ 0 ] & 16 )== 0 )
    		houseptr->shape(1,0,  
                -abs(randArray[3])%2000,
                 0,
                -abs(randArray[4])%2000,  
                 abs(randArray[5])%1000+1000,
                 abs(randArray[6])%2000,
                 abs(randArray[7])%2000);
*/
       if (( randArray[ 0 ] & 16 )== 0 )
			houseptr->shape((randArray[ 0 ] & 32)/32,0,  -8000,0,-8000,  8000,8000,8000);
//			houseptr->shape((randArray[ 0 ] & 32)/32,0,  -8000,256*4,-8000,  8000,8000,8000);

		//Sleep(1);

        int index=0;

		int scale =4;

		int dsx = g_voxel_size_x/8;
		int dsy = g_voxel_size_y;
		int dsz = g_voxel_size_z;
		char* data = g_voxel_data;//get_data(dsx,dsy,dsz);

		int m,n;
/*		loop(m, -1,32)
		loop(n,-32,1)
		{
			int stx = 8*dsx*4 * m;
			int stz =   dsy*4 * n;
			houseptr->shape(5,0, stx,0,stz,  stx+8*dsx*4,4*dsy,stz+4*dsz,data);
		}
		*/


		houseptr->shape(4,0,  -4000,0,-4000,  4000,4000,4000);
/*
//		for(j=0;j!=(abs(randArray[0])%124540)+25000;j++)
		for(j=0;j!=(abs(randArray[0])%6540)+6000;j++)
		{
			int sx=(abs(randArray[(j*29+1) % randSize])%4000)-2000;
			int sy=(abs(randArray[(j*13+4) % randSize])%70)-50;
			int sz=(abs(randArray[(j*15+5) % randSize])%4000)-2000;
			int dy=(abs(randArray[(j*37+7) % randSize])%80)+5;
			int dx=(abs(randArray[(j*19+4) % randSize])%80)+5 ;
			int dz=(abs(randArray[(j*37+1)% randSize])%80)+5 ;
			houseptr->shape(1,0,scale*sx,scale*sy,scale*sz,scale*(sx+dx),scale*(sy+dy),scale*(sz+dz));
            //if ((j&511)==0) Sleep(1);
		}

//		for(j=0;j!=(abs(randArray[0])%88000)+278000;j++)
		for(j=0;j!=(abs(randArray[0])%20000)+20000;j++)
		{
		//	int sx=(((j*52334)^23568)%4000)-2000;
		//	int sy=(((j*74357)^85467)%270)-70;
		//	int sz=(((j*15865)^23466)%4000)-2000;
		//	int dy=(((j*74746)^45643)%120)+30;
			int sx=(abs(randArray[(j*34+5) % randSize])%4000)-2000;
			int sy=(abs(randArray[(j*25+3) % randSize])%100)-70;
			int sz=(abs(randArray[(j*13+7) % randSize])%4000)-2000;
			int dy=(abs(randArray[(j*37+2) % randSize])%70)+5 ;
			int dx=(abs(randArray[(j*48+8) % randSize])%70)+5 ;
			int dz=(abs(randArray[(j*19+5) % randSize])%70)+5 ;
			//dy=dy*3/2;
			houseptr->shape(1,1,scale*sx,scale*sy,scale*sz,scale*(sx+dx),scale*(sy+dy),scale*(sz+dz));
            //if ((j&511)==0) Sleep(1);
		}
*/		
		//Sleep(1);
		houseptr->mc->timing[7] = timeGetTime()-houseptr->mc->timing[7];

	}							

	houseptr->getPolys();
    houseptr->updated=true;

	//Sleep(1);
	return true;
}
///////////////////////////////////////////
DWORD WINAPI ENGINE::voxelUpdate( LPVOID lpParameter )
{
	Sleep(1000);

	while(!info->exit)

	{
        if ( globalUpdate == true )
        {
			housep[ClipBoxCount-1]->setParent(NULL);
            for (int t=ClipBoxCount-1;t>=0;t--)
            {
    	        houseupdate(housep[t],true);
            }
            globalUpdate = false;
        }

        int minBox = 0;
        if(info->key[ 43].pressed) minBox = 2;

        for (int t=ClipBoxCount-1;t>=minBox;t--)
        {
    	    houseupdate(housep[t]);
    	    //if (t!=minBox   )houseupdate(housep[minBox  ]);
    	    //if (t!=minBox+1 )houseupdate(housep[minBox+1]);
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

