///////////////////////////////////////////
#define STRICT
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#include <malloc.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
///////////////////////////////////////////
#include "Tools.h"
///////////////////////////////////////////
char  stdprint_buffer_out[1024*1024];
int   _fps_framecount=0;
int   _fps_last_sec  =0;
int   _fps           =0;
///////////////////////////////////////////
char* str(const char *format, ... )
{
 if (strlen(format)>1024*512)return "overflow";
 va_list args;
 va_start(args, format);
 vsprintf( stdprint_buffer_out,format,args );
 va_end(args);
 return(stdprint_buffer_out);
}
///////////////////////////////////////////
bool isInString(char *string, const char *search) 
{
	int pos=0;
	int maxpos=strlen(search)-1;
	int len=strlen(string);	
	char *other;
	for (int i=0; i<len; i++) 
	{
		if ((i==0) || ((i>1) && string[i-1]=='\n'))					// New Extension Begins Here!
		{				
			other=&string[i];			
			pos=0;													// Begin New Search
			while (string[i]!='\n') 
			{														// Search Whole Extension-String
				if (string[i]==search[pos]) pos++;					// Next Position
				if ((pos>maxpos) && string[i+1]=='\n') return true; // We Have A Winner!
				i++;
			}			
		}
	}	
	return false;													// Sorry, Not Found!
}
///////////////////////////////////////////
bool strCmp (char *a1,char *a2){
 int l1=0,l2=0;
 while(((byte)a1[l1]!=0)&&(l1<100))l1++;
 while(((byte)a2[l2]!=0)&&(l2<101))l2++;
 if(l1!=l2)return false;
 if(((byte)a1[0]==0)||((byte)a2[0]==0))return false;
 int a=0;
 while((a1[a]!=NULL)&&
       (a2[a]!=NULL)&&
	   (a1[a]!='$')&&
	   (a2[a]!='$')&&
	   (a<100)&&(a2[a]==a1[a]))
   a++;
 if(((a2[a]=='$')||(a2[a]==NULL))&&
	((a1[a]=='$')||(a1[a]==NULL))&&(a!=100))
  return true;
 return false;
}
///////////////////////////////////////////
int getFPS(){	 
 
 if	( timeGetTime()-_fps_last_sec > 1000 )
 {
   _fps				= _fps_framecount;
   _fps_framecount	= 0 ;
   _fps_last_sec	= timeGetTime();
 }
 else
   _fps_framecount++;


 return _fps;
}