/******************************************************************************
 * filename: globals.h                                                        *
 *                                                                            *
 * author:   Christopher Oat                                                  *
 *           ATI Research, Inc.                                               *
 *           Application Research Group                                       *
 *                                                                            *
 * description: global variables used through out program                     *
 *                                                                            *
 ******************************************************************************
 $Header::                                                                    $
 ******************************************************************************
 *   (C) 2001 ATI Research, Inc.  All rights reserved.                        *
 ******************************************************************************/

#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "glati.h"
#include "wglati.h"

static char CopyrightString[] = "(C) 2001 ATI Research, Inc.";

// Prototypes needed.

void printError (char *str, ...);
void printGlError (void);

extern PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;

extern PFNGLACTIVETEXTUREARBPROC        glActiveTexture;
extern PFNGLMULTITEXCOORD2FARBPROC      glMultiTexCoord2f;
extern PFNGLTEXBUMPPARAMETERFVATIPROC   glTexBumpParameterfvATI;

extern PFNWGLMAKECONTEXTCURRENTARBPROC  wglMakeContextCurrentARB;

// pixel_format function pointers 
extern PFNWGLCHOOSEPIXELFORMATARBPROC	wglChoosePixelFormatARB;

// pbuffer function pointers 
extern PFNWGLCREATEPBUFFERARBPROC		wglCreatePbufferARB;
extern PFNWGLDESTROYPBUFFERARBPROC		wglDestroyPbufferARB;
extern PFNWGLGETPBUFFERDCARBPROC		wglGetPbufferDCARB;
extern PFNWGLRELEASEPBUFFERDCARBPROC	wglReleasePbufferDCARB;
extern PFNWGLQUERYPBUFFERARBPROC		wglQueryPbufferARB;

// render-to-texture function pointers
extern PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;
extern PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;
extern PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB;

// Viewing parameters
extern GLfloat viewMatrix[];
extern GLfloat fov;
extern GLfloat clipNear;
extern GLfloat clipFar;
extern GLfloat scaleFactor;
extern GLint screenWidth;
extern GLint screenHeight;

#endif