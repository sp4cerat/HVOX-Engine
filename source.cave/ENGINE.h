#ifndef _ENGINE_class
#define _ENGINE_class

#include "Vector.h"
#include "GFX.h"
#include "MC.h"
#include "HOUSE.h"
#include "OBJ.h"
#include "Structs.h"
#include "PBUFFER.h"
#include <signal.h>
#include <mmsystem.h>

#define MAX_ENTITIES 5000
#define ClipBoxCount 7

class ENGINE
{
public:

	ENGINE(InfoType *globalInfo);

	void renderWaterTex();
	void saveEntities(char* filename);
	void loadEntities(char* filename);
	void shootHandler();
	void shootPainter(matrix view);

	int init();

	int run ();

	int quit();

    void getSeed();

	~ENGINE();


private:

	static void voxelGetFps(void);

	static bool houseupdate(HOUSE* houseptr ,bool forceUpdate=false);

	//static void CALLBACK voxelUpdate( UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 ) ;
	static DWORD WINAPI voxelUpdate( LPVOID lpParameter );
	//void *voxelUpdate(void * arg);

public:

//private:

	static InfoType *info;
	static bool threadRunning; // voxelUpdateThread
	static int instances; // voxelUpdateThread

	static ENGINE* self;

	vector viewpos;
	vector viewrot;
	matrix viewmat;
	GFX	   *gfx;

	SCENE* scene[ClipBoxCount];

	MC* mcp;

	int groundtexture;

	int shootTex;
	int terrainTex0;
	int terrainTex1;
	int terrainTex2;
	int terrainTexDetail0;
	int terrainTexDetail1;
	int terrainTexDetail2;
	int terrainTexSelect;

    int wireframe ;
    int skybox ;
    int useTexture;
    int useFog;

    static int randSize;
    static int randArray [ 16384 ];

	HANDLE  Threadhandle;

	ENTITY entity[MAX_ENTITIES];
	int    entityCount;

	ENTITY shoot[MAX_ENTITIES];
	int    shootCount;

	Fisch* weapon;
};

#endif
