//
//  main.m
//

#import <UIKit/UIKit.h>
#include "Gdk.h"

// ****************************************************************************
int main(int argc, char *argv[])
{
    // Setup ObjC object pool
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    // Init the GDK System
    if(Gdk::Application::Platform_InitGdk() == false)
        return 1;
    
    // Run the application
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    
    // Shutdown the GDK System
    Gdk::Application::Platform_ShutdownGdk();
    
    // Release the pool
    [pool release];
    
    return retVal;
}
