/*
 * This code was created by Jeff Molofee '99
 * (ported to Linux/GLX by Mihael Vrbanec '00)
 *
 * If you've found this code useful, please let me know.
 *
 * Visit Jeff at http://nehe.gamedev.net/
 *
 * or for port-specific comments, questions, bugreports etc.
 * email to Mihael.Vrbanec@stud.uni-karlsruhe.de
 */
//#################################################################//
#define RENDER_TYPE GL_TRIANGLE_STRIP
#define RENDER_LEN_DIV 1
#define PADDR(functype,funcname) (funcname = (functype) wglGetProcAddress( #funcname ))
//#################################################################//
#include "GFX.h"

int GFX::texCounter=0;
PFNGLTEXIMAGE3DPROC	GFX::glTexImage3D;
//#################################################################//
GFX::GFX(InfoType *globalInfo)
{
	info=globalInfo;

	texCounter=0;

	framestotal=0;
	framecachecount=0;
	
	benchmark=new BENCHMARK();
	benchmark->tstart();

	font = -1;

	reflectionData = NULL;

	initGL();

}
//#################################################################//
/* general OpenGL initialization function */
int GFX::initGL()
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE_MODE);
	glCullFace (GL_FRONT);
	
	glEnable(GL_TEXTURE_2D);       /* Enable Texture Mapping */
	glShadeModel(GL_SMOOTH);//FLAT);//SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glFlush();
	waterBumpTexture=loadTex("Data/waterBump.bmp",TEX_NORMALMAP);
	firstRaw=NULL;

	initExtensions();
	loadAllPrograms();
			
	return true;
}
//#################################################################//
GFX::~GFX()
{
}
//#################################################################//
int GFX::loadTex(char *name,int flags)
{
	BMP bmp;
	if((flags & TEX_HORIZONLOOKUP)==0)
		if(!bmp.load(name))
			return -1;//,true,255,0,255);

	glGenTextures(1,(unsigned int*)(&texCounter));//texcounter++

	glBindTexture  (GL_TEXTURE_2D, texCounter);
	glPixelStorei  (GL_UNPACK_ALIGNMENT, 1);

	if(flags==0)						bmp.addalpha(255,0,255);
	if((flags & TEX_NORMALIZE)!=0)		bmp.normalize();
	if((flags & TEX_NORMALMAP)!=0)		bmp.normalMap();
	if((flags & TEX_HORIZONMAP)!=0){	bmp.horizonMap();
 	    glEnable(GL_TEXTURE_3D);
	    glBindTexture  (GL_TEXTURE_3D, texCounter);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexImage3D   (GL_TEXTURE_3D, 0, GL_LUMINANCE8, bmp.width, bmp.height, 32 , 0, GL_LUMINANCE,  GL_UNSIGNED_BYTE, bmp.data);

		return texCounter;
	}
	if((flags & TEX_HORIZONLOOKUP)!=0){	
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	    glBindTexture  (GL_TEXTURE_CUBE_MAP_ARB, texCounter);
		BMP *bmpHL[6];
		int texSize=256;
	    for ( int i=0;i<6;i++)
		{
			bmpHL[i]=new BMP(texSize,texSize,16,NULL);
			bmpHL[i]->createElevationLookupMap(i);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, 
						 GL_LUMINANCE8_ALPHA8, texSize, texSize, 0, GL_LUMINANCE_ALPHA,
						 GL_UNSIGNED_BYTE, bmpHL[i]->data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return texCounter;
	}


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	if(bmp.bpp==32)	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA, bmp.width, bmp.height,/*GL_RGBA*/GL_BGRA_EXT, GL_UNSIGNED_BYTE, bmp.data );
	else			gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB, bmp.width, bmp.height,/*GL_RGB*/ GL_BGR_EXT , GL_UNSIGNED_BYTE, bmp.data );

	return texCounter;
}

//#################################################################//
void GFX::text(float x,float y,float size_x,float size_y,float r,float g,float b,float a,const char *format, ...)
{
//return;
	if(font==-1) font=loadTex("Data/font3.bmp");

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_DST_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	zBuffer(0);
	
	if (!((y<1)&&(y+size_y>-1)&&(x<1)))return;

	int  index=-1;
	char zei,i;

	float add_x[4]={ 0,1,1,0 };
	float add_y[4]={ 0,0,1,1 };

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D,font);

	char outext[1024];
	if (strlen(format)>512)return;
	va_list args;
	va_start(args, format);
	vsprintf( outext,format,args );
	va_end(args);

	while((++index<100)&&(outext[index]!=0)&&(x<1))
	{
		zei=(char)outext[index]-32;
		if( zei>=64)zei-=32;
		if((zei>= 0)&&(zei<64))
		{
			glBegin(GL_TRIANGLE_FAN);
			glColor4f   (r,g,b,a);

			for(i=3;i>=0;i--)
			{
				glTexCoord2f( (float)(((zei>>3)<<5)+  add_x[i]*31)/256,
				              (float)(((7-(zei &7))<<5)+1+add_y[i]*30)/256);
				glVertex3f  (x+add_x[i]*size_x,y+add_y[i]*size_y,1);
			}

			glEnd();
		}
		x+=size_x;
	}
	glDisable(GL_BLEND);
}
//#################################################################//
textureImage* GFX::createRawTex(int x,int y,int bpp,unsigned char* data)
{
	texCounter++;

	textureImage* tex = (textureImage*)malloc(sizeof(textureImage));
	tex->width=x;
	tex->height=y;
	tex->bpp=bpp;
	tex->id=texCounter;
	tex->update=true;
	tex->link=NULL;
	tex->data=data;

	if(data==NULL)
		tex->data=(unsigned char*)malloc(x*y*(bpp/8));

	if (firstRaw==NULL)	//linked List:  firstRaw->link => next ... => NULL (end)
		firstRaw=tex;
	else
	{
		textureImage* tmp=firstRaw;
		while(tmp->link!=NULL)
			tmp=(textureImage*)(tmp->link);

		if(tmp->link==NULL)
			((textureImage*)tmp)->link=tex;
	}
	return tex;
}
//#################################################################//
void GFX::showFps(void)
{
	polycount_tmp+=polycount;
	polycount=0;
	
	int timenow;
	time((time_t*)&timenow);
	if(timenow!=frametime)
	{
		frametime=timenow;
		fps=framecount;
		framecount=0;
		mpolycount=polycount_tmp;
		polycount_tmp=0;
	}
	framecount++;
	text(-1,-1, 0.05f,0.1f,  1,1,0,1, "fps gl %d",fps  );

	unsigned int  tmp;

}
//#################################################################//
void GFX::updateTextures(void)
{
	textureImage* tmp=firstRaw;

	while(tmp!=NULL) // check linklist
	{
		if(tmp->update)
		{
			tmp->update=false;
			glBindTexture(GL_TEXTURE_2D, tmp->id);

			if(tmp->bpp==24)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmp->width, tmp->height, 0,
				             GL_BGR_EXT, GL_UNSIGNED_BYTE, tmp->data);

			if(tmp->bpp==32)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmp->width, tmp->height, 0,
				             GL_BGRA_EXT, GL_UNSIGNED_BYTE, tmp->data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		tmp=(textureImage*)(tmp->link);
	}
}
//#################################################################//
void GFX::clear(void)
{
	
	updateTextures();

//	glXSwapBuffers(GLWin.dpy, GLWin.win);
	glClearColor(0.0f,0.0f, 0.0f, 1.0f );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	framestotal++;
	
	//benchmark->tend();
	//if(fps!=0)
	//milliseconds+=10000/fps;//benchmark->tval();
	
	framecachecount=0;
	cachedtstrips=0;
}
//#################################################################//
void GFX::setTex(int id)
{
	glBindTexture(GL_TEXTURE_2D, id);   /* select our  texture */
}
//############9#####################################################//
/*  rot.x=(float)rx/512-0.5f;
  rot.y=(float)ry/384-0.5f;
	  
	vector add=vector(0,0,0);//walk
	if(keytab[83]==83){ add.x-=2.63f; }
	if(keytab[81]==81){ add.x+=2.63f; }
	if(keytab[82]==82){ add.z-=2.63f; }
	if(keytab[84]==84){ add.z+=2.63f; }
	matrix tmp=ID;
	tmp>>rot;
	add*=tmp;
	mov+=add;
*/
//#################################################################//
void GFX::zBuffer(int valu){//Mode: 0=off,1=less,2=greater,3=lessequal,4=greaterequal
   if(valu==0){
    glDisable(GL_DEPTH_TEST); }
   else{
    glEnable (GL_DEPTH_TEST);
    if(valu==2)glDepthFunc(GL_LESS);
    if(valu==1)glDepthFunc(GL_GREATER);
    if(valu==4)glDepthFunc(GL_LEQUAL);
    if(valu==3)glDepthFunc(GL_GEQUAL);
   }
}
//#################################################################//
void GFX::screenshot()
{

	char  bmp[54]={	0x42,0x4D,0x36,0x30,0,0,0,0,0,0,0x36,0,0,0,0x28,0,0,0,
	                200,0,0,0, // X-Size
	                200,0,0,0, // Y-Size
	                1,0,0x18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	FILE* fn;
	char name[100];
	char* mem=(char*)malloc(info->win.width*info->win.height*3);

	int timenow;
	time((time_t*)&timenow);

	sprintf(name,"screenshots/Screenshot_%d.bmp",(timenow+framestotal));

	
	if ((fn = fopen (name,"wb")) != NULL)
	{
		bmp[18]=info->win.width;
		bmp[19]=info->win.width/256;
		bmp[22]=info->win.height;
		bmp[23]=info->win.height/256;

		// MessageBox(NULL,"texture","Midi",MB_OK);
		glFinish();
		glDisable (GL_LOGIC_OP);
		glPixelZoom(1,1);
		
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(ID.a);
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(ID.a);
		
		glRasterPos2d(-1,-1);//-(float)x/(float)width,-(float)y/(float)height);//width2-x/2,height2-y/2);
		glDrawBuffer( GL_BACK );
		glReadBuffer( GL_BACK );
		glReadPixels( 0,0,info->win.width,info->win.height, GL_BGR_EXT ,GL_UNSIGNED_BYTE,mem);
		glEnable (GL_LOGIC_OP);

		fwrite(bmp,1,54,fn);
		fwrite(mem,1,info->win.width*info->win.height*3,fn);
		fclose(fn);
		free(mem);
	}
}
//#define _DLIST_
#define _VARRAY_

//#################################################################//
int lid=0;

void GFX::drawVoxels(SCENE* scene,int scale,int level,int renderType)
{
//return;
	if(scene==NULL)return;
	if(scene->v_count<10)return;
	if(scene->v_indexcount==0)return;

	int i,j,type,xyz;
	float len,wid,siz=1,x,y,z;

	if(scene->changed)
	{
		if(scene->arrayID!=-1)
		{
			glDeleteBuffersARB( 1, (unsigned int*)&scene->arrayID );						// Free The Memory
	    }
		glGenBuffersARB( 1, (unsigned int*)&scene->arrayID ); 
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, scene->arrayID );				// Bind The Buffer 
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(T_ELEM)*scene->v_count, &scene->v[0], GL_STATIC_DRAW_ARB );

		scene->va_cacheCount =0;
		scene->changed=false;
		scene->va_cache=false;
	}

	/*
	if(scale==1)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer  ( 3, GL_FLOAT,         sizeof(T_ELEM), (char *) NULL );		// Set The Vertex Pointer To The Vertex Buffer
		glColorPointer   ( 4, GL_UNSIGNED_BYTE, sizeof(T_ELEM), (char *) NULL+24 );		// Set The TexCoord Pointer To The TexCoord Buffer
		glTexCoordPointer( 2, GL_FLOAT,         sizeof(T_ELEM), (char *) NULL+16 );		// Set The TexCoord Pointer To The TexCoord Buffer
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, scene->arrayID );			// Bind The Buffer
		for(i=0;i<scene->v_indexcount;i++)
		{
			int len=scene->v_indexs[i+1]-scene->v_indexs[i];
			glDrawArrays( RENDER_TYPE , scene->v_indexs[i],len );
			polycount+=len;
		}
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		return;
	}
	*/
	if(scene->childChanged)
	{
		scene->va_cacheCount =0;
		scene->va_cache=false;
		scene->childChanged=false;
	}


    if (renderType!=GL_LINE_STRIP)
	glEnableClientState(GL_COLOR_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
    if (renderType!=GL_LINE_STRIP)
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, scene->arrayID );			// Bind The Buffer
	//glInterleavedArrays( GL_T2F_C4UB_V3F, 0, NULL/*&(scene->v[0])*/);

	glVertexPointer  ( 3, GL_FLOAT,         sizeof(T_ELEM), (char *) NULL );		// Set The Vertex Pointer To The Vertex Buffer
	glColorPointer   ( 4, GL_UNSIGNED_BYTE, sizeof(T_ELEM), (char *) NULL+24 );		// Set The TexCoord Pointer To The TexCoord Buffer
	glTexCoordPointer( 2, GL_FLOAT,         sizeof(T_ELEM), (char *) NULL+16 );		// Set The TexCoord Pointer To The TexCoord Buffer



    glBegin ( GL_LINES ) ;
    glColor4f( 1-((float)((level+1)&3) / 3)  , ((float)(level&7) / 7) , level &1 ,0.45f );
    glEnd();

	if(scale!=1)	
	if(scene->va_cache)
	{
		int i;
		for(i=0;i<scene->va_cacheCount;i++)
		{
			glDrawArrays( renderType ,	
				scene->va_cacheList[i].start,
				scene->va_cacheList[i].count
			);
			polycount+=scene->va_cacheList[i].count-2;
		}
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		return;
	}

	int childstart=1+8*2;//*2*2;
	int childend=24*2;//*2*2;

	int childstartx=childstart+scene->posx;
	int childstarty=childstart+scene->posy;
	int childstartz=childstart+scene->posz;

	int childendx=childend+scene->posx;
	int childendy=childend+scene->posy;
	int childendz=childend+scene->posz;

	scene->v_uncachedcount=0;
	
	bool list_needs_to_be_ended=false;

	ivector id1,id2;

	
	int cachecount=0;

	for(i=0;i<scene->v_indexcount;i++){

		int 
		startid=scene->v_indexs[i],
		length=0;
		
		bool incube_old=false;
		bool incube=false;
		
		if(scale!=1)
		{
			for(j= scene->v_indexs[i] ;j< scene->v_indexs[i+1];j+=2)
			{
			id1=scene->v_pointids[j];
			id2=scene->v_pointids[j+1];

				incube_old=incube;
				if(
				((id1.x>=childstartx)&&(id1.x<=childendx)&&
				(id1.y>=childstarty)&&(id1.y<=childendy)&&
				(id1.z>=childstartz)&&(id1.z<=childendz))
				&&
				((id2.x>=childstartx)&&(id2.x<=childendx)&&
				(id2.y>=childstarty)&&(id2.y<=childendy)&&
				(id2.z>=childstartz)&&(id2.z<=childendz))
				)
				 	incube=true;
				 else
				 	incube=false;
				 
				if(incube&&incube_old)
				{
				 if(length!=0)
				 {
                    if (length>2)
                    {
					    glDrawArrays( renderType , startid, length );

					    scene->va_cacheList[cachecount].start=startid;
					    scene->va_cacheList[cachecount].count=length;
					    cachecount++;
                    }

					if(list_needs_to_be_ended)
						framecachecount+=length;
					polycount+=length-2;
					length=2;
				 }
				startid=j;
				}
				else
				{
				 length+=2;
				}
			}
			if(length!=0)
			{
                if (length>2)
                {
				    glDrawArrays( renderType /*TRIANGLE*/, startid,length );

				    scene->va_cacheList[cachecount].start=startid;
				    scene->va_cacheList[cachecount].count=length;
				    cachecount++;
                }

				if(list_needs_to_be_ended)
					framecachecount+=length;
				polycount+=length-2;
			}
		}
		else
		{
			int len=scene->v_indexs[i+1]-scene->v_indexs[i];
            if (len>2)
            {
			    glDrawArrays( renderType /*TRIANGLE*/, scene->v_indexs[i],len );
			    //scene->va_cacheList[cachecount].start=scene->v_indexs[i];
			    //scene->va_cacheList[cachecount].count=len;
			    cachecount++;
            }

			if(list_needs_to_be_ended)
				framecachecount+=len;
			polycount+=len-2;

		}
		
		//if(framecachecount>=GFX_CACHE_ALLOWED)
		if(list_needs_to_be_ended)
		{
			list_needs_to_be_ended=false;
			scene->v_indexcachedcount=i;
		}
		
	}

	if(scale!=1)
	{
	scene->va_cache=true;
	scene->va_cacheCount = cachecount;
	}
	
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
//#################################################################//
void GFX::drawFog(int a)
{
//return;
	//glMatrixMode(GL_PROJECTION);
	//glMatrixMode(GL_MODELVIEW);

	if(a==0){glDisable(GL_FOG);return;}

	bool   gp;// G Pressed? ( New )
	GLuint filter;// Which Filter To Use
	GLuint fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };// Storage For Three Types Of Fog
	GLuint fogfilter= 0;// Which Fog To Use

	GLfloat fogColor[4];

	GLfloat fogColor1[4]= {0.2f, 0.3f, 0.4f, 0.0f};// Fog Color 
	GLfloat fogColor2[4]= {1.0f, 1.0f, 0.5f, 0.0f};// Fog Color 
	GLfloat fogColor3[4]= {0.0f, 0.0f, 0.0f, 1.0f};// Fog Color 
	GLfloat fogColor4[4]= {0.25f, 0.07f, 0.025f, 1.0f};// Fog Color 

	//glClearColor(1.0f,1.0f,1.0f,1.0f);// We'll Clear To The Color Of The Fog ( Modified ) glFogi(GL_FOG_MODE, fogMode[fogfilter]);// Fog Mode

	if (a==1) glFogfv(GL_FOG_COLOR, fogColor1);// Set Fog Color
	if (a==2) glFogfv(GL_FOG_COLOR, fogColor2);// Set Fog Color
	if (a==3) glFogfv(GL_FOG_COLOR, fogColor3);// Set Fog Color
	if (a==4) glFogfv(GL_FOG_COLOR, fogColor4);// Set Fog Color

	glFogf(GL_FOG_DENSITY, 0.003f);// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);// Fog Hint Value
//  glFogf(GL_FOG_START, 0.2f);// Fog Start Depth
//	glFogf(GL_FOG_END, 1.0f);// Fog End Depth
	glFogf(GL_FOG_START, (float)(1)/25.0f);// Fog Start Depth
	glFogf(GL_FOG_END,   (float)(280000)/15.0f);// Fog End Depth
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glEnable(GL_FOG);// Enables GL_FOG 

//	glDisable(GL_FOG);// Enables GL_FOG 
 
}
//#################################################################//
void GFX::drawQuadZ(float x,float y,float z,float sizeX,float sizeY,float r,float g,float b,float a){
	glBegin(GL_QUADS);
	glColor4f(r,g,b,a);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(x-sizeX,y+sizeY,z);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(x+sizeX,y+sizeY,z);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(x+sizeX,y-sizeY,z);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(x-sizeX,y-sizeY,z);
	glEnd();
	polycount+=2;
}

//#################################################################//
void GFX::drawQuadY(float x,float y,float z,float sizeX,float sizeY,float r,float g,float b,float a){
	glBegin(GL_QUADS);
	glColor4f(r,g,b,a);
	glTexCoord2f(0.1f,0.1f);
	glVertex3f(x-sizeX,y,z+sizeY);
	glTexCoord2f(0.1f,0.99f);
	glVertex3f(x+sizeX,y,z+sizeY);
	glTexCoord2f(0.99f,0.1f);
	glVertex3f(x+sizeX,y,z-sizeY);
	glTexCoord2f(0.99f,0.99f);
	glVertex3f(x-sizeX,y,z-+sizeY);
	glEnd();
	polycount+=2;
}

//#################################################################//
void GFX::drawQuadX(float x,float y,float z,float sizeX,float sizeY,float r,float g,float b,float a){
	glBegin(GL_QUADS);
	glColor4f(r,g,b,a);
	glTexCoord2f(0.1f,0.99f);
	glVertex3f(x,y-sizeY,z+sizeX);
	glTexCoord2f(0.1f,0.1f);
	glVertex3f(x,y+sizeY,z+sizeX);
	glTexCoord2f(0.99f,0.1f);
	glVertex3f(x,y+sizeY,z-sizeX);
	glTexCoord2f(0.99f,0.99f);
	glVertex3f(x,y-sizeY,z-+sizeX);
	glEnd();
	polycount+=2;
}
//#################################################################//
void GFX::clearMultiTex()
{
 	/*
	glActiveTexture(GL_TEXTURE0_ARB);  // why?
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_VERTEX_PROGRAM_ARB);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	glDisable(GL_LIGHTING);

	return;
   	*/
	glDisable(GL_VERTEX_PROGRAM_ARB);
	glDisable(GL_FRAGMENT_PROGRAM_ARB);

	glActiveTexture(GL_TEXTURE0_ARB);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity ();
	glDisable(GL_TEXTURE_3D_EXT);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glActiveTexture(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity ();
	glDisable(GL_TEXTURE_3D_EXT);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glActiveTexture(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity ();
	glDisable(GL_TEXTURE_3D_EXT);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glActiveTexture(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity ();
	glDisable(GL_TEXTURE_3D_EXT);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glActiveTexture(GL_TEXTURE4_ARB);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity ();
	glDisable(GL_TEXTURE_3D_EXT);
	glDisable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glActiveTexture(GL_TEXTURE4_ARB);
	glBindTexture (GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE3_ARB);
	glBindTexture (GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2_ARB);
	glBindTexture (GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1_ARB);
	glBindTexture (GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0_ARB);
//	glBindTexture (GL_TEXTURE_2D, 0);

//	glActiveTexture(GL_TEXTURE0_ARB);
	glDisable(GL_BLEND);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}
//#################################################################//
void GFX::setBlurTex(int tex0) //  mirror , standard , bump
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// bump mapping
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vpid[3]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fpid[3]);

	return;
}
//#################################################################//
void GFX::setThresholdTex(int tex0) //  mirror , standard , bump
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vpid[5]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fpid[5]);

	return;
}
//#################################################################//
void GFX::setGlowTex(int texNormal,int texGlow) //  mirror , standard , bump
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, texNormal);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, texGlow);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// bump mapping
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vpid[4]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fpid[4]);

	return;
}
//#################################################################//
void GFX::setEnvBumpTex(int tex0,int tex1,int tex2) //  mirror , standard , bump
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glActiveTexture(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, tex2);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// bump mapping
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vpid[0]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fpid[0]);

	float screen[4]={
		(float)1/1024.0f,
		(float)1/1024.0f,
		0,0
	};
	glProgramLocalParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 0, (const float*)&screen);

	return;
}
//#################################################################//
void GFX::setEnvBumpTex2(int tex0,int tex1,int tex2) //  mirror , standard , bump
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glActiveTexture(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glActiveTexture(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, tex2);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// bump mapping
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vpid[7]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fpid[7]);

	float screen[4]={
		(float)1/1024.0f,
		(float)1/1024.0f,
		0,0
	};
	glProgramLocalParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB, 0, (const float*)&screen);

	return;
}
//#################################################################//
void GFX::setSpecialBump(int texmap,int heightmap,int normalmap, int horizonmap,int horizonlookupmap,
						vector lightpos,vector lightcol,float lightradius, vector  campos)
{
	// TMU0 : texmap
	// TMU1 : heightmap
	// TMU2 : normalmap
	// TMU3 : horizonmap
	// TMU4 : horizonlookupmap

	glActiveTexture(GL_TEXTURE4_ARB);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, horizonlookupmap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glActiveTexture(GL_TEXTURE3_ARB);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, horizonmap);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

	glActiveTexture(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, normalmap);
	
	glActiveTexture(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, heightmap);

	glActiveTexture(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texmap);

	// bump mapping
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vpid[2]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fpid[2]);

	glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0, lightpos.x,lightpos.y,lightpos.z,1 );
	glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 1, 1/lightradius, 0,0,1 );
	glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 2, campos.x,campos.y,campos.z,1 );

	glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0, lightcol.x,lightcol.y,lightcol.z,1 );
}
//#################################################################//
//void GFX::setTerrainTex(int tex0,int tex1,int tex2,int texDetail,int selectTex) //  mirror , standard , bump
void GFX::setTerrainTex(int tex0,int tex1,int tex2,int texDetail1,int texDetail2,int texDetail3,int selectTex)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, tex0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	glActiveTexture(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, tex1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	glActiveTexture(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	glActiveTexture(GL_TEXTURE3_ARB);
	glBindTexture(GL_TEXTURE_2D, selectTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	glActiveTexture(GL_TEXTURE4_ARB);
	glBindTexture(GL_TEXTURE_2D, texDetail1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	glActiveTexture(GL_TEXTURE5_ARB);
	glBindTexture(GL_TEXTURE_2D, texDetail2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	glActiveTexture(GL_TEXTURE6_ARB);
	glBindTexture(GL_TEXTURE_2D, texDetail3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

//	glActiveTexture(GL_TEXTURE4_ARB);
//	glBindTexture(GL_TEXTURE_2D, texDetail);


	// bump mapping
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vpid[1]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fpid[1]);

	float screen[4]={
		(float)1/(float)info->win.width,
		(float)1/(float)info->win.height,
		0,0
	};

	return;
}
//#################################################################//
void GFX::setObjTex(int tex,int detail,int glow) 
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	glActiveTexture(GL_TEXTURE1_ARB);
	glBindTexture(GL_TEXTURE_2D, detail);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	glActiveTexture(GL_TEXTURE2_ARB);
	glBindTexture(GL_TEXTURE_2D, glow);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf      (GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,   GL_MODULATE);

	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vpid[6]);
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fpid[6]);

	return;
}
//#################################################################//
// Check to make sure the extensions we need are available.
//#################################################################//
bool GFX::initExtensions(void)
{
/*    if ( !isExtensionSupported( "GL_ARB_shader_objects" )   ||
         !isExtensionSupported( "GL_ARB_fragment_shader" )  ||
         !isExtensionSupported( "GL_ARB_vertex_shader" )    ||
         !isExtensionSupported( "GL_ARB_shading_language_100" ) )*/

    PADDR(PFNGLCREATEPROGRAMOBJECTARBPROC, glCreateProgramObjectARB);
    PADDR(PFNGLCREATESHADEROBJECTARBPROC, glCreateShaderObjectARB);
    PADDR(PFNGLDELETEOBJECTARBPROC, glDeleteObjectARB);
    PADDR(PFNGLDETACHOBJECTARBPROC, glDetachObjectARB);
    PADDR(PFNGLATTACHOBJECTARBPROC, glAttachObjectARB);
    PADDR(PFNGLSHADERSOURCEARBPROC, glShaderSourceARB);
    PADDR(PFNGLCOMPILESHADERARBPROC, glCompileShaderARB);
    PADDR(PFNGLLINKPROGRAMARBPROC, glLinkProgramARB);
    PADDR(PFNGLGETINFOLOGARBPROC, glGetInfoLogARB);
    PADDR(PFNGLUSEPROGRAMOBJECTARBPROC, glUseProgramObjectARB);
    PADDR(PFNGLGETOBJECTPARAMETERIVARBPROC, glGetObjectParameterivARB);
    PADDR(PFNGLGETOBJECTPARAMETERFVARBPROC, glGetObjectParameterfvARB);
    PADDR(PFNGLGETUNIFORMLOCATIONARBPROC, glGetUniformLocationARB);
    PADDR(PFNGLGETATTRIBLOCATIONARBPROC, glGetAttribLocationARB);
    PADDR(PFNGLBINDATTRIBLOCATIONARBPROC, glBindAttribLocationARB);
    PADDR(PFNGLUNIFORM1FARBPROC, glUniform1fARB);
    PADDR(PFNGLUNIFORM2FARBPROC, glUniform2fARB);
    PADDR(PFNGLUNIFORM3FARBPROC, glUniform3fARB);
    PADDR(PFNGLUNIFORM4FARBPROC, glUniform4fARB);
    PADDR(PFNGLUNIFORM1IARBPROC, glUniform1iARB);
    PADDR(PFNGLUNIFORM2IARBPROC, glUniform2iARB);
    PADDR(PFNGLUNIFORM3IARBPROC, glUniform3iARB);
    PADDR(PFNGLUNIFORM4IARBPROC, glUniform4iARB);
    PADDR(PFNGLUNIFORM1FVARBPROC, glUniform1fvARB);
    PADDR(PFNGLUNIFORM2FVARBPROC, glUniform2fvARB);
    PADDR(PFNGLUNIFORM3FVARBPROC, glUniform3fvARB);
    PADDR(PFNGLUNIFORM4FVARBPROC, glUniform4fvARB);
    PADDR(PFNGLUNIFORM1IVARBPROC, glUniform1ivARB);
    PADDR(PFNGLUNIFORM2IVARBPROC, glUniform2ivARB);
    PADDR(PFNGLUNIFORM3IVARBPROC, glUniform3ivARB);
    PADDR(PFNGLUNIFORM4IVARBPROC, glUniform4ivARB);
    PADDR(PFNGLVERTEXATTRIB4FVARBPROC, glVertexAttrib4fvARB);
    PADDR(PFNGLVERTEXATTRIB3FVARBPROC, glVertexAttrib3fvARB);
    PADDR(PFNGLVERTEXATTRIB2FVARBPROC, glVertexAttrib2fvARB);
    PADDR(PFNGLVERTEXATTRIB1FVARBPROC, glVertexAttrib1fvARB);
    PADDR(PFNGLVERTEXATTRIB4FARBPROC, glVertexAttrib4fARB);
    PADDR(PFNGLVERTEXATTRIB3FARBPROC, glVertexAttrib3fARB);
    PADDR(PFNGLVERTEXATTRIB2FARBPROC, glVertexAttrib2fARB);
    PADDR(PFNGLVERTEXATTRIB1FARBPROC, glVertexAttrib1fARB);

	glBindProgramARB = NULL;
	PADDR(PFNGLGENPROGRAMSARBPROC	,glGenProgramsARB);
	PADDR(PFNGLBINDPROGRAMARBPROC	,glBindProgramARB);
	PADDR(PFNGLPROGRAMSTRINGARBPROC	,glProgramStringARB);
	PADDR(PFNGLPROGRAMLOCALPARAMETER4FVARBPROC, glProgramLocalParameter4fvARB);
	PADDR(PFNGLPROGRAMLOCALPARAMETER4FARBPROC, glProgramLocalParameter4fARB);

	if (glBindProgramARB == NULL)
    {
		MessageBox(NULL, "OpenGL Shading Language extensions not available", "EXT", MB_OK);exit(312);
    }

	PADDR(PFNGLTEXIMAGE3DPROC, glTexImage3D);

	glActiveTexture		= (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2f	= (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
	glGenBuffersARB		= (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
	glBindBufferARB		= (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
	glBufferDataARB		= (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
	glDeleteBuffersARB	= (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");

    if (	(glActiveTexture==NULL)   ||
			(glMultiTexCoord2f==NULL)   ||
			(glGenBuffersARB==NULL)   ||
			(glBindBufferARB==NULL)   ||
			(glBufferDataARB==NULL)   ||
			(glDeleteBuffersARB==NULL)  )
    {
		MessageBox(NULL, "VBO Unsupported", "EXT", MB_OK);exit(312);
    }
	    

	//glTexBumpParameterfvATI = (PFNGLTEXBUMPPARAMETERFVATIPROC)wglGetProcAddress("glTexBumpParameterfvATI");

	return 0;
}
//#################################################################//
// The recommended technique for querying OpenGL extensions;
// from http://opengl.org/resources/features/OGLextensions/
int GFX::isExtensionSupported( const char *extension )
{
    const GLubyte *extensions = NULL;
    const GLubyte *start;
    GLubyte *where, *terminator;

    /* Extension names should not have spaces. */
    where = (GLubyte *) strchr(extension, ' ');
    if (where || *extension == '\0')
        return 0;

    extensions = glGetString(GL_EXTENSIONS);

    /* It takes a bit of care to be fool-proof about parsing the
    OpenGL extensions string. Don't be fooled by sub-strings, etc. */
    start = extensions;
    for (;;) {
        where = (GLubyte *) strstr((const char *) start, extension);
        if (!where)
            break;
        terminator = where + strlen(extension);
        if (where == start || *(where - 1) == ' ')
            if (*terminator == ' ' || *terminator == '\0')
                return 1;
        start = terminator;
    }
    return 0;
}
//#################################################################//
char* GFX::readProgram(const char* name){
	FILE *in = fopen(name, "r");
   
	if (!in)
    {
		MessageBox(NULL, "Shader Program not found", "EXT", MB_OK);exit(312);
    }
 
	char *buffer = 0;
   
	// get file size
	long size = 0, curpos;
	curpos = ftell(in);
	fseek(in, 0L, SEEK_END);
	size = ftell(in);
	fseek(in, curpos, SEEK_SET);
   
	if (!size)
		return 0;   
	if (!(buffer = new char[size + 1]))
		return 0;

	memset(buffer, '\0', size + 1);
	if (!buffer)
		return 0;
	fread(buffer, 1, size, in); // check return with size?
	fclose(in);

	return buffer;
}
//#################################################################//
void GFX::loadFPandVP(int num,char* name)
{
    char fname[200];
	char* program;

	sprintf(fname,"shaders/vp_%s.txt",name);

	program = readProgram(fname);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, vpid[num]);
	glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
		strlen(program), program);

	sprintf(fname,"shaders/fp_%s.txt",name);

	program = readProgram(fname);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, fpid[num]);
	glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
		strlen(program), program);
}
//#################################################################//
void GFX::loadAllPrograms()
{
	// load vertex and fragment programs
	char* program;
	glGenProgramsARB(10, vpid);
	glGenProgramsARB(10, fpid);

	// #0 Environmental Bumpmapping
	loadFPandVP(0,"envbump");
	// #1 Multitexture Terrain Rendering
	loadFPandVP(1,"terraintex");
	// #2 Special BumpMapping
	loadFPandVP(2,"uberbump");
	// #3 Blur
	loadFPandVP(3,"blur");
	// #4 Glow
	loadFPandVP(4,"glow");
	// #5 Threshold
	loadFPandVP(5,"threshold");
	// #6 3D Objects
	loadFPandVP(6,"objtex");
	// #7 Water pass 2
	loadFPandVP(7,"envbump2");
}
//#################################################################//
#include "SKY.h"
//#################################################################//
