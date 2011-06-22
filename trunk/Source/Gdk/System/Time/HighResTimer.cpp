/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "HighResTimer.h"

using namespace Gdk;


// ===================================================================================
#ifdef GDKPLATFORM_WINDOWS

double HighResTimer::GetSeconds()
{
	static bool initialized = false;
	static LARGE_INTEGER perfFrequency;

	if(initialized == false)
	{
		// Initialize
		QueryPerformanceFrequency(&perfFrequency);
		initialized = true;
	}

	// Get the current time
	LARGE_INTEGER perfTime;
	QueryPerformanceCounter(&perfTime);

	double seconds = (double)perfTime.QuadPart / (double)perfFrequency.QuadPart;
	return seconds;
}
#endif

// ===================================================================================
#ifdef GDKPLATFORM_APPLE

#import <mach/mach_time.h>

double HighResTimer::GetSeconds()
{
	static bool initialized = false;
	static mach_timebase_info_data_t timebase;

	if(initialized == false)
	{
		// Initialize
		mach_timebase_info(&timebase);
		initialized = true;
	}

	// Get the current time
	uint64_t t = mach_absolute_time();
	double nanoSeconds = ((double)t) * ( (double) timebase.numer) / ((double) timebase.denom);
	return (nanoSeconds * 1e-9);
}

#endif

// ===================================================================================
#ifdef GDKPLATFORM_LINUX

double HighResTimer::GetSeconds()
{
	// TODO(Android)

	// use these methods...
	int clock_getres(clockid_t clock_id, struct timespec *res);
	int clock_gettime(clockid_t clock_id, struct timespec *tp);
}

#endif