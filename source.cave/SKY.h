//part of GFX.cpp
//#################################################################//
BMP skycolors1("Data/skycol.bmp");//[1024*3];
BMP skycolors2("Data/skycol2.bmp");//[1024*3];
int GFX::gfx_suntex=-1;
int GFX::gfx_startex=-1;
int GFX::gfx_hangtex=-1;
int GFX::gfx_planttex=-1;
//#################################################################//
#define _PLANETDETAIL_ 64
//#################################################################//
int gfx_planetlist=0;
	// TMU0 : texmap
	// TMU1 : heightmap
	// TMU2 : normalmap
	// TMU3 : horizonmap
	// TMU4 : horizonlookupmap
int gfx_planettex=-1;
int gfx_planettexHEI=-1;
int gfx_planettexNOR=-1;
int gfx_planettexHOR=-1;
int gfx_planettexHLU=-1;
//#################################################################//
void GFX::TangentAndBinormal(vector inP[3],vector inT[3],vector* outTan,vector* outBin)
{
  vector p,q,st1,st2;

  // Calculate triangle's edges:
  p  = inP[1] - inP[0];
  q  = inP[2] - inP[0];
  
  // Calculate S and T vectors:
  st1= inT[1] - inT[0];
  st2= inT[2] - inT[0];

  // Calculate tangent and binormal for a triangle with the given edges P and Q.
  float s = 1 / ((st1.x * st2.y) - (st2.x * st1.y));

  *outTan = s * (  st2.y*p - st1.y*q ); *outTan=!(*outTan);
  *outBin = s * ( -st2.x*p + st1.x*q ); *outBin=!(*outBin);
}
//#################################################################//
void GFX::Planet()
{
	if(gfx_planettex==-1)
	{
		gfx_planettex=loadTex("Data/moon.bmp");

		gfx_planetlist=glGenLists(1);
		glNewList(gfx_planetlist, GL_COMPILE);
	
		setTex(gfx_planettex);

		 for( int i=0;i<=_PLANETDETAIL_-1;i++)
		 {
			glBegin(GL_TRIANGLE_STRIP);
			glColor4f(1,1,1,1);

			 for( int j=0;j<=_PLANETDETAIL_;j++)
			 {

				float tx,ty;
				vector p;

				tx=(float)i*8/_PLANETDETAIL_;
				ty=(float)j*8/_PLANETDETAIL_;
				p.x=    sin((float)i*PI/_PLANETDETAIL_)*cos((float)j*2*PI/_PLANETDETAIL_);
				p.y=    sin((float)i*PI/_PLANETDETAIL_)*sin((float)j*2*PI/_PLANETDETAIL_);
				p.z=    cos((float)i*PI/_PLANETDETAIL_);
				glTexCoord2f(tx,ty);
				glNormal3f(p.x,p.y,p.z);
				glVertex3f(p.x,p.y,p.z);

				i++;

				tx=(float)i*8/_PLANETDETAIL_;
				ty=(float)j*8/_PLANETDETAIL_;
				p.x=    sin((float)i*PI/_PLANETDETAIL_)*cos((float)j*2*PI/_PLANETDETAIL_);
				p.y=    sin((float)i*PI/_PLANETDETAIL_)*sin((float)j*2*PI/_PLANETDETAIL_);
				p.z=    cos((float)i*PI/_PLANETDETAIL_);
				glTexCoord2f(tx,ty);
				glNormal3f(p.x,p.y,p.z);
				glVertex3f(p.x,p.y,p.z);

				i--;

			 }
			 glEnd();
		}

	
	}else
	glCallList(gfx_planetlist);
}
//#################################################################//
int gfx_ringlist=0;
int gfx_ringtex=-1;
//#################################################################//
void GFX::RingPlanet()
{
    Planet();
	
	if(gfx_ringtex==-1)
	{
		gfx_ringtex=loadTex("Data/ring.bmp");
		
		gfx_ringlist=glGenLists(1);
		glNewList(gfx_ringlist, GL_COMPILE);

		setTex(gfx_ringtex);
	
		glBegin(GL_TRIANGLE_STRIP);
		glColor4f(1,1,1,1);

		 for( int j=0;j<=_PLANETDETAIL_;j++)
		 {

			float tx,ty;
			vector p;

			tx=(float)j*2/_PLANETDETAIL_;
			ty=(float)1;
			p.x=  -sin((float)j*2*PI/_PLANETDETAIL_)*(1.3f+1*0.2f);
			p.z=   0;
			p.y=   cos((float)j*2*PI/_PLANETDETAIL_)*(1.3f+1*0.2f);
			glTexCoord2f(tx,ty);
			glNormal3f(p.x,p.y,p.z);
			glVertex3f(p.x,p.y,p.z);

			tx=(float)j*2/_PLANETDETAIL_;
			ty=(float)0;
			p.x=  -sin((float)j*2*PI/_PLANETDETAIL_)*(1.3f+0*0.2f);
			p.z=   0;
			p.y=   cos((float)j*2*PI/_PLANETDETAIL_)*(1.3f+0*0.2f);
			glTexCoord2f(tx,ty);
			glNormal3f(p.x,p.y,p.z);
			glVertex3f(p.x,p.y,p.z);

		 }
		glEnd();
		glEndList();
   	}else
	glCallList(gfx_ringlist);
}
//#################################################################//
#define _WATERDETAIL_ 64
//#################################################################//
int gfx_waterlist=0;
int gfx_watertex=-1;
//#################################################################//
void GFX::Water(vector ofs,vector color,matrix camera)
{
	glDisable(GL_CULL_FACE);

	if(gfx_watertex==-1)
	{
		gfx_watertex=loadTex("Data/WaterTransparency.bmp");
	}
   setEnvBumpTex(reflectionTexture,waterBumpTexture,gfx_watertex);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//   glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	for(int pass=0;pass<2;pass++)
	{
		if(pass==0)setEnvBumpTex(reflectionTexture,waterBumpTexture,gfx_watertex);
		if(pass==1)setEnvBumpTex2(reflectionTexture,waterBumpTexture,gfx_watertex);

		if(pass==0)glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		if(pass==1)glBlendFunc(GL_SRC_ALPHA,GL_ONE);

		if(pass==0)glDepthMask(0);
		if(pass==1)glDepthMask(1);


		for( int i=0;i<=_WATERDETAIL_/2-1;i++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			
			float alpha1=0.4f+(float)((i+1)*(i+1))/(50);//-ofs.y/800;
			float alpha2=0.4f+(float)((i+2)*(i+2))/(50);//-ofs.y/800;

			if(pass==1)color.x=color.y=color.z=1.0f;

			if(pass==1)
			{
				alpha1=1-alpha1;
				alpha2=1-alpha2;
			}

			if(alpha1>0.99)alpha1=0.99f;
			if(alpha1<0.0f)alpha1=0.0f;
			if(alpha2>0.99)alpha2=0.99f;
			if(alpha2<0.0f)alpha2=0.0f;

			vector scrCoord;
			//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_DONT_CARE);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE_EXT);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE_EXT);


      
			float tm=((float)((info->time/10)%2047))/2047;


			for( int j=0;j<=_WATERDETAIL_;j++)
			{
				 
				if(pass==0)color.x=color.y=color.z=alpha1;

				glColor4f(color.x,color.y,color.z,alpha1);

				float tx,ty,dist=((float)i*i);
				vector p;

				//tx=(float)i/_WATERDETAIL_;
				//ty=(float)j/_WATERDETAIL_;
				p.x=    40*dist*cos((float)j*2*PI/_WATERDETAIL_);
				p.z=    40*dist*sin((float)j*2*PI/_WATERDETAIL_);
				p.y=    1;//cos((float)i*PI/_WATERDETAIL_);

//				ty=(p.x)/1000;
//				tx=(p.z)/1000;

				scrCoord=p;
				scrCoord*=camera;
				scrCoord.z*=2;
				if(scrCoord.z>0){
				tx=scrCoord.x/scrCoord.z+0.5f;
				ty=scrCoord.y/scrCoord.z+0.5f;}
				else{
				tx=scrCoord.x/0.001f+0.5f;
				ty=scrCoord.y/0.001f+0.5f;}

			//	glTexCoord2f(tx,ty);
		        glMultiTexCoord2f(GL_TEXTURE0_ARB, (-info->view.pos.x+p.x)/290+tm, (-info->view.pos.z+p.z)/290);


				glVertex3f(p.x,p.y,p.z);

				i++;
				if(pass==0)color.x=color.y=color.z=alpha2;

				glColor4f(color.x,color.y,color.z,alpha2);
				dist= ((float)i*i);
				p.x=    40*dist*cos((float)j*2*PI/_WATERDETAIL_);
				p.z=    40*dist*sin((float)j*2*PI/_WATERDETAIL_);
				p.y=    1;//cos((float)i*PI/_WATERDETAIL_);

				scrCoord=p;
				scrCoord*=camera;
				scrCoord.z*=2;
				if(scrCoord.z>0){
				tx=scrCoord.x/scrCoord.z+0.5f;
				ty=scrCoord.y/scrCoord.z+0.5f;}
				else{
				tx=scrCoord.x/0.001f+0.5f;
				ty=scrCoord.y/0.001f+0.5f;}
				
				glMultiTexCoord2f(GL_TEXTURE0_ARB, (-info->view.pos.x+p.x)/290+tm, (-info->view.pos.z+p.z)/290);

				glVertex3f(p.x,p.y,p.z);

				i--;

			 }
			 glEnd();
		}
	}//pass


//	}else
//	glCallList(gfx_waterlist);
	clearMultiTex();
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

//#################################################################//

//#################################################################//
void GFX::drawSky(bool flip)
{
	int i;
	matrix kamera;
	kamera =permutation;

	zBuffer(0);
	drawFog(0);
	
	glDisable (GL_BLEND);
	glClearColor(0.0f,0.0f, 0.0f, 1.0f );
	glClearDepth(0.0f);
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	zBuffer(3);

	if(flip)
	{
		kamera *=vector(1,-1,1);
		glCullFace (GL_BACK);
		info->view.rot.y*=-1;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(kamera.a);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float LightAmb[4]= {-0.1f,-0.1f,-0.1f, 1.0f }; 
	float LightDif[4];
	float LightPos[4];
	           
	vector movep((-0.0-1.8f+(float)(34-15)/50) ,-0.00f -0.3f,0.1+ 0.8f-(float)(45-15)/50);
	vector rotp((float)((int)(-9))/20,(float)((int)(-3-1))/20, (float)info->time/100000.0f);
	rotp=vector(0,2.71f,(float)info->time*4/900000.0f);

	matrix view=ID;
	view*=vector((float)info->win.height/info->win.width,1,1);
	view<<info->view.rot;
	
	matrix 
	planetmat=view;
	planetmat*=vector(100,100,100);
	planetmat+=movep;
	planetmat<<rotp;
	glLoadMatrixf(planetmat.a);
	
	matrix	lightmat1=ID;
	lightmat1/=vector(100,100,100);
	lightmat1+=movep;
	lightmat1<<rotp;
	matrix	lightmat;
	matrixInvert(lightmat1.a,lightmat.a);
	vector light=vector(171,2,-40);//
	light=vector(-45, 38, -145);//info->view.pos;//( info->view.pos.x , info->view.pos.y ,info->view.pos.z  );
	light*=lightmat;
	
	LightPos[0]=light.x;LightPos[1]=light.y;LightPos[2]=light.z;
	LightDif[0]=2.0f;   LightDif[1]=1.0f;   LightDif[2]=0.0f;LightDif[3]=1.0f;
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDif);
	glLightfv(GL_LIGHT0, GL_POSITION,LightPos); 
//	clearMultiTex();
	glDisable (GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	RingPlanet();
	
	glDisable (GL_BLEND);
	glDisable (GL_LIGHT0);
	glDisable(GL_LIGHTING);
				
	view=ID;
	view*=vector((float)info->win.height/info->win.width,1,1);
	glLoadMatrixf(view.a);

	view=ID;
	view<<info->view.rot;

	setTex(0);
	if(gfx_suntex==-1)
		gfx_suntex=loadTex("Data/flare1.sunset.bmp");
	
		
	//STARS
	if(gfx_startex==-1)
		gfx_startex=loadTex("Data/flare6.bmp");
	
	setTex(gfx_startex);

//	if(0)
  vector star;

	for(i=0;i<100;i++){

	 star.x= ((i*235^13632) & 8191)-4096;
	 star.y=-100-((i*642^3425) & 8191);
	 star.z= ((i*523^73563) & 8191)-4096;
	 star*=view;

	 drawQuadZ(star.x,star.y,star.z,100,100,0.7f,0.7f,0.0f);
	}
		
	glClear( GL_DEPTH_BUFFER_BIT);

	view=ID;
	view*=vector((float)info->win.height/info->win.width,1,1);
	view<<info->view.rot;
	glLoadMatrixf(view.a);

	setTex(0);	
	zBuffer(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	for (i=0;i<=32;i++)
	{//int i
 	 glBegin(GL_TRIANGLE_STRIP);
	 for (int j=0;j<=32;j++)
	 for (int k=0;k<=1;k++)
	 {
	 	i+=1-k;
		float tx=0,ty=0;
		vector p;
		vector color;
		
		float c=sin(i*PI/32)*sin(j*PI/32);
		
		p.x=-i+16;
		p.z=-j+16;
		p.y=-c*5+1;
		
		glColor4f(0,0,0,1-c);
		glTexCoord2f(tx,ty);
		glVertex3f(p.x,p.y,p.z);
		
	 	i-=1-k;
	 }
	 
	 glEnd();
	}
	
	
	glBlendFunc(GL_DST_ALPHA,GL_ONE);
	glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
	
	if(skycolors1.data!=NULL)
	for (int i=0;i<=32;i++)
	{//int i
	glBegin(GL_TRIANGLE_STRIP);

	 for (int j=0;j<=32;j++)
	 for (int k=0;k<=1;k++)
	 {
	 	i+=1-k;
	 
		vector p;
		vector color;

		int tx=i*31/32;
		int ty=j*31/32;
		
		color.x  =(float)skycolors1.data[((tx+ty*32)&1023)*3+2]/255;
		color.y  =(float)skycolors1.data[((tx+ty*32)&1023)*3+1]/255;
		color.z  =(float)skycolors1.data[((tx+ty*32)&1023)*3+0]/255;
		
		p.x=-i+16;
		p.z=-j+16;
		p.y=-(sin(i*PI/32)*sin(j*PI/32)*5)+1;

		glColor4f(color.x,color.y,color.z,sqrt((float)(sqr(i-16)/16+sqr(j-16)/16)/16) );
		glVertex3f(p.x,p.y,p.z);
		
	 	i-=1-k;
	 }
	 
	 glEnd();
	}

	glDisable(GL_BLEND);
	zBuffer(0);
	
	if(skycolors2.data!=NULL)
	for (int i=0;i<=32;i++)
	{//int i
	glBegin(GL_TRIANGLE_STRIP);

	 for (int j=0;j<=32;j++)
	 for (int k=0;k<=1;k++)
	 {
	 	i+=k;
	 
		vector p;
		vector color;

		int tx=i*31/32;
		int ty=j*31/32;
		
		color.x  =(float)skycolors2.data[((tx+ty*32)&1023)*3+2]/255;
		color.y  =(float)skycolors2.data[((tx+ty*32)&1023)*3+1]/255;
		color.z  =(float)skycolors2.data[((tx+ty*32)&1023)*3+0]/255;
		
		p.x=-i+16;
		p.z=-j+16;
		p.y=-(-(sin(i*PI/32)*sin(j*PI/32)*5)+0)+1;

		glColor4f(color.x,color.y,color.z,1);
		glVertex3f(p.x,p.y,p.z);
		
	 	i-=k;
	 }
	 
	 glEnd();
	}
	
	// SUN
	view=ID;
	view*=vector((float)info->win.height/info->win.width,1,1);
	glLoadMatrixf(view.a);

	view=ID;
	view<<info->view.rot;

	zBuffer(0);
	setTex(gfx_suntex);
	//setTex(gfx_startex);
    star=vector(0,-32,-320);//320,0,-320/10);
//    star=vector(320,-320,-320);//320,0,-320/10);
	view=ID;
	view<<info->view.rot;
	star*=view;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
//	drawQuadZ(star.x,star.y,star.z,280,280,  0.6f,0.5f,0.0f);
	drawQuadZ(star.x,star.y,star.z,60,60,  0.6f,0.5f,0.0f);
	glDisable(GL_BLEND);

	zBuffer(3);
/*
	kamera =permutation;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(kamera.a);
*/
//	matrix kamera;
	kamera =permutation;
	if(flip)
	{
		info->view.rot.y*=-1;
		//kamera *=vector(1,-1,1);
		glCullFace (GL_FRONT);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(kamera.a);
		glMatrixMode(GL_MODELVIEW);
	}
}
//#################################################################//
void GFX::Hangs(MC_POINT *points,int count)
{
	if(gfx_hangtex==-1)
	{
		gfx_hangtex=loadTex("Data/hang.bmp");
  }
	
	// SUN
	matrix view=ID;
	view*=vector((float)info->win.height/info->win.width,1,1);
	view<<info->view.rot;
	view+=info->view.pos;
	glLoadMatrixf(view.a);

	view=ID;
//	view<<rot;

//  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//	glBlendFunc(GL_DST_ALPHA,GL_ONE);
//	glBlendFunc(GL_DST_ALPHA,GL_ONE);
//	zBuffer(0);
	setTex(gfx_hangtex);

	vector star;

	glEnable(GL_BLEND);
  glDepthMask(0);
	glDisable(GL_CULL_FACE);
	for(int i=0;i!=count;i++)
	{
    if(points[i].idmax==0)continue;
    bool leave=false;
    for(int j=0;j!=points[i].idmax;j++)
     if(points[points[i].id[j]].q.y>-0.4f)
      leave=true;
    if(leave) continue;
		
		star=points[i].p;	
		
		//star*=view;
		if(points[i].q.y<-0)
		{
		int x=star.x;
		int y=star.y;
		int z=star.z;
		x^=x>>4;
		x^=x>>4;
		y^=y>>4;
		y^=y>>4;
		z^=z>>4;
		z^=z>>4;
		int sizex=((((x^y^z)*12345)^75633)%60);
		if(sizex>0)
		{
		 if(sizex&1)
		 drawQuadZ(star.x,star.y+sizex*3-20,star.z,sizex,sizex*3,0.2f,0.2f,0.2f);
		 else
		 drawQuadX(star.x,star.y+sizex*3-20,star.z,sizex,sizex*3,0.2f,0.2f,0.2f);
		}
		}
	}
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
  glDepthMask(1);
}
//#################################################################//
#include "OBJ.h"
Fisch* tree=NULL;
Fisch* tower=NULL;
Fisch* artifact=NULL;
Fisch* facility=NULL;
Fisch* robo=NULL;
Fisch* plant=NULL;

void GFX::Entities(ENTITY *entity,matrix view)
{
	if(entity==NULL) return;

	if(tree==NULL)
	{
		tree=new Fisch("tree");
		tower=new Fisch("tower");
		artifact=new Fisch("artifact");
		facility=new Fisch("facility");
		robo=new Fisch("robo");
		plant=new Fisch("plant");
	}

	int i=0;
	while(entity[i].id!=-1)
	{
		int id=entity[i].id;

		matrix 
		objPos=view;
		objPos+=entity[i].pos;

		if((id==0)&&(tower!=NULL))
		{
			objPos*=vector(560,560,560);
			glLoadMatrixf(objPos.a);
			setObjTex(tower->texture[0],tower->texture[1],tower->texture[2]);
			polycount+=tower->show(0,0);
			clearMultiTex();
		}
		else
		if((id==1)&&(artifact!=NULL))
		{
			objPos*=vector(560,560,560);
			setObjTex(artifact->texture[0],artifact->texture[1],artifact->texture[2]);
			glLoadMatrixf(objPos.a);
			polycount+=artifact->show(0,0);
			clearMultiTex();
		}
		else
		if((id==2)&&(facility!=NULL))
		{
			objPos*=vector(560,560,560);
			setObjTex(facility->texture[0],facility->texture[1],facility->texture[2]);
			glLoadMatrixf(objPos.a);
			polycount+=facility->show(0,0);
			clearMultiTex();
		}
		else
		if((id==3)&&(tree!=NULL))
		{
			objPos*=vector(32,32,32);
			glLoadMatrixf(objPos.a);
			setObjTex(tree->texture[0],tree->texture[1],tree->texture[2]);
			polycount+=tree->show(0,0);
			clearMultiTex();
		}
		else
		if((id==4)&&(robo!=NULL))
		{
			objPos*=vector(32,32,32);
			glLoadMatrixf(objPos.a);
			setObjTex(robo->texture[0],robo->texture[1],robo->texture[2]);
			polycount+=robo->show(0,0);
			clearMultiTex();
		}
		else
		if((id==5)&&(plant!=NULL))
		{
			objPos*=vector(2,2,2);
			glLoadMatrixf(objPos.a);
			setObjTex(plant->texture[0],plant->texture[1],plant->texture[2]);
			polycount+=plant->show(0,0);
			clearMultiTex();
		}
		i++;
	}
    glDisable(GL_ALPHA_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
//	glDepthMask(1);
	glLoadMatrixf(view.a);
}

void GFX::Plants(SCENE *scene,int scale,matrix view)
{
	if(scene==NULL)return;
	if(scene->v_count<10)return;
	if(scene->v_indexcount==0)return;
	if(scene->entitiesCount<10)return;
	if(scene->entities==NULL)return;
	if(scene->entitiesNV==NULL)return;

	if(tree==NULL)
	{
		tree=new Fisch("tree");
		tower=new Fisch("tower");
		artifact=new Fisch("artifact");
		facility=new Fisch("facility");
	}
	
	if(tree==NULL) return;
	if(tower==NULL) return;

	if(gfx_planttex==-1)
	{
		gfx_planttex=loadTex("Data/plant2.bmp");
    }
	matrix objPos;

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	setTex(gfx_planttex);

	//if(scale!=64)
	//{
    glEnable(GL_ALPHA_TEST);
	//glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glAlphaFunc(GL_GREATER, 0);
	//glEnable(GL_BLEND);
	//glDisable(GL_CULL_FACE);
	//}
	
	vector star=vector(0,0,0);

//    glDepthMask(0);
	if(scene->entitiesCount>10000)scene->entitiesCount=10000;

	int towercount=1;

	//if(0)
	for(int i=0;i<scene->entitiesCount;i+=1)
    if(scene->entities[i].y<10)
    if(scene->entitiesNV[i].y>0.0f)
	{
		star=scene->entities[i];

		int id=scene->entitiesID[i]%3;

		//if(scale==64)
		{
		
		objPos=view;

		//if(towercount>0)
		if(id==0)
		{
			//star.y-=500;
			objPos+=star;
			objPos*=vector(560,560,560);
			glLoadMatrixf(objPos.a);
			setObjTex(tower->texture[0],tower->texture[1],tower->texture[2]);
			polycount+=tower->show(0,0);
			//towercount--;
			clearMultiTex();
		}
		else
		if((id==1)&&(artifact!=NULL))
		{
			//star.y-=500;
			objPos+=star;
			objPos*=vector(560,560,560);
			setObjTex(artifact->texture[0],artifact->texture[1],artifact->texture[2]);
			glLoadMatrixf(objPos.a);
			polycount+=artifact->show(0,0);
			clearMultiTex();
		}
		else
		{
			//star.y-=500;
			objPos+=star;
			objPos*=vector(560,560,560);
			setObjTex(facility->texture[0],facility->texture[1],facility->texture[2]);
			glLoadMatrixf(objPos.a);
			polycount+=facility->show(0,0);
			clearMultiTex();
		}
		/*
		else
		if((((i)%28)==10)&&(tree!=NULL))
		{
			star.y-=6;
			objPos+=star;
			objPos*=vector(32,32,32);
			glLoadMatrixf(objPos.a);
			setObjTex(tree->texture[0],tree->texture[1],tree->texture[2]);
			polycount+=tree->show(0,0);
			clearMultiTex();
		}
		  */

		}
		//else{
		

		int sizex=scale;
	
		if(sizex>0)
		{
		 //drawQuadZ(star.x,star.y-sizex,star.z,sizex,sizex,0.3f,0.3f,0.3f);
		 //drawQuadX(star.x,star.y-sizex,star.z,sizex,sizex,0.3f,0.3f,0.3f);
		}

		//}
		
	}
    glDisable(GL_ALPHA_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
//	glDepthMask(1);
	glLoadMatrixf(view.a);


}
//#################################################################//
