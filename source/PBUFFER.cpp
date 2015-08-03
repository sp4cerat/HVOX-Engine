//-----------------------------------------------------------------------------
//           Name: ogl_offscreen_rendering_1.cpp
//         Author: Kevin Harris (kevin@codesampler.com)
//  Last Modified: 08/13/04
//    Description: This sample demonstrates how to create dynamic textures 
//                 through off-screen rendering. The off-screen rendering step 
//                 is accomplished using a p-buffer, which is created using 
//                 OpenGL's WGL_ARB_pbuffer, and WGL_ARB_pixel_format 
//                 extensions. The step of dynamic texture creation from a 
//                 p-buffer involves reading the pixel data out of the p-buffer 
//                 with glReadPixels and then loading it into our texture with
//                 a regular call to glTexImage2D. This is by far the slowest 
//                 way to create dynamic textures from a p-buffer.
//                 
//                 As a demonstration, a spinning textured cube is rendered 
//                 to a p-buffer, which is in turn, used to create a dynamic 
//                 texture. The dynamic texture is then used to texture a 
//                 second spinning cube, which will be rendered to the 
//                 application's window.
//
//   Control Keys: Left Mouse Button  - Spin the large, black cube.
//                 Right Mouse Button - Spin the textured cube being rendered 
//                                      into the p-buffer.
//-----------------------------------------------------------------------------

#include "pbuffer.h"
#include <windows.h>
#include <stdlib.h>
#include <mmsystem.h>
#include <GL/gl.h>
#include <GL/glaux.h>
#include "resource.h"
#include "wglext.h"      // Sample's header file
//#include <GL/wglext.h> // Your local header file

//-----------------------------------------------------------------------------
// FUNCTION POINTERS FOR OPENGL EXTENSIONS
//-----------------------------------------------------------------------------

// For convenience, this project ships with its own "wglext.h" extension header 
// file. If you have trouble running this sample, it may be that this "wglext.h" 
// file is defining something that your hardware doesn’t actually support. 
// Try recompiling the sample using your own local, vendor-specific "wglext.h" 
// header file.

#include "wglext.h"      // Sample's header file
//#include <GL/wglext.h> // Your local header file

// WGL_ARB_extensions_string
PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = NULL;

// WGL_ARB_pbuffer
PFNWGLCREATEPBUFFERARBPROC    wglCreatePbufferARB    = NULL;
PFNWGLGETPBUFFERDCARBPROC     wglGetPbufferDCARB     = NULL;
PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB = NULL;
PFNWGLDESTROYPBUFFERARBPROC   wglDestroyPbufferARB   = NULL;
PFNWGLQUERYPBUFFERARBPROC     wglQueryPbufferARB     = NULL;

// WGL_ARB_pixel_format
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;

// WGL_ARB_render_texture
PFNWGLBINDTEXIMAGEARBPROC     wglBindTexImageARB     = NULL;
PFNWGLRELEASETEXIMAGEARBPROC  wglReleaseTexImageARB  = NULL;
PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB = NULL;

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND   g_hWnd             = NULL;
HDC	   g_hDC              = NULL;
HGLRC  g_hRC              = NULL;
GLuint g_testTextureID    = -1;
GLuint g_dynamicTextureID = -1;

typedef struct {

    HPBUFFERARB hPBuffer;
    HDC         hDC;
    HGLRC       hRC;
    int         nWidth;
    int         nHeight;

} PBUFFERTYPE;

PBUFFERTYPE g_pbuffer;

int PBUFFER_WIDTH  = 1024;
int PBUFFER_HEIGHT = 1024;

static bool pbufferInitialized = false;


#define SHOWERROR(a) {MessageBox(NULL,a,"Error",MB_OK);exit(0);}

// ****************************************************************************
// pbufferInit: initializes a pbuffer of size width and height.  this function
// takes care of finding a reasonable pixel format and then allocates the a 
// chunk of video memory for use as an off screen buffer.
// ****************************************************************************
bool pbufferInit (int width, int height)
{
   PBUFFER_WIDTH = width;
   PBUFFER_HEIGHT = height;

   if( pbufferInitialized ) return true;
   pbufferInitialized = true;

   g_hRC = wglGetCurrentContext();
   g_hDC = wglGetCurrentDC();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	//
	// This is our dynamic texture, which will be loaded with new pixel data
	// after we're finshed rendering to the p-buffer.
	//

	glGenTextures( 1, &g_dynamicTextureID );
	glBindTexture( GL_TEXTURE_2D, g_dynamicTextureID );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, PBUFFER_WIDTH, PBUFFER_HEIGHT, 0, GL_RGB, GL_FLOAT, 0 );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR );

	//
	// If the required extensions are present, get the addresses for the
	// functions that we wish to use...
	//

	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	char *ext = NULL;
	
	if( wglGetExtensionsStringARB )
		ext = (char*)wglGetExtensionsStringARB( wglGetCurrentDC() );
	else
	{
		MessageBox(NULL,"Unable to get address for wglGetExtensionsStringARB!",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
	}

	//
	// WGL_ARB_pbuffer
	//

	if( strstr( ext, "WGL_ARB_pbuffer" ) == NULL )
	{
		MessageBox(NULL,"WGL_ARB_pbuffer extension was not found",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
	}
	else
	{
		wglCreatePbufferARB    = (PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
		wglGetPbufferDCARB     = (PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
		wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
		wglDestroyPbufferARB   = (PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
		wglQueryPbufferARB     = (PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");

		if( !wglCreatePbufferARB || !wglGetPbufferDCARB || !wglReleasePbufferDCARB ||
			!wglDestroyPbufferARB || !wglQueryPbufferARB )
		{
			MessageBox(NULL,"One or more WGL_ARB_pbuffer functions were not found",
				"ERROR",MB_OK|MB_ICONEXCLAMATION);
			exit(-1);
		}
	}

	//
	// WGL_ARB_pixel_format
	//

	if( strstr( ext, "WGL_ARB_pixel_format" ) == NULL )
	{
		MessageBox(NULL,"WGL_ARB_pixel_format extension was not found",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
	}
	else
	{
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");

		if( !wglChoosePixelFormatARB )
		{
			MessageBox(NULL,"One or more WGL_ARB_pixel_format functions were not found",
				"ERROR",MB_OK|MB_ICONEXCLAMATION);
			exit(-1);
		}
	}

	//
	// WGL_ARB_render_texture
	//

	if( strstr( ext, "WGL_ARB_render_texture" ) == NULL )
	{
		MessageBox(NULL,"WGL_ARB_render_texture extension was not found",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
	}
	else
	{
		wglBindTexImageARB     = (PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
		wglReleaseTexImageARB  = (PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");
		wglSetPbufferAttribARB = (PFNWGLSETPBUFFERATTRIBARBPROC)wglGetProcAddress("wglSetPbufferAttribARB");

		if( !wglBindTexImageARB || !wglReleaseTexImageARB || !wglSetPbufferAttribARB )
		{
			MessageBox(NULL,"One or more WGL_ARB_render_texture functions were not found",
				"ERROR",MB_OK|MB_ICONEXCLAMATION);
			exit(-1);
		}
	}

	//-------------------------------------------------------------------------
	// Create a p-buffer for off-screen rendering.
	//-------------------------------------------------------------------------

	g_pbuffer.hPBuffer = NULL;
	g_pbuffer.nWidth   = PBUFFER_WIDTH;
	g_pbuffer.nHeight  = PBUFFER_HEIGHT;

	//
	// Define the minimum pixel format requirements we will need for our 
	// p-buffer. A p-buffer is just like a frame buffer, it can have a depth 
	// buffer associated with it and it can be double buffered.
	//

	int pf_attr[] =
	{
		WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
		WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer
		WGL_BIND_TO_TEXTURE_RGBA_ARB, TRUE, // P-buffer will be used as a texture
		WGL_RED_BITS_ARB, 8,                // At least 8 bits for RED channel
		WGL_GREEN_BITS_ARB, 8,              // At least 8 bits for GREEN channel
		WGL_BLUE_BITS_ARB, 8,               // At least 8 bits for BLUE channel
		WGL_ALPHA_BITS_ARB, 8,              // At least 8 bits for ALPHA channel
		//WGL_DEPTH_BITS_ARB, 16,             // At least 16 bits for depth buffer
		WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
		0                                   // Zero terminates the list
	};

	unsigned int count = 0;
	int pixelFormat;
	wglChoosePixelFormatARB( g_hDC,(const int*)pf_attr, NULL, 1, &pixelFormat, &count);

	if( count == 0 )
	{
		MessageBox(NULL,"Could not find an acceptable pixel format!",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
	}

	//
	// Set some p-buffer attributes so that we can use this p-buffer as a
	// 2D RGBA texture target.
	//

	int pb_attr[] =
	{
		WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB, // Our p-buffer will have a texture format of RGBA
		WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,   // Of texture target will be GL_TEXTURE_2D
		0                                             // Zero terminates the list
	};

	//
	// Create the p-buffer...
	//

	g_pbuffer.hPBuffer = wglCreatePbufferARB( g_hDC, pixelFormat, g_pbuffer.nWidth, g_pbuffer.nHeight, pb_attr );
	g_pbuffer.hDC      = wglGetPbufferDCARB( g_pbuffer.hPBuffer );
	g_pbuffer.hRC      = wglCreateContext( g_pbuffer.hDC );

	if( !g_pbuffer.hPBuffer )
	{
		MessageBox(NULL,"Could not create the p-buffer",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
	}

	int h;
	int w;
	wglQueryPbufferARB( g_pbuffer.hPBuffer, WGL_PBUFFER_WIDTH_ARB, &h );
	wglQueryPbufferARB( g_pbuffer.hPBuffer, WGL_PBUFFER_WIDTH_ARB, &w );

	if( h != g_pbuffer.nHeight || w != g_pbuffer.nWidth )
	{
		MessageBox(NULL,"The width and height of the created p-buffer don't match the requirements!",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
	}

// make a texture object for bind the pbuffer to
//   glGenTextures(1, &pbufferTextureID);
//   glBindTexture(GL_TEXTURE_2D, pbufferTextureID);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   pbufferInitialized = true;
   return(true);
}

// ****************************************************************************
// pbufferEnable: change the current context such that we will now be writing
// to the pbuffer.  also, resize viewport to size width by height.
// ****************************************************************************
void pbufferEnable (int width, int height)
{
   // make sure the pbuffer has been initialized
   if (!(pbufferInitialized))
   {
      return;
   }
    int flag = 0;
    wglQueryPbufferARB( g_pbuffer.hPBuffer, WGL_PBUFFER_LOST_ARB, &flag );

    if( flag != 0 )
    {
        MessageBox(NULL,"The p-buffer was lost!",
            "ERROR",MB_OK|MB_ICONEXCLAMATION);
        exit(-1);
    }

	if( !wglMakeCurrent( g_pbuffer.hDC, g_pbuffer.hRC ) )
	{
		MessageBox(NULL,"Could not make the p-buffer's context current!",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
        exit(-1);
    }

   glViewport(0.0, 0.0, width, height);
   glDrawBuffer( GL_BACK );
   glReadBuffer( GL_BACK );

   return ;
}

// ****************************************************************************
// pbufferDisable: change the current context such that we will now be writing
// to the framebuffer again.  also, resize viewport to size width by height.
// ****************************************************************************
void pbufferDisable (int width, int height)
{
//
    // Before we forget, we need to make sure that the p-buffer has been 
    // released from the dynamic "render-to" texture.
    //

	if( !wglMakeCurrent( g_hDC, g_hRC ) )
	{
		MessageBox(NULL,"Could not make the window's context current!",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
        exit(-1);
    }

   glViewport(0.0, 0.0, width, height);

   return;
}
void pbufferBind (void)
{
   // make sure we haven't lost our pbuffer due to a display mode change
   //int flag = 0;
   //wglQueryPbufferARB(hPBuffer, WGL_PBUFFER_LOST_ARB, &flag);
   //if (flag != 0)
   //{
   //  pbufferInit(pbufferWidth, pbufferHeight);
   //}
   //wglBindTexImageARB(hPBuffer, WGL_FRONT_LEFT_ARB);//WGL_FRONT_LEFT_ARB);

	// Bind the dynamic texture like you normally would, then bind the
	// p-buffer to it.
	//

    glBindTexture( GL_TEXTURE_2D, g_dynamicTextureID );

	if( !wglBindTexImageARB( g_pbuffer.hPBuffer, WGL_FRONT_LEFT_ARB ) )
	{
		MessageBox(NULL,"Could not bind p-buffer to render texture!",
			"ERROR",MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
	}
   return;
}
// ****************************************************************************
// pbufferRelease: release the pbuffer from the texture it was bound to.  once
// the pbuffer is released you will be able to render to it again by calling
// pbufferEnable()
// ****************************************************************************
void pbufferRelease (void)
{
   // make sure the pbuffer has been initialized
   if (!(pbufferInitialized))
   {
      return;
   }
    if( !wglReleaseTexImageARB( g_pbuffer.hPBuffer, WGL_FRONT_LEFT_ARB ) )
    {
        MessageBox(NULL,"Could not release p-buffer from render texture!",
            "ERROR",MB_OK|MB_ICONEXCLAMATION);
        exit(-1);
    }

   return;
}

// ****************************************************************************
// pbufferDestroy: free all resources associated with the pbuffer
// ****************************************************************************
void pbufferDestroy (void)
{
   // make sure the pbuffer has been initialized
   if (!(pbufferInitialized))
   {
      return;
   }
   pbufferInitialized = false;
	if( g_hRC != NULL )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( g_hRC );
		g_hRC = NULL;
	}

	if( g_hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_hDC );
		g_hDC = NULL;
	}

	//
	// Don't forget to clean up after our p-buffer...
	//

	if( g_pbuffer.hRC != NULL )
	{
		wglMakeCurrent( g_pbuffer.hDC, g_pbuffer.hRC );
		wglDeleteContext( g_pbuffer.hRC );
		wglReleasePbufferDCARB( g_pbuffer.hPBuffer, g_pbuffer.hDC );
		wglDestroyPbufferARB( g_pbuffer.hPBuffer );
		g_pbuffer.hRC = NULL;
	}

	if( g_pbuffer.hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_pbuffer.hDC );
		g_pbuffer.hDC = NULL;
	}

   return;
}
// ****************************************************************************
int pbufferTexID(){return g_dynamicTextureID;}
// ****************************************************************************
