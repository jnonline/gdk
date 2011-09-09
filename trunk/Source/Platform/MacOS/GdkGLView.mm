/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


#import "GdkGLView.h"
#import "Gdk.h"

extern bool _Gdk_Mac_IsMouseVisible;
GdkGLView* _Gdk_Mac_MainGLView;

// --------------------------------------
@interface GdkGLView (PrivateMethods)
- (void) drawView;
- (void) processMouseMove: (NSEvent*) theEvent;
- (void) closeMyWindow;

@end

@implementation GdkGLView

// ********************************************************************
- (CVReturn) getFrameForTime:(const CVTimeStamp*)outputTime
{
	// There is no autorelease pool when this method is called 
	// because it will be called from a background thread
	// It's important to create one or you will leak objects
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	// Make the GL Context the current context
	[[self openGLContext] makeCurrentContext];
    
	// We draw on a secondary thread through the display link
	// When resizing the view, -reshape is called automatically on the main thread
	// Add a mutex around to avoid the threads accessing the context simultaneously	when resizing
    CGLContextObj cglContext = (CGLContextObj) [[self openGLContext] CGLContextObj];
	CGLLockContext(cglContext);
	
    // Do a GDK update loop!
    Gdk::Application::Platform_MainLoop();
    
    // Present the GL Rendering
	CGLFlushDrawable(cglContext);
    
    // Unlock the mutex
	CGLUnlockContext(cglContext);
    
    // Is there an exit request?
    if(Gdk::Application::IsExitRequest())
    {
        [self performSelectorOnMainThread:@selector(closeMyWindow) withObject:nil waitUntilDone:NO];
        
        // Stop the display link
        CVDisplayLinkStop(displayLink);
    }
	
	[pool release];
	return kCVReturnSuccess;
}

// ********************************************************************
- (void) closeMyWindow
{
    [[self window] close];
}

// ********************************************************************
// This is the renderer output callback function
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(GdkGLView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

// ********************************************************************
- (id) initWithFrame:(NSRect)frameRect
{
    // Setup the pixel format
	NSOpenGLPixelFormatAttribute attrs[] =
	{
		NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAAccelerated,
		NSOpenGLPFADepthSize, 24,
		0
	};
	
	NSOpenGLPixelFormat *pixFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
	
	if (!pixFormat)
	{
		NSLog(@"No OpenGL pixel format");
	}
	
    // Initialize the GdkGLView
    self = [super initWithFrame:frameRect pixelFormat:pixFormat];
	if (self)
	{
		// Make this openGL context current to the thread
        [[self openGLContext] makeCurrentContext];
        
        // Synchronize buffer swaps with vertical refresh rate
        GLint swapInt = 1;
        [[self openGLContext] setValues:&swapInt forParameter:NSOpenGLCPSwapInterval];

		// Create a display link capable of being used with all active displays
		CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
		
		// Set the renderer output callback function
		CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
		
		// Set the display link for the current renderer
        CGLContextObj cglContext = (CGLContextObj) [[self openGLContext] CGLContextObj];
		CGLPixelFormatObj cglPixelFormat = (CGLPixelFormatObj) [[self pixelFormat] CGLPixelFormatObj];
		CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
		
        // GDK Graphics Setup
        // -----------------------------------
        
        // Tell the GDK about the default OS frame buffers
        Gdk::Graphics::Platform_SetOSFrameBuffers(0, 0, 0, 0);
        
        // Init the GDK Graphics & Game
        Gdk::Application::Platform_InitGame();
        
        // -----------------------------------
        
		// Activate the display link
		CVDisplayLinkStart(displayLink);
        
        // Store a global reference to this view
        _Gdk_Mac_MainGLView = self;
	}
    
    [pixFormat release];
    
    // Done
	return self;
}

// ********************************************************************
-(void) awakeFromNib
{
    // The view and window have been created   (awoken from the nib)
    
    // Resize the window to the GDK settings
    NSSize windowSize;
    windowSize.width = Gdk::Application::GetWidth();
    windowSize.height = Gdk::Application::GetHeight();
    [[self window] setContentSize:windowSize];
    
    // Update the window title
    const wchar_t* title = Gdk::Application::GetTitle();
    int numBytes = wcslen(title) * sizeof(wchar_t);
    NSString* newTitle = [[NSString alloc] initWithBytes:title length:numBytes encoding:NSUTF32LittleEndianStringEncoding];
    [self window].title = newTitle;
    
    // Setup input tracking
    // --------------------
    
	// Setup the tracking options
    NSTrackingAreaOptions trackingOptions =
        NSTrackingEnabledDuringMouseDrag |
        NSTrackingMouseMoved |
        NSTrackingMouseEnteredAndExited |
        NSTrackingActiveAlways |
        NSTrackingInVisibleRect;
        
    // Create a tracking area
	trackingArea = [[NSTrackingArea alloc]
                    initWithRect: [self bounds] // in our case track the entire view
                    options: trackingOptions
                    owner: self
                    userInfo: nil];
	[self addTrackingArea: trackingArea];

}

// ********************************************************************
-(void) viewDidMoveToWindow
{
    // Make this view the first responder, so we get all events
    [[self window] makeFirstResponder:self];
    [[self window] setInitialFirstResponder:self];
    //[[self window] setAcceptsMouseMovedEvents:YES];
}

// ********************************************************************
- (void) reshape
{
	// We draw on a secondary thread through the display link
	// When resizing the view, -reshape is called automatically on the main thread
	// Add a mutex around to avoid the threads accessing the context simultaneously when resizing
    CGLContextObj cglContext = (CGLContextObj) [[self openGLContext] CGLContextObj];
	CGLLockContext(cglContext);
	
    // Get the new window bounds
	NSRect rect = [self bounds];
	
    // Tell the GDK about the resize
    Gdk::Application::Platform_OnResize(rect.size.width, rect.size.height);
    
    // Use the new window size to determine the simulation device orientation
    if(rect.size.width > rect.size.height)
        Gdk::Device::Platform_SetDeviceOrientation(Gdk::DeviceOrientation::LandscapeLeft);
    else
        Gdk::Device::Platform_SetDeviceOrientation(Gdk::DeviceOrientation::Portrait);
	
    // Unlock the mutex
	CGLUnlockContext(cglContext);
}

// ********************************************************************
- (void) shutdownGdk
{	
    // Only allow this to happen ONCE
    if(_Gdk_Mac_MainGLView != NULL)
    {
        _Gdk_Mac_MainGLView = NULL;
        
        // Release the display link
        CVDisplayLinkRelease(displayLink);
        
        // Shutdown the GDK Game
        Gdk::Application::Platform_ShutdownGame();
    }
}

// ********************************************************************
- (void) dealloc
{	
    // Shutdown the Gdk 
    [self shutdownGdk];
	
    // Release the tracking area
    if (trackingArea)
	{
		[self removeTrackingArea: trackingArea];
		[trackingArea release];
		trackingArea = nil;
	}    
    
	[super dealloc];
}

/// Mouse Input Handlers
/// ###################################################################################################


// ********************************************************************
-(void)mouseMoved:(NSEvent *)theEvent
{
    [self processMouseMove:theEvent];
}

// ********************************************************************
-(void)mouseDragged:(NSEvent *)theEvent
{
    [self processMouseMove:theEvent];
}

// ********************************************************************
-(void)rightMouseDragged:(NSEvent *)theEvent
{
    [self processMouseMove:theEvent];
}

// ********************************************************************
-(void)otherMouseDragged:(NSEvent *)theEvent
{
    [self processMouseMove:theEvent];
}

// ********************************************************************
- (void) processMouseMove: (NSEvent*) theEvent
{
    // Get the mouse position within the view
    NSPoint eventLocation = [theEvent locationInWindow];
    NSPoint localPoint = [self convertPoint:eventLocation fromView:nil];
    
    // Mac uses BottomLeft for screen 0,0.  So lets convert it to TopRight which the GDK uses
    NSRect viewBounds = [self bounds];
    localPoint.y = viewBounds.size.height - localPoint.y;
    
    // In case the GDK thinks the mouse is outside the app, correct it :D
    if(Gdk::Mouse::IsMouseOverApp() == false)
        Gdk::Mouse::Platform_ProcessMouseEnterApp();
    
    // Pass the mouse move to the GDK
    Gdk::Mouse::Platform_ProcessMouseMove(localPoint.x, localPoint.y);
}

// ********************************************************************
-(void)mouseEntered:(NSEvent *)theEvent
{
    // If the mouse supposed to be hidden, hide it
    if(_Gdk_Mac_IsMouseVisible == false)
        [NSCursor hide];
    
    Gdk::Mouse::Platform_ProcessMouseEnterApp();
}

// ********************************************************************
-(void)mouseExited:(NSEvent *)theEvent
{
    // If the mouse currently hidden, show it
    if(_Gdk_Mac_IsMouseVisible == false)
        [NSCursor unhide];
    
    Gdk::Mouse::Platform_ProcessMouseLeaveApp();
}

// ********************************************************************
-(void)scrollWheel:(NSEvent *)theEvent
{
    float deltaX = [theEvent deltaX];
    float deltaY = [theEvent deltaY];
    Gdk::Mouse::Platform_ProcessMouseWheelScroll(deltaX, deltaY);
    //NSLog(@"Mouse Scroll %f, %f", deltaX, deltaY);
}

// ********************************************************************
-(void)mouseDown:(NSEvent *)theEvent
{
    Gdk::Mouse::Platform_ProcessMouseButtonDown(Gdk::MouseButton::Left);
    
    //NSLog(@"Left Mouse Down");
}

// ********************************************************************
-(void)mouseUp:(NSEvent *)theEvent
{
    Gdk::Mouse::Platform_ProcessMouseButtonUp(Gdk::MouseButton::Left);
    
    //NSLog(@"Left Mouse Up");
}

// ********************************************************************
-(void)rightMouseDown:(NSEvent *)theEvent
{
    Gdk::Mouse::Platform_ProcessMouseButtonDown(Gdk::MouseButton::Right);
    
    //NSLog(@"Right Mouse Down");
}

// ********************************************************************
-(void)rightMouseUp:(NSEvent *)theEvent
{
    Gdk::Mouse::Platform_ProcessMouseButtonUp(Gdk::MouseButton::Right);
    
    //NSLog(@"Right Mouse Up");
}

// ********************************************************************
-(void)otherMouseDown:(NSEvent *)theEvent
{
    int buttonNumber = [theEvent buttonNumber];
    if(buttonNumber < Gdk::MouseButton::MAX_BUTTONS)
        Gdk::Mouse::Platform_ProcessMouseButtonDown((Gdk::MouseButton::Enum) buttonNumber);
    
    //NSLog(@"Other Mouse Down [%d]", buttonNumber);
}

// ********************************************************************
-(void)otherMouseUp:(NSEvent *)theEvent
{
    int buttonNumber = [theEvent buttonNumber];
    if(buttonNumber < Gdk::MouseButton::MAX_BUTTONS)
        Gdk::Mouse::Platform_ProcessMouseButtonUp((Gdk::MouseButton::Enum) buttonNumber);
    
    //NSLog(@"Other Mouse Up [%d]", buttonNumber);
}


/// Keyboard Input Handlers
/// ###################################################################################################

// ********************************************************************
-(void)keyUp:(NSEvent *)theEvent 
{
    // Handle the Key Down event
    unsigned short keyCode = [theEvent keyCode];
    Gdk::Keys::Enum key = Gdk::Keyboard::Platform_ConvertScanCodeToKey((UInt8)keyCode);
    Gdk::Keyboard::Platform_ProcessKeyUp(key);
}

// ********************************************************************
-(void)keyDown:(NSEvent *)theEvent
{  
    // Handle the Key Down event
    unsigned short keyCode = [theEvent keyCode];
    Gdk::Keys::Enum key = Gdk::Keyboard::Platform_ConvertScanCodeToKey((UInt8)keyCode);
    Gdk::Keyboard::Platform_ProcessKeyDown(key);
    
    // Handle any characters typed as a result of this key down
    NSString* chars = [theEvent characters];
    for(int i = 0; i < [chars length]; i++)
    {
        unichar ch = [chars characterAtIndex:i];
        Gdk::Keyboard::Platform_ProcessChar((wchar_t) ch);
    }
}

// ********************************************************************
-(void)flagsChanged:(NSEvent *)theEvent
{  
    // Get the GDK key associated with this keyCode
    unsigned short keyCode = [theEvent keyCode];
    Gdk::Keys::Enum key = Gdk::Keyboard::Platform_ConvertScanCodeToKey((UInt8)keyCode);
    
    // Is this key currently down?
    if(Gdk::Keyboard::IsKeyDown(key))
        Gdk::Keyboard::Platform_ProcessKeyUp(key);
    else
        Gdk::Keyboard::Platform_ProcessKeyDown(key);
}

/// Touch Input Handlers
/// ###################################################################################################

// ********************************************************************
//-(void)touchesBeganWithEvent:(NSEvent *)event
//{
    // TODO(P2) - Wire Up Touch handling for Mac OSX
//}




@end
