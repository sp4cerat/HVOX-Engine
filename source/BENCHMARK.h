#ifndef _BENCHMARK_class
#define _BENCHMARK_class

#include <windows.h>
#include <time.h>

#include <stdio.h>
#include <string.h>

class BENCHMARK
{
public:

	BENCHMARK();
	~BENCHMARK();
	void tstart(void);
	void tend(void);
	double tval(void);
	char* osinfo(void);

private:

    LARGE_INTEGER _tstart, _tend;
    LARGE_INTEGER freq;

public:

};

#endif
