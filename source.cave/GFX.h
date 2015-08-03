#ifndef _GL_class
#define _GL_class

#include <windows.h>

#include <GL/gl.h>													// Header File For The OpenGL32 Library
#include <GL/glu.h>													// Header File For The GLu32 Library
#include <GL/glut.h>
#include <GL/glaux.h>												// Header File For The Glaux Library
//nclude "glati.h"
//nclude "wglati.h"
#include <GL/glext.h>


#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>

#include "STRUCTS.h"

#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB 0x88E4

#include "BENCHMARK.h"
#include "VECTOR.h"
#include "BMP.h"
#include "MC.h"
//#include "OBJ.h"

// max. tstrips allowed to be converted to displaylists per scene
#define GFX_CACHE_ALLOWED 2000
// min. lifetime before beeing cached
#define GFX_CACHE_MINLIFE 2 
// min. length o cacheable trianglestrip
#define GFX_CACHE_MINSTRIPSIZE 15

//struct textureImage;

typedef struct
{
	int id;
	int width;
	int height;
	int bpp;
	unsigned char *data;
	bool update;
	void* link; // poiner to next textureImage
}
textureImage;

class GFX
{
public:

    enum TexLoadFlags {
      TEX_ADDALPHA=1,
      TEX_NORMALMAP=2,
      TEX_HORIZONMAP=4,
      TEX_HORIZONLOOKUP=8,
      TEX_NORMALIZE=16,
      TEX_16BIT=32,
    };

	GFX(InfoType *globalInfo);
	~GFX();

	void zBuffer(int mode);// 0=off,1=less,2=greater,3=lessequal,4=greaterequal
	void clear(void);
	void setTex(int id);
	static int loadTex(char *name,int flags=0);
	int loadBMP(char *filename, textureImage *texture);
	bool loadGLTextures();   /* Load Bitmaps And Convert To Textures */
	bool scene();
	void text(float x,float y,float size_x,float size_y,float r,float g,float b,float a,const char *format, ...);
	void showFps(void);
	textureImage* createRawTex(int x,int y,int bpp,unsigned char* data);
	void updateTextures(void);
	void drawVoxels(SCENE* scene,int last,int level,int renderType=GL_TRIANGLE_STRIP);
	void drawFog(int a);

	void drawQuadZ(float x,float y,float z,float sizeX=1,float sizeY=1,float r=1,float g=1,float b=1,float a=1);
	void drawQuadY(float x,float y,float z,float sizeX=1,float sizeY=1,float r=1,float g=1,float b=1,float a=1);
	void drawQuadX(float x,float y,float z,float sizeX=1,float sizeY=1,float r=1,float g=1,float b=1,float a=1);
	
	void Entities(ENTITY *entity,matrix view);
	void Plants(SCENE *scene,int scale,matrix view);
	void Hangs(MC_POINT *points,int count);
	void drawSky(bool flip);
	void TangentAndBinormal(vector inP[3],vector inT[3],vector* outTan,vector* outBin);
	void PlanetPos(int i,int j,vector *p,vector *t);
	void Planet();
	void RingPlanet();
	void Water(vector ofs,vector color,matrix camera);

	void setThresholdTex(int tex0);
	void setGlowTex(int texNormal,int texGlow);
	void setBlurTex(int tex0);
	void setTerrainTex(int tex0,int tex1,int tex2,int texDetail1,int texDetail2,int texDetail3,int selectTex) ;
	void setEnvBumpTex(int tex0,int tex1,int tex2);
	void setEnvBumpTex2(int tex0,int tex1,int tex2); //  mirror , standard , bump
    void setSpecialBump(int texmap,int heightmap,int normalmap, int horizonmap,int horizonlookupmap,
						vector lightpos,vector lightcol,float lightradius, vector  campos);
	void setObjTex(int tex,int detail,int glow);
	void clearMultiTex();

	void loadFPandVP(int num,char* name);
	void screenshot();
	int initGL();


private:

	int isExtensionSupported( const char *extension );
	bool initExtensions(void);
	char* readProgram(const char* name);
	void loadAllPrograms();

//	void mainCheckExtensions (void);
//	int mainExtensionSupported (const char *extension);

public:

	int polycount;
	int mpolycount;
	
	int cachedtstrips;
	int framecount,framecachecount;


	int fps;
		
	unsigned int*	reflectionData;
	unsigned int	reflectionTexture;
	int				waterBumpTexture;
	static int gfx_suntex;
	static int gfx_startex;
	static int gfx_hangtex;
	static int gfx_planttex;
	
private:

    InfoType *info;
	BENCHMARK *benchmark;

	GLuint  texture[1];  /* Storage For One Texture */
	static int texCounter;
	int font;
	int frametime,framestotal;
	textureImage* firstRaw;
	float fov; // field of view; =height/width

	int polycount_tmp;

	unsigned int vpid[10];
	unsigned int fpid[10];

	PFNGLACTIVETEXTUREARBPROC       glActiveTexture;
	PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2f;
	PFNGLGENBUFFERSARBPROC			glGenBuffersARB ;			// VBO Name Generation Procedure
	PFNGLBINDBUFFERARBPROC			glBindBufferARB ;			// VBO Bind Procedure
	PFNGLBUFFERDATAARBPROC			glBufferDataARB ;			// VBO Data Loading Procedure
	PFNGLDELETEBUFFERSARBPROC		glDeleteBuffersARB ;		// VBO Deletion Procedure
//	PFNGLTEXBUMPPARAMETERFVATIPROC  glTexBumpParameterfvATI;
//	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

	//
	// OpenGL Shading Language entry points
	//

	PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
	PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
	PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
	PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
	PFNGLATTACHOBJECTARBPROC glAttachObjectARB;

	PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
	PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
	PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
	PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
	PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;

	PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
	PFNGLGETOBJECTPARAMETERFVARBPROC glGetObjectParameterfvARB;
	PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
	PFNGLGETATTRIBLOCATIONARBPROC glGetAttribLocationARB;

	PFNGLBINDATTRIBLOCATIONARBPROC glBindAttribLocationARB;

	PFNGLUNIFORM1FARBPROC glUniform1fARB;
	PFNGLUNIFORM2FARBPROC glUniform2fARB;
	PFNGLUNIFORM3FARBPROC glUniform3fARB;
	PFNGLUNIFORM4FARBPROC glUniform4fARB;

	PFNGLUNIFORM1IARBPROC glUniform1iARB;
	PFNGLUNIFORM2IARBPROC glUniform2iARB;
	PFNGLUNIFORM3IARBPROC glUniform3iARB;
	PFNGLUNIFORM4IARBPROC glUniform4iARB;

	PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
	PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
	PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
	PFNGLUNIFORM4FVARBPROC glUniform4fvARB;

	PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
	PFNGLUNIFORM2IVARBPROC glUniform2ivARB;
	PFNGLUNIFORM3IVARBPROC glUniform3ivARB;
	PFNGLUNIFORM4IVARBPROC glUniform4ivARB;

	PFNGLVERTEXATTRIB4FVARBPROC glVertexAttrib4fvARB;
	PFNGLVERTEXATTRIB3FVARBPROC glVertexAttrib3fvARB;
	PFNGLVERTEXATTRIB2FVARBPROC glVertexAttrib2fvARB;
	PFNGLVERTEXATTRIB1FVARBPROC glVertexAttrib1fvARB;
	PFNGLVERTEXATTRIB4FARBPROC	glVertexAttrib4fARB;
	PFNGLVERTEXATTRIB3FARBPROC	glVertexAttrib3fARB;
	PFNGLVERTEXATTRIB2FARBPROC	glVertexAttrib2fARB;
	PFNGLVERTEXATTRIB1FARBPROC	glVertexAttrib1fARB;

	static PFNGLTEXIMAGE3DPROC	glTexImage3D;

	PFNGLGENPROGRAMSARBPROC		glGenProgramsARB;
	PFNGLBINDPROGRAMARBPROC		glBindProgramARB;
	PFNGLPROGRAMSTRINGARBPROC	glProgramStringARB;
	PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
	PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;

};

#endif
