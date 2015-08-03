#pragma once
/*
#include "compression/huffman.h"
#include "compression/lz.h"
#include "compression/rice.h"
#include "compression/rle.h"
#include "compression/shannonfano.h"
*/
#define loop(a_l,start_l,end_l) for ( a_l = start_l;a_l<end_l;++a_l )
#define loops(a_l,start_l,end_l,step_l) for ( a_l = start_l;a_l<end_l;a_l+=step_l )
#define byte unsigned char

class RLE
{
public :

	class Stick
	{
		public:
		short start;
		short end;
	};
	class StickCol
	{
		public:
		unsigned char mat;
		unsigned char nx1,ny1,nz1;
		unsigned char mat2;
		unsigned char nx2,ny2,nz2;
	};

	class Elem //X-Z-Element
	{
		public:
		char len;
		int stick; //offset
	};

	Elem *map;
	Stick *sticks;
	int numsticks;	

	class MipMap
	{
	public:
		int sx,sy,sz;
		Elem *map;
		Stick *sticks;
		StickCol *sticks_col ;
		int numsticks;	
	};

	int nummipmaps;
	MipMap *mipmap;
	MipMap *mipmap_coarse;
	int mipmap_coarse_scale;
	int mipmap_coarse_shift;

	class Tile
	{
		public:

		int tilesize;	

		void init (int tilesize, int numsticks)
		{
			this->tilesize = tilesize;
			//this->numsticks = numsticks;

			//sticks = (Stick*)malloc ( numsticks*sizeof(Stick) );
			//map = (Elem*) malloc ( tilesize*tilesize*sizeof(Elem) );
		}
	};

	int tilesize;
	int tx,tz;
	int sx,sy,sz;

	Tile *tiles;

	/*------------------------------------------------------*/

	void save(char *filename)
	{
		printf("Saving %s\n",filename);

		FILE* fn; 
		
		if ((fn = fopen (filename,"wb")) == NULL) return;

		fwrite(&sx,1,4,fn);
		fwrite(&sy,1,4,fn);
		fwrite(&sz,1,4,fn);
		fwrite(&numsticks,1,4,fn);
		fwrite(map,1,sx*sz*sizeof(Elem),fn);
		fwrite(sticks,1,numsticks*sizeof(Stick),fn);

		fclose(fn);
	}

	/*------------------------------------------------------*/

	bool load(char *filename)
	{
		FILE* fn; 

		if ((fn = fopen (filename,"rb")) == NULL) return false;

		printf("Loading %s\n",filename);

		fread(&sx,1,4,fn);
		fread(&sy,1,4,fn);
		fread(&sz,1,4,fn);
		fread(&numsticks,1,4,fn);

		map = (Elem*) malloc ( sx*sz*sizeof(Elem) );
		sticks = (Stick*)malloc ( numsticks*sizeof(Stick) );

		fread(map,1,sx*sz*sizeof(Elem),fn);
		fread(sticks,1,numsticks*sizeof(Stick),fn);

		fclose(fn);

		return true;

	//	gen_mipmaps();
	}

	/*------------------------------------------------------*/

	unsigned char* load_raw_16(int mx,int my,int mz,char *filename)
	{
		unsigned short *tmp = (unsigned short*) malloc(2*mx*my*mz);
		FILE* fn; 

		if ((fn = fopen (filename,"rb")) == NULL) return false;

		printf("Loading %s\n",filename);

		fread(tmp,1,mx*my*mz*2,fn);

		fclose(fn);

		int mx8 = mx/8;

		unsigned char* data = (unsigned char*)malloc((mx/8)*my*mz);
		memset (data,0,mx8*my*mz);

		int a,b,c;
		loop(a,0,mx)
		loop(b,0,my)
		loop(c,0,mz)
		{
			int src = a+b*mx+c*mx*my;
			int dst = (a/8)+b*mx8+c*mx8*my;
			int dbit = 1<<(a&7);
			unsigned char b1 = tmp[src] /256;
			unsigned char b2 = tmp[src] &255;
			if (b1+b2*256>556) data[dst]|=dbit;
		}
		sx=mx;
		sy=my;
		sz=mz;
		free(tmp);

		return data;
	}

	/*------------------------------------------------------*/

	unsigned char* load_raw_8(int mx,int my,int mz,char *filename)
	{
		unsigned char *tmp = (unsigned char*) malloc(mx*my*mz);
		FILE* fn; 

		if ((fn = fopen (filename,"rb")) == NULL) return false;

		printf("Loading %s\n",filename);

		fread(tmp,1,mx*my*mz,fn);

		fclose(fn);

		int mx8 = mx/8;

		unsigned char* data = (unsigned char*)malloc((mx/8)*my*mz*2);
		memset (data,0,mx8*my*mz*2);

		int a,b,c;
		loop(a,0,mx)
		loop(b,0,my)
		loop(c,0,mz*2)
		{
			int src = a+b*mx+(c/2)*mx*my;
			int dst = (a/8)+b*mx8+c*mx8*my;
			int dbit = 1<<(a&7);
			unsigned char b1 = tmp[src] /256;
			unsigned char b2 = tmp[src] &255;
			if (tmp[src]>50) data[dst]|=dbit;
		}
		sx=mx;
		sy=my;
		sz=mz*2;
		free(tmp);

		return data;
	}

	/*------------------------------------------------------*/

	char* uncompress()
	{
		printf("uncompressing...");

		char* data = (char*)malloc(sx*sy*sz/8);

		int k,x,y,z;

		int num = 0;

		int sx8 = sx/8;

		memset (data,0,sx*sy*sz/8);

		//get max complexity
		for ( x = 0; x < sx ; x++ )
		for ( z = 0; z < sz ; z++ )
		{
			Elem &elem = map[ x+z*sx ];
			
			int pre1 = (x)/8+(z)*sx8*sy;
			unsigned char pre2 = 1<<(x&7);

			for ( k = elem.stick ; k < elem.stick + elem.len ; k++ )
			{
				if( sticks[k].start < 0 )
					loop (y,sticks[k-1].end,sticks[k].end) data[pre1+y*sx8] |= pre2;
				else
					loop (y,sticks[k].start,sticks[k].end) data[pre1+y*sx8] |= pre2;
			}
		}
		return data;
	}

	/*------------------------------------------------------*/
	/*------------------------------------------------------*/
};


