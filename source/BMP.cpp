//#################################################################//
#include "BMP.h"
//#################################################################//
BMP::BMP()
{
	data=NULL;
	data_tmp=NULL;
	lens_lookup=NULL;
}
//#################################################################//
BMP::BMP(char*filename)
{
	data=NULL;
	data_tmp=NULL;
	lens_lookup=NULL;
	load(filename);
}
//#################################################################//
BMP::BMP(int x,int y,int b,unsigned char*buffer)
{
	data=NULL;
	data_tmp=NULL;
	lens_lookup=NULL;
	set(x,y,b,buffer);
}
//#################################################################//
BMP::~BMP()
{
	if (data) free(data);
	if (data_tmp) free(data_tmp);
	if (lens_lookup) free(lens_lookup);
}
//#################################################################//
bool BMP::save(char*filename)
{
	unsigned char bmp[58]=
			{0x42,0x4D,0x36,0x30,0,0,0,0,0,0,0x36,0,0,0,0x28,0,0,0,
	           	0x40,0,0,0, // X-Size
	           	0x40,0,0,0, // Y-Size
                   	1,0,0x18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	bmp[18]	=width;
	bmp[19]	=width>>8;
	bmp[22]	=height;
	bmp[23]	=height>>8;
	bmp[28]	=bpp;

	FILE* fn;
	if ((fn = fopen (filename,"wb")) != NULL)
	{
		fwrite(bmp ,1,54   ,fn);
		fwrite(data,1,width*height*(bpp/8),fn);
		fclose(fn);
		return true;
	}
	return false;
}
//#################################################################//
bool  BMP::addalpha(unsigned char r,unsigned char g,unsigned char b)
{
  if(bpp==32)return true;

	unsigned char *data32=(unsigned char*)malloc(width*height*4);
	int x,y,c;
		for(x=0;x<width;x++)
			for(y=0;y<height;y++)
			{
				data32[(y*width+x)*4+0]=data[(y*width+x)*3+0];
				data32[(y*width+x)*4+1]=data[(y*width+x)*3+1];
				data32[(y*width+x)*4+2]=data[(y*width+x)*3+2];
				data32[(y*width+x)*4+3]=255;
				if(abs(r-data[(y*width+x)*3+0])<20)
				if(abs(g-data[(y*width+x)*3+1])<20)
				if(abs(b-data[(y*width+x)*3+2])<20)
				{
					data32[(y*width+x)*4+0]=0;
					data32[(y*width+x)*4+1]=0;
					data32[(y*width+x)*4+2]=0;
					data32[(y*width+x)*4+3]=0;
				}
			}
	free(data);
	data=data32;               
	bpp=32;
	
	return true;
}
//#################################################################//
bool  BMP::normalize()
{
	int x,y,c,tmp;
	int bytes=bpp/8;
	int val0,val1,val2;

	if(bytes<3)return false;

	for(x=0;x<width;x++)
	for(y=0;y<height;y++)
	{
		tmp=0;
		val0=data[(y*width+x)*bytes+0];
		val1=255-val0;
		val2=data[(y*width+x)*bytes+2];
		val0=val0*(255-val2)/255;
		val1=val1*(255-val2)/255;

		data[(y*width+x)*bytes+0]=val0;
		data[(y*width+x)*bytes+1]=val1;
		data[(y*width+x)*bytes+2]=val2;
	}
	
	return true;
}
//#################################################################//
inline int BMP::sampleMap(int x,int y)
{
  return (255-data[(  (y%height)*width+x%width )*(bpp/8)]);
}
//#################################################################//
bool BMP::normalMap(void)
{
	unsigned char* tmpData=(unsigned char*)malloc(width*height*3);

	for(int y=0;y<height;y++)
	for(int x=0;x<width;x++)
	{
		int h0=sampleMap(x, y);
		int hs=sampleMap(x-3, y);
		int ht=sampleMap(x, y-3);

		vector vs(10,0,(hs-h0));
		vector vt(0,10,(ht-h0));
		vector n=vs^vt;
		n=!n;

		tmpData[(x+y*width)*3+0]=(float)(n.x*100+128);
		tmpData[(x+y*width)*3+1]=(float)(n.y*100+128);
		tmpData[(x+y*width)*3+2]=(float)(n.z*100+128);
	}

	free(data);

	bpp=24;
	data=tmpData;

	return true;
}
//#################################################################//
unsigned char BMP::calcHorizon(int x,int y,vector dir)
{
	int bytes=bpp/8;
	int maxH=0;
	int iniH=data[(x+y*width)*bytes];

	vector origin(x,y,0);
	vector pos(x,y,0);
	dir*=2;
	pos+=dir;
	for(int i=0;i<width/2;i++)
	{
		pos=origin+dir*i;
		int curX=pos.x;curX%=width;
		int curY=pos.y;curY%=height;
		int curH=data[(curX+curY*width)*bytes];
		if(curH>iniH)
		{
			vector flat=pos-origin;
			vector elev=flat;
			elev.z=(float)(curH-iniH)*((float)width/10.0f)/255.0f;
			elev=!elev;
			flat=!flat;
			int curAn=(float)(1-(elev*flat))*255.0f;
			if(curAn>maxH)maxH=curAn;
			if(curH==255)return curAn;
		}
	}
	
	return maxH;
}
//#################################################################//
bool BMP::horizonMap(void)
{
	int samples=32;
	int bytes=bpp/8;
	unsigned char* tmpData=(unsigned char*)malloc(width*height*samples);
	float zoom=((float)width/10.0f)/255.0f;
								  
	for(int k=0;k<samples;k++)
	for(int y=0;y<height;y++)
	for(int x=0;x<width;x++)
	{
	    float angle=2*PI*((float)k)/((float)samples);
		vector dir=vector(cos(angle),sin(angle),0);

		int maxAn=0;
		int iniH=data[(x+y*width)*bytes];

		vector origin(x,y,0);
		vector pos(x,y,0);
		//dir*=2;
		pos+=dir;
		for(int i=0;i<width;i++)
		{			 			
			int curX=pos.x;curX%=width;
			int curY=pos.y;curY%=height;
			int curH=data[(curX+curY*width)*bytes];
			if(curH>iniH)
			{
				//vector flat=dir;
				vector elev=pos-origin;
				elev.z=(float)(curH-iniH)*zoom;
				elev=!elev;
				int curAn=(float)(1-(elev*dir))*255.0f;
				if(curAn>maxAn)maxAn=curAn;
				if(curH==255)
				{
				 tmpData[k*width*height+y*width+x]=maxAn;
				 i=width;
				 continue;
				}
			}
			pos+=dir;
		}		
		tmpData[k*width*height+y*width+x]=maxAn;
	}
	free(data);
	bpp=8*samples;
	data=tmpData;

	return true;
}
//#################################################################//
vector BMP::elevGetCubeVector(int side, int x,int y)
{
  float s = (float)(x + 0.5) / (float)width;
  float t = (float)(y + 0.5) / (float)width;
  float sc = s*2 - 1;
  float tc = t*2 - 1;

  if(side==0)return vector(1, -tc, -sc);
  if(side==1)return vector(-1, -tc, sc);
  if(side==2)return vector(sc, 1, tc);
  if(side==3)return vector(sc, -1, -tc);
  if(side==4)return vector(sc, -tc, 1);
  if(side==5)return vector(-sc, -tc, -1);

  return vector(1,1,1);
}
//#################################################################//
bool BMP::createElevationLookupMap(int side)
{
    if(data!=NULL) free(data);
	data=(unsigned char*)malloc(width*height*2);
	bpp=16;
	
	for (int y=0;y<height;y++)
    {
      for (int x=0;x<width;x++)
      {
        vector  v=elevGetCubeVector(side, x, y);

        // Calculate elevation:
        vector e0(v.x, v.y, 0);
		e0=!e0;
        float elevation = 1 - (e0 * v);

        // Calculate heading:
        float heading = acos(e0.x);
        if (e0.y < 0 ) 
          heading = PI + (PI - heading);

        // Write them to the cube map.
        data[(y*width + x)*2 + 0] = (heading/(2*PI))*255;
        data[(y*height+ x)*2 + 1] =  elevation*255;
      }
    }
	return true;
}
//#################################################################//
bool  BMP::blur(int count)
{
	int x,y,b,c;
	int bytes=bpp/8;
	for(c=0;c<count;c++)
		for(x=0;x<width-1;x++)
			for(y=0;y<height-1;y++)
				for(b=0;b<bytes;b++)
					data[(y*width+x)*bytes+b]=
					    (	(int)data[((y+0)*width+x+0)*bytes+b]+
					      (int)data[((y+0)*width+x+1)*bytes+b]+
					      (int)data[((y+1)*width+x+0)*bytes+b]+
					      (int)data[((y+1)*width+x+1)*bytes+b] ) /4;

	return true;
}
//#################################################################//
bool  BMP::hblur(int count)
{
	int x,y,b,a;
	int bytes=bpp/8;

	for(a=0;a<count;a++)
		for(x=0;x<width-1;x++)
			for(y=0;y<height;y++)
				for(b=0;b<bytes;b++)
					data[(y*width+x)*bytes+b]=
					    (	(int)data[((y+0)*width+x+0)*bytes+b]+
					      (int)data[((y+0)*width+x+1)*bytes+b]
					    ) /2;

	return true;
}
//#################################################################//
bool  BMP::vblur(int count)
{
	int x,y,b,a;
	int bytes=bpp/8;

	for(a=0;a<count;a++)
		for(x=0;x<width;x++)
			for(y=0;y<height-1;y++)
				for(b=0;b<bytes;b++)
					data[(y*width+x)*bytes+b]=
					    (	(int)data[((y+0)*width+x+0)*bytes+b]+
					      (int)data[((y+1)*width+x+0)*bytes+b]
					    ) /2;

	return true;
}
//#################################################################//
bool BMP::crop(int x,int y)
{
	if(data==NULL)return false;

	unsigned char* newdata;
	int i,j;

	int bytes=bpp/8;

	newdata=(unsigned char*)malloc(x*y*bytes);

	memset(newdata,0,x*y*bytes);

	for(i=0;i<y;i++)
		if(i<height)
			for(j=0;j<x*bytes;j++)
				if(j<width*bytes)
					newdata[i*x*bytes+j]=data[i*width*bytes+j];
	free(data);
	data=NULL;
	set(x,y,bpp,newdata);
}
//#################################################################//
bool BMP::scale(int x,int y)
{
	if(data==NULL)return false;
	if(x==0)return false;
	if(y==0)return false;

	unsigned char* newdata;
	int i,j,k;

	int bytes=bpp/8;
	newdata=(unsigned char*)malloc(x*y*bytes);
	memset(newdata,0,x*y*bytes);

	for(i=0;i<y;i++)
		for(j=0;j<x;j++)
			for(k=0;k<bytes;k++)
				newdata[i*x*bytes+j*bytes+k]=data[(i*height/y)*(width*bytes)+(j*width/x)*bytes+k];

	free(data);
	data=NULL;
	set(x,y,bpp,newdata);
	return true;
}
//#################################################################//
bool BMP::lens(float distort_x,float distort_y)
{
	int bytes=bpp/8;
	int i,j,k;

	if(data==NULL)return false;
	if(width==0)return false;
	if(height==0)return false;

	if(lens_lookup==NULL) 
	{
		lens_lookup=(int*)malloc(width*height*bytes*sizeof(int));
		lens_distort_x=-1;
		lens_distort_y=-1;
	}

	if((lens_distort_x!=distort_x)||(lens_distort_y!=distort_y))
	{	
		lens_distort_x=distort_x;
		lens_distort_y=distort_y;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
			{
				float radius=sqrt(sqr(i-height/2)+sqr(j-width/2))/
						 sqrt(sqr(width/2)+sqr(height/2))
				;
				float zoom=0.9f;//2/(1.7f+1.35f);

				float amp_x=zoom*(1-pow(1-radius,distort_x))/radius;// 1.7f
				float amp_y=zoom*(1-pow(1-radius,distort_y))/radius;// 1.35f

				int new_x=(int)((j-width/2)*amp_x+width/2);
				int new_y=(int)((i-height/2)*amp_y+height/2);

				if(new_x<0){new_x=0;new_y=0;}
				if(new_y<0){new_x=0;new_y=0;}
				if(new_x>width-1){new_x=0;new_y=0;}
				if(new_y>height-1){new_x=0;new_y=0;}

				for(k=0;k<bytes;k++)
					lens_lookup[(i*width+j)*bytes+k]=(new_y*width+new_x)*bytes+k;
			}
	}
	
	for(k=0;k<width*height*bytes;k++)
		data_tmp[k]=data[lens_lookup[k]];
	
	memcpy(data,data_tmp,width*height*bytes);
	
	return true;
}
//#################################################################//
bool BMP::set(int x,int y,int b,unsigned char*buffer)
{
	width=x;
	height=y;
	bpp=b;
	if(data) free(data);
	if(data_tmp) free(data_tmp);
	if(lens_lookup) free(lens_lookup);
	data=buffer;
	if(data==NULL)
	{
		data=(unsigned char*) malloc(width*height*(bpp/8));
		memset(data,0,width*height*(bpp/8));
	}
	data_tmp=(unsigned char*)malloc(width*height*(bpp/8));
	memset(data_tmp,0,width*height*(bpp/8));

	bmp[18]	=width;
	bmp[19]	=width>>8;
	bmp[22]	=height;
	bmp[23]	=height>>8;
	bmp[28]	=bpp;

	return true;
}
//#################################################################//
bool BMP::load(char *filename)
{
	return load(filename,false,0,0,0);
}
//#################################################################//
bool BMP::load(char *filename,bool checktransparency,int check_r,int check_g, int check_b)
{
	FILE* handle;

	if(filename==NULL)		return false;
	if((char)filename[0]==0)	return false;

	if ((handle = fopen(filename, "rb")) == NULL)
	{
		MessageBox(NULL, "File not found", "Error", MB_OK);
		printf("File not found : %s\n", filename);
		return false;
	}
	if(!fread(bmp, 11, 1, handle))
	{
		MessageBox(NULL, "File read error", "Error", MB_OK);
		printf("Error reading file!\n");
		return false;
	}
	if(!fread(&bmp[11], (int)((unsigned char)bmp[10])-11, 1, handle))
	{
		printf("Error reading file!\n");
		return false;
	}

	width		=(int)((unsigned char)bmp[18])+((int)((unsigned char)(bmp[19]))<<8);
	height	=(int)((unsigned char)bmp[22])+((int)((unsigned char)(bmp[23]))<<8);
	bpp		=bmp[28];

	if(width*height>2048*2048)
	{
		printf("Error reading file - too big!\n");
		fclose(handle);
		return false;
	}

	if(data)free(data);
	if(data_tmp)free(data_tmp);
	if(lens_lookup)free(lens_lookup);

	data_tmp=(unsigned char*)malloc(width*height*(bpp/8));
	memset(data_tmp,0,width*height*(bpp/8));

	switch (bpp)
	{
	case 24:
		short i,r,g,b,alpha;
		alpha=false;
		data=(unsigned char*)malloc(width*height*4);
		fread(data,width*height*3,1,handle);
		for(i=width*height-1;i>=0;i--)
		{
			b=data[i*3+0];
			g=data[i*3+1];
			r=data[i*3+2];
			if((r==check_r)&&(g==check_g)&&(b==check_b))
				alpha=true;
		}

		if(!checktransparency) break;

		if (alpha)
		{
			bpp=32;
			for(i=width*height-1;i>=0;i--)
			{
				b=data[i*4+0]=data[i*3+0];
				g=data[i*4+1]=data[i*3+1];
				r=data[i*4+2]=data[i*3+2];
				data[i*4+3]=255;
				if((r==check_r)&&(g==check_g)&&(b==check_b))
				{
					data[i*4+0]=0;
					data[i*4+1]=0;
					data[i*4+2]=0;
					data[i*4+3]=0;
				}
			}
		}
		break;

	case 32:
		data=(unsigned char*)malloc(width*height*4);
		fread(data,width*height*4,1,handle);
		break;

	default:
		printf("unknown Format - %dx%dx%d Bit",width,height,bpp);
		fclose(handle);
		return false;
	}

	fclose(handle);
	printf("read successfully %s ; %dx%dx%d Bit \n",filename,width,height,bpp);

	return true;
}

//#################################################################//
