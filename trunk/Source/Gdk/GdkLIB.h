
#pragma once

// ============================================================================
// Platform-specific information goes in this header file.  
//
// GDK Defines the following macros for platform specific code:
//
// GDKPLATFORM_WINDOWS		: Microsoft Windows (2000, XP, Vista, 7, etc)
//
// GDKPLATFORM_APPLE		: Apple 
//   GDKPLATFORM_MACOSX		: Apple \ Macintosh OS X
//   GDKPLATFORM_IOS		: Apple \ iOS
//
// TODO:
// GDKPLATFORM_LINUX
//   GDKPLATFORM_NACL		: Google NaCl
//   GDKPLATFORM_ANDROID	: Android phones
//
// Wiiware, 3dsware, etc 
// ============================================================================

//----------------------------------------------------------------------------
// Platform:  Microsoft Windows
//----------------------------------------------------------------------------
#ifdef WIN32

	#define GDKPLATFORM_WINDOWS

	// Common Types
	// ----------------

	typedef __int8	Int8;
	typedef __int16	Int16;
	typedef __int32	Int32;
	typedef __int64 Int64;
	typedef unsigned __int8		UInt8;
	typedef unsigned __int16	UInt16;
	typedef unsigned __int32	UInt32;
	typedef unsigned __int64	UInt64;

	typedef UInt8	Byte;
	typedef UInt16	Word;
	typedef UInt32	DWord;

	// Windows headers
	#include <windows.h>

	// OpenGL headers
	#include <GL/glew.h>
	
	// Pthreads
    #include "pthread/pthread.h"

#endif


//----------------------------------------------------------------------------
// Platform:  Apple:   Mac OS X & iOS (iPhone / iPad / iPod)
//----------------------------------------------------------------------------

#ifdef __APPLE__

	#define GDKPLATFORM_APPLE

    // Includes for all Apple based platforms
	#include <wchar.h>
	#include <TargetConditionals.h>
	#include <stdint.h>
    #include <errno.h>
    #include <pthread.h>


	#if (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR) 

        //----------------------------------------------------------------------------
        // iOS platform
        //----------------------------------------------------------------------------

		#define GDKPLATFORM_IOS

        // Platform Includes
        #include <mactypes.h>

		// Include OpenGL ES
		#define GDK_OPENGL_ES
		#include <OpenGLES/ES2/gl.h>
		#include <OpenGLES/ES2/glext.h>

        // Common Types
        // ----------------

        typedef int8_t		Int8;
        typedef int16_t		Int16;
        typedef int32_t		Int32;
        typedef int64_t		Int64;
        //typedef uint8_t	UInt8;      // These are already defined in <mactypes.h>
        //typedef uint16_t	UInt16;
        //typedef uint32_t	UInt32;
        typedef uint64_t	UInt64;

    #else

        //----------------------------------------------------------------------------
        // MAC OS X platform
        //----------------------------------------------------------------------------

		#define GDKPLATFORM_MACOSX

        // Platform Includes
        #include <Carbon/Carbon.h>

        // Include OpenGL
        #include <OpenGL/gl.h>
        #include <OpenGL/OpenGL.h>

        // Common Types
        // ----------------

        typedef int8_t		Int8;
        typedef int16_t		Int16;
        typedef int32_t		Int32;
        typedef int64_t		Int64;
        //typedef uint8_t     UInt8;    
        //typedef uint16_t	UInt16;
        //typedef uint32_t	UInt32;
        //typedef uint64_t	UInt64;

	#endif

#endif

// ---------------------------------------------------------------------------
// Common Types

typedef UInt8   Byte;
typedef UInt16  Word;
typedef UInt32  DWord;

// ---------------------------------------------------------------------------
// Standard Includes

// C++ headers
#include <cassert>
#include <cctype>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>

// STL headers
#include <utility>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <deque>
#include <set>


using namespace std;


// ---------------------------------------
// GDK Macros

#define GDK_NOT_USED(x) ((void)(x))

