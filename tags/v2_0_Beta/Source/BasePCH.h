/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


// This include has all the C/C++/STL/Platform headers used by the project/game
#include "GdkLIB.h"

// This will cause the GDK headers to be built into the PCH
#include "Gdk.h"

// ATTENTION:   Add other 3rd party libraries here if you want them to be built into the Pre-compiled header


// Apple platform PCHs
#ifdef __APPLE__

    #ifdef __OBJC__
    #import <Foundation/Foundation.h>
    #endif

    // iOS platform
    #if (defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE) || (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR) 

        #import <Availability.h>

        #ifdef __OBJC__
        #import <UIKit/UIKit.h>
        #endif
    
    // MAC
    #else

        #ifdef __OBJC__
        #import <Cocoa/Cocoa.h>
        #endif
        
    #endif

#endif