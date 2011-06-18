/*
 */


#import <Cocoa/Cocoa.h>
#include "Gdk.h"

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
    
    // Run the application
    int retVal = NSApplicationMain(argc,  (const char **) argv);
    
    // Shutdown the GDK System
    Gdk::Application::Platform_ShutdownGdk();
    
    // Release the pool
    [pool release];

    return retVal;
}
