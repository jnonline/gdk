//
//  GdkViewController.m
//

#import <QuartzCore/QuartzCore.h>

#import "GdkViewController.h"
#import "GdkGLView.h"

#import "Gdk.h"

@interface GdkViewController ()
@property (nonatomic, retain) EAGLContext *context;
@property (nonatomic, assign) CADisplayLink *displayLink;
@end

@implementation GdkViewController

@synthesize animating, context, displayLink;

// ****************************************************************************
- (void)awakeFromNib
{
    // Create a GLES2 Context
    EAGLContext *aContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    
    // Check that we got a valid context
    if (!aContext)
        NSLog(@"Failed to create ES context");
    else if (![EAGLContext setCurrentContext:aContext])
        NSLog(@"Failed to set ES context current");
    
    // Save a reference to the context
	self.context = aContext;
	[aContext release];
	
    // Setup the GL View with the context
    GdkGLView *glView = (GdkGLView*)self.view;
    [glView setContext:context];
    [glView ensureGLContextReady];
    
    // Start with animation/updating disabled
    animating = FALSE;
    animationFrameInterval = 1;
    self.displayLink = nil;
    
    // Turn on remote control events
    [[UIApplication sharedApplication] beginReceivingRemoteControlEvents];
    
    // GDK Graphics Setup
    // -----------------------------------
    
    // Init the GDK Graphics & Game
    Gdk::Application::Platform_InitGame();
}

// ****************************************************************************
- (void)dealloc
{
    // Shutdown the GDK Game
    Gdk::Application::Platform_ShutdownGame();
    
    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
    
    [context release];
    
    [super dealloc];
}

// ****************************************************************************
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)orientation
{
    // Support both landscape & both portrait orientations
    if(UIInterfaceOrientationIsLandscape(orientation) || UIInterfaceOrientationIsPortrait(orientation))
        return YES;
    
    return NO;
}

// ****************************************************************************
- (void)didReceiveMemoryWarning
{
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

// ****************************************************************************
- (void)viewWillAppear:(BOOL)animated
{
    // The view is becoming active
    Gdk::Application::Platform_OnActive();

    // Start the animation cycle
    [self startAnimation];
    
    [super viewWillAppear:animated];
}

// ****************************************************************************
- (void)viewWillDisappear:(BOOL)animated
{
    // The view is becoming in-active
    Gdk::Application::Platform_OnDeactive();
    
    // Stop the animation cycle 
    [self stopAnimation];
    
    [super viewWillDisappear:animated];
}

// ****************************************************************************
- (void)viewDidUnload
{
	[super viewDidUnload];

    // Tear down context.
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	self.context = nil;	
}

// ****************************************************************************
- (NSInteger)animationFrameInterval
{
    return animationFrameInterval;
}

// ****************************************************************************
- (void)setAnimationFrameInterval:(NSInteger)frameInterval
{
    /*
	 Frame interval defines how many display frames must pass between each time the display link fires.
	 The display link will only fire 30 times a second when the frame internal is two on a display that refreshes 60 times a second. The default frame interval setting of one will fire 60 times a second when the display refreshes at 60 times a second. A frame interval setting of less than one results in undefined behavior.
	 */
    if (frameInterval >= 1) 
    {
        animationFrameInterval = frameInterval;
        
        if (animating) 
        {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

// ****************************************************************************
- (void)startAnimation
{
    // Make sure we arent currently animating
    if (!animating) 
    {
        // Get a DisplayLink from the main UI screen
        CADisplayLink *aDisplayLink = [[UIScreen mainScreen] displayLinkWithTarget:self selector:@selector(drawFrame)];
        
        // Setup the display link
        [aDisplayLink setFrameInterval:animationFrameInterval];
        [aDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        self.displayLink = aDisplayLink;
        
        animating = TRUE;
    }
}

// ****************************************************************************
- (void)stopAnimation
{
    // Make sure we are currently animating
    if (animating) 
    {
        // Release our DisplayLink
        [self.displayLink invalidate];
        self.displayLink = nil;
        animating = FALSE;
    }
}

// ****************************************************************************
- (void)drawFrame
{
    // This is the Main Update loop method that is kicked by the DisplayLink (every screen refresh (60 hz))
    //
    
    // Setup GL Context
    [(GdkGLView *)self.view ensureGLContextReady];
    
    // Do a GDK update loop!
    Gdk::Application::Platform_MainLoop();
    
    // Present the frame buffer
    [(GdkGLView *)self.view presentFramebuffer];
}


@end
