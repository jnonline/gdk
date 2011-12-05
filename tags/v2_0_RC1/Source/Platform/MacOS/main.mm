/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */



#import <Cocoa/Cocoa.h>
#include "Gdk.h"

// ************************************************************
void ExitHandler(void)
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    // Shutdown the GDK System
    Gdk::Application::Platform_ShutdownGdk();
    
    [pool release];
}


// ************************************************************
int main(int argc, char *argv[])
{
    // Set the locale to unicode
    setlocale(LC_CTYPE, "UTF-8");
    
    // Create an Auto-release pool for Objective-C
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    // Init the GDK System
    if(Gdk::Application::Platform_InitGdk() == false)
        return 1;
    
    // Add an exit handler
    atexit(&ExitHandler);
    
    // Run the application
    int retVal = NSApplicationMain(argc,  (const char **) argv);
    
        // Release the pool
    [pool release];

    return retVal;
}
