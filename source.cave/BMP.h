#ifndef _BMP_class
#define _BMP_class

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VECTOR.h"

class BMP
{
public:

	BMP();
	BMP(int x,int y,int bpp,unsigned char*data);
	BMP(char*filename);
	~BMP();

	bool load(char*filename);
	bool load(char *filename,bool checktransparency,int check_r,int check_g, int check_b);
	bool save(char*filename);
	bool set(int x,int y,int bpp,unsigned char*data);
	bool crop(int x,int y);
	bool scale(int x,int y);
	bool blur(int count);
	bool hblur(int count);
	bool vblur(int count);
	bool lens(float distort_x,float distort_y);
	bool addalpha(unsigned char r,unsigned char g,unsigned char b);
	bool normalize(void);
	bool normalMap(void);
	bool horizonMap(void);
	bool createElevationLookupMap(int side);

private:

	bool headingElevation(void);
	int  sampleMap(int i,int j);
	unsigned char calcHorizon(int x,int y,vector dir);
	vector elevGetCubeVector(int side, int x,int y);

public:

	unsigned char*data;
	unsigned char*cubeMaps[6];
	int width;
	int height;
	int bpp;

private:

	float lens_distort_x;
	float lens_distort_y;
	int* lens_lookup;
	unsigned char* data_tmp;
	unsigned char bmp[54];
};

#endif
