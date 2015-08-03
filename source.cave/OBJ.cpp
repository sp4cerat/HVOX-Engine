//#################################################################//
#include "OBJ.h"
#include "MC.h"
#include "GFX.h"

   vector * v_       ;int v_anz;
   vector * vt       ;int vtanz;
   vector * vn       ;int vnanz;
   int    **f_       ;int f_anz;


//#################################################################//
// Implementation
Fisch::Fisch(){};
//#################################################################//
obj3d Fisch::load(char* cc_name,matrix transformation)
{
	obj3d fischii;

	int i,j;
	char line[1000];
	FILE *fn;

	v_anz=0;
	vtanz=0;
	vnanz=0;
	f_anz=0;

	fischii.v_anz=v_anz=0;
	fischii.vnanz=vnanz=0;
	fischii.vtanz=vtanz=0;
	fischii.f_anz=f_anz=0;


	if ((fn = fopen (cc_name,"rb")) == NULL) return fischii;

	usetex=0;
	objnamnum=0;

	while(( fgets( line, 1000, fn ) != NULL)&&(f_anz<50000))
	{

	 if(strncmp(line,"v ",2)==0)
	 {
	  if(sscanf(line, "v %f %f %f",&v_[v_anz].x, &v_[v_anz].y ,&v_[v_anz].z )==3)
	  { v_[v_anz]*=0.5f;v_anz++; }
	 }else
	 if(strncmp(line,"vt ",3)==0)
	 {
	  if(sscanf(line,"vt %f %f"   ,&vt[vtanz].x, &vt[vtanz].y               )==2)
	   vtanz++;
	 }else
	 if(strncmp(line,"vn ",3)==0)
	 {
	  if(sscanf(line,"vn %f %f %f",&vn[vnanz].x, &vn[vnanz].y ,&vn[vnanz].z )==3)
	   vnanz++;
	 }else
	 if(strncmp(line,"f ",2)==0)
	 {
	  int v9,v10,v11;
	  if(sscanf(line,"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", 
													 &f_[f_anz][0],&f_[f_anz][3],&f_[f_anz][6],
			   										 &f_[f_anz][1],&f_[f_anz][4],&f_[f_anz][7],
													 &f_[f_anz][2],&f_[f_anz][5],&f_[f_anz][8],
													 &v9,&v10,&v11)==12)
	  { 

		  f_[f_anz][9]=objnamnum;
		  
		  f_anz++; 


		  f_[f_anz][0]=f_[f_anz-1][0];
		  f_[f_anz][3]=f_[f_anz-1][3];
		  f_[f_anz][6]=f_[f_anz-1][6];

		  f_[f_anz][1]=f_[f_anz-1][2];
		  f_[f_anz][4]=f_[f_anz-1][5];
		  f_[f_anz][7]=f_[f_anz-1][8];

		  f_[f_anz][2]=v9;
		  f_[f_anz][5]=v10;
		  f_[f_anz][8]=v11;

		  f_[f_anz][9]=objnamnum;


		  f_anz++; usetex=1; 
  
	  }
	  if(sscanf(line,"f %d/%d/%d %d/%d/%d %d/%d/%d", &f_[f_anz][0],&f_[f_anz][3],&f_[f_anz][6],
			   										 &f_[f_anz][1],&f_[f_anz][4],&f_[f_anz][7],
													 &f_[f_anz][2],&f_[f_anz][5],&f_[f_anz][8])==9)	  
	  { f_[f_anz][9]=objnamnum;f_anz++; usetex=1; }
	  else
	  if(sscanf(line,"f %d//%d %d//%d %d//%d",  &f_[f_anz][0],              &f_[f_anz][6],
			   									&f_[f_anz][1],              &f_[f_anz][7],
												&f_[f_anz][2],              &f_[f_anz][8])==6)
	  { f_[f_anz][9]=objnamnum;f_anz++;  }
	  }else
	  if(strncmp(line,"g ",2)==0)
	  {
	   if(sscanf(line,"g %s\n",&objnam[objnamnum*100])==1)
		objnamnum++;

	  };
	}//end while
  
	fclose(fn);
	//-----------//
	  fischii.v_anz=v_anz;
	  fischii.vtanz=vtanz;
	  fischii.vnanz=vnanz;
	  fischii.f_anz=f_anz;
	  fischii.v_=( vector* )malloc(sizeof(vector)*(v_anz+vtanz+vnanz));// if(fischii.v_==NULL)fischii.v_anz=0;
	  fischii.vt=&fischii.v_[v_anz];//( vector* )malloc(sizeof(vector)*vtanz);// if(fischii.vt==NULL)fischii.v_anz=0;
	  fischii.vn=&fischii.v_[v_anz+vtanz];//( vector* )malloc(sizeof(vector)*vnanz);// if(fischii.vn==NULL)fischii.v_anz=0;
	  fischii.f_=( int  ** )malloc(sizeof(int   )*f_anz*(10+1));// if(fischii.f_==NULL)fischii.v_anz=0;

	  for(i=0;i<f_anz;i++)
	  {
	   fischii.f_[i]=(int*)&fischii.f_[f_anz+i*10];//(int *)malloc(sizeof(int)*10);
	   memcpy(fischii.f_[i],f_[i], sizeof(int)*10);
	  }

	  for(i=0;i<v_anz;i++) v_[i].x*=-1;
	  for(i=0;i<v_anz;i++) vn[i].x*=-1;
	  for(i=0;i<v_anz;i++) v_[i]*=transformation;
	  for(i=0;i<vnanz;i++) vn[i]*=transformation;
	  for(i=0;i<vnanz;i++) vn[i] =!vn[i];

	  memcpy(fischii.v_,v_,sizeof(vector)*v_anz);
	  memcpy(fischii.vn,vn,sizeof(vector)*vnanz);
	  memcpy(fischii.vt,vt,sizeof(vector)*vtanz);

	//-----------//
	return fischii;
}
//#################################################################//
Fisch::Fisch(char* fname,bool noLight,matrix transform) 
{
	initialized=false;
	noLightCalc=noLight;
	init(fname,transform);
	initialized=true;
}
//#################################################################//
void Fisch::init(char* fname,matrix transform) 
{
 
   strcpy(name,fname);

   int i,ani=0,anz=1;

   objnam=(char*)malloc(10000);

   if(v_==NULL){

	   vector* allVecData=(vector* )malloc(sizeof(vector)*50000*3);

	   v_=&allVecData[50000*0];//(vector* )malloc(sizeof(vector)*50000);
	   vt=&allVecData[50000*1];//(vector* )malloc(sizeof(vector)*50000);
	   vn=&allVecData[50000*2];//(vector* )malloc(sizeof(vector)*50000);

	   int* f_list=(int *)malloc(sizeof(int)*(10+1)*50000);

	   f_=(int  ** )&f_list[0];//malloc(sizeof(int)*50000);

	   for(i=0;i<50000;i++) 
	   f_[i]=&f_list[50000+i*10];
   }

   for(i=0;i<100;i++)
   {
	texture [i]=-1;
	displist[i]=-1;
	fischi[i].v_anz=0;
   }

   usetex=0;
   
   texture_anz=-1;
   reflection=-1;

   char cc_name[300];
	   
	anz=1;

	sprintf(cc_name,"Data/%s/%s.obj",fname,fname);//,objpath
	fischi[0]=load(cc_name,transform);

	if(fischi[0].f_anz==0)
	{
 	 ani=-1;
	 do
	 {
	  ani++;

   	  if (ani<=9)  sprintf(cc_name,"Data/%s/%s00%d.obj",fname,fname,ani);else//,objpath
   	  if (ani<=99) sprintf(cc_name,"Data/%s/%s0%d.obj",fname,fname,ani);else//,objpath
   	  if (ani<=999)sprintf(cc_name,"Data/%s/%s%d.obj",fname,fname,ani);//,objpath
	    
	  fischi[ani]=load(cc_name,transform);

	 }while (fischi[ani].f_anz!=0);
 	 
	 anz=ani;
	 if(anz<1)anz=1;
	}
	//load anis

	//center fischi
    float x_min=10000,y_min=10000,z_min=10000;
    float x_max=-10000,y_max=-10000,z_max=-10000;
	int t,u;

	for(t=0;t!=anz;t++)
	if(fischi[t].v_anz!=0)
	for(u=0;u!=fischi[t].v_anz;u++)
	{
		if(fischi[t].v_[u].x < x_min) x_min=fischi[t].v_[u].x;
		if(fischi[t].v_[u].y < y_min) y_min=fischi[t].v_[u].y;
		if(fischi[t].v_[u].z < z_min) z_min=fischi[t].v_[u].z;

		if(fischi[t].v_[u].x > x_max) x_max=fischi[t].v_[u].x;
		if(fischi[t].v_[u].y > y_max) y_max=fischi[t].v_[u].y;
		if(fischi[t].v_[u].z > z_max) z_max=fischi[t].v_[u].z;
	}

	float dx=x_max-x_min;if(dx==0)dx=0.01f;
	float dy=y_max-y_min;if(dy==0)dy=0.01f;
	float dz=z_max-z_min;if(dz==0)dz=0.01f;
	if(dy>dx)dx=dy;
	if(dz>dx)dx=dz;

	//float x_mid=(x_min+x_max)/2;
	//float y_mid=(y_min+y_max)/2;
	//float z_mid=(z_min+z_max)/2;

	for(t=0;t!=anz;t++)
	if(fischi[t].v_anz!=0)
	for(u=0;u!=fischi[t].v_anz;u++)
	{
		fischi[t].v_[u].x=(fischi[t].v_[u].x-x_min)/dx-0.5f;
		fischi[t].v_[u].y=(fischi[t].v_[u].y-y_min)/dx-0.5f;
		fischi[t].v_[u].z=(fischi[t].v_[u].z-z_min)/dx-0.5f;

		/*
		if(inv_normals)
		{
			fischi[t].vn[u].x=-fischi[t].vn[u].x;
			fischi[t].vn[u].y=-fischi[t].vn[u].y;
			fischi[t].vn[u].z=-fischi[t].vn[u].z;
		} */
	}
	 
    fischi_anz=anz;
}
//#################################################################//
int Fisch::show(int dist,int tex)
{
	if(!initialized) return 0;

	int i,j;

	int ani=dist;if(ani>fischi_anz)ani=fischi_anz;

	if(fischi[0].f_anz==0)return 0;
	if(fischi[ani].v_anz<=1)return 0;


	if(texture_anz == -1)
	{
  		texture_anz=1;
	    char cc_name[300];

		if(usetex)
		{
			sprintf(cc_name,"Data/%s/%s.bmp",name,name);//,objpath
			texture[0]=GFX::loadTex(cc_name);
			sprintf(cc_name,"Data/%s/%s_detail.bmp",name,name);//,objpath
			texture[1]=GFX::loadTex(cc_name);
			sprintf(cc_name,"Data/%s/%s_glow.bmp",name,name);//,objpath
			texture[2]=GFX::loadTex(cc_name);
			texture_anz=3;
			//load textures
		}
		else
		{
		  texture[0]=0;
		}
	}

	if(texture_anz<1)return 0;

//	glBindTexture  (GL_TEXTURE_2D, texture[0]);

	if(displist[ani]<0)
	{
	 for(int anii=0;anii<fischi_anz;anii++)
	 {
		 GLint lid=glGenLists(1);
		 glNewList(lid, GL_COMPILE);
		 displist[anii]=lid;
			  
		 glBegin (GL_TRIANGLES);	
		 //glColor4f(1,1,1,1);

		  for(i=0;i<fischi[anii].f_anz;i++)
		   {  
		   int vi0=fischi[anii].f_[i][0]-1;
		   int vi1=fischi[anii].f_[i][1]-1;
		   int vi2=fischi[anii].f_[i][2]-1;
		   if(vi0<0)exit(984);
		   if(vi1<0)exit(983);
		   if(vi2<0)exit(982);
		   
		   for(j=0;j<=2;j++)
			{
			  int ti;
			  int vi=fischi[anii].f_[i][j+0]-1;
			  int ni=fischi[anii].f_[i][j+6]-1;//Normal index

			  if(vi<0)exit(987);
			  if(ni<0)exit(765);

			  if(usetex)
			  {
				  ti=fischi[anii].f_[i][j+3]-1;//Texture index
				  if(ti<0)exit(543);
				  glTexCoord2f(  fischi[anii].vt[ti].x,fischi[anii].vt[ti].y);
			  }  

			  int color=MC::getSingleColor((fischi[anii].vn[ni].y),-100);
			  if(noLightCalc)color=0xffffff;
			  int r=(color>>0)&255;
			  int g=(color>>8)&255;
			  int b=(color>>16)&255;

			  glColor3f((float)r/256,(float)g/256,(float)b/256);
			  //glColor3f(1,1,1);

			  //glNormal3f  ( fischi[anii].vn[ni].x,-fischi[anii].vn[ni].z,fischi[anii].vn[ni].y);	
			  //glVertex3f  ( fischi[anii].v_[vi].x,-fischi[anii].v_[vi].z,fischi[anii].v_[vi].y );  
			  glVertex3f  ( fischi[anii].v_[vi].x,-fischi[anii].v_[vi].y,fischi[anii].v_[vi].z );  
			}  	
		   }
     			
		  glEnd ();
		 glEndList();
	  }//end lists
	}//  init end
	if(displist[ani]>=0) glCallList(displist[ani]);
		
	return fischi[ani].f_anz;
}

