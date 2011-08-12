/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


#import "GdkAppDelegate.h"
#include "Gdk.h"

@implementation GdkAppDelegate

@synthesize window;

// ********************************************************************
- (void)applicationDidFinishLaunching:(NSNotification *)aNotification 
{
	// Insert code here to initialize your application 
}

// ********************************************************************
- (void)applicationDidBecomeActive:(NSNotification *)notification   
{
    Gdk::Application::Platform_OnActive();
}

// ********************************************************************
-(void)applicationWillResignActive:(NSNotification *)notification
{
    Gdk::Application::Platform_OnDeactive();
}

// ********************************************************************
-(void)applicationDidUnhide:(NSNotification *)notification
{
    Gdk::Application::Platform_OnResume();
}

// ********************************************************************
-(void)applicationWillHide:(NSNotification *)notification   
{
    Gdk::Application::Platform_OnSuspend();
}

// ********************************************************************
-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

@end
