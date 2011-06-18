

// GDK includes
#include "Gdk.h"

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