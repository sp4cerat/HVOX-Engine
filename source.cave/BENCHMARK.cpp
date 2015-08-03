//#################################################################//
#include "BENCHMARK.h"
//#################################################################//
BENCHMARK::BENCHMARK()
{}
//#################################################################//
BENCHMARK::~BENCHMARK()
{}
//#################################################################//
#ifdef _WIN32
//#################################################################//
void BENCHMARK::tstart(void)
{
        static int first = 1;

        if(first) {
            QueryPerformanceFrequency(&freq);
            first = 0;
        }
        QueryPerformanceCounter(&_tstart);
}
//#################################################################//
void BENCHMARK::tend(void)
{
        QueryPerformanceCounter(&_tend);
}
//#################################################################//
double BENCHMARK::tval(void)
{
        return ((double)_tend.QuadPart -
                    (double)_tstart.QuadPart)/((double)freq.QuadPart);
}
//#################################################################//
#else // LINUX?
//#################################################################//
void BENCHMARK::tstart(void)
{
        gettimeofday(&_tstart, &tz);
}
//#################################################################//
void BENCHMARK::tend(void)
{
        gettimeofday(&_tend,&tz);
}
//#################################################################//
double BENCHMARK::tval(void)
{
        double t1, t2;

        t1 =  (double)_tstart.tv_sec + (double)_tstart.tv_usec/(1000*1000);
        t2 =  (double)_tend.tv_sec + (double)_tend.tv_usec/(1000*1000);
        return t2-t1;
}
//#################################################################//
#endif
//#################################################################//
char*BENCHMARK::osinfo(void)
{
	int ver_underbars = 0;
	char *q;
#ifdef _WIN32
	static char verbuf[256];
#else
	static char verbuf[4*SYS_NMLN + 4];
#endif

#ifdef _WIN32
	OSVERSIONINFO VersionInfo;

	VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo);
	if(GetVersionEx(&VersionInfo))
	{
		if(strlen(VersionInfo.szCSDVersion) > 200)
			VersionInfo.szCSDVersion[100] = 0;
		sprintf(verbuf, "Windows %d.%d build%d PlatformId %d SP=\"%s\"",
		        VersionInfo.dwMajorVersion,
		        VersionInfo.dwMinorVersion,
		        VersionInfo.dwBuildNumber,
		        VersionInfo.dwPlatformId,
		        VersionInfo.szCSDVersion);
	}
	else
	{
		strcpy(verbuf, "WINDOWS UNKNOWN");
	}
#else
	struct utsname ubuf;

	if(uname(&ubuf))
	{
		strcpy(verbuf, "LINUX UNKNOWN");
	}
	else
	{
		sprintf(verbuf,"%s %s %s %s",
				ubuf.sysname,
				ubuf.release,
				ubuf.version,
				ubuf.machine);
	}
#endif
	// Substitute an underbar for white space. Makes output
	// easier to parse.
	if(ver_underbars)
	{
		for(q = verbuf; *q; q++)
			if(*q == ' '  || *q == '\t' || *q == '\n' ||
			        *q == '\r' || *q == '\b' || *q == '\f')
				*q = '_';
	}
	return verbuf;

}
//#################################################################//
