//
//  GdkGLView.m
//

#import <QuartzCore/QuartzCore.h>

#import "GdkGLView.h"
#include "Gdk.h"

@interface GdkGLView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation GdkGLView

@synthesize context;

// ****************************************************************************
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

// ****************************************************************************
//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    // Init from the Nib/Xib
    self = [super initWithCoder:coder];
	if (self) 
    {
        // Get the GLLayer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        // Setup the GLLayer
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
        
        // Turn on multi-touch support
        self.multipleTouchEnabled = YES;
        
        // Init the buffer IDs to 0
        frameBufferId = 0;
        colorRenderBufferId = 0;
        depthRenderBufferId = 0;
    }
    
    return self;
}

// ****************************************************************************
-(BOOL) canBecomeFirstResponder
{
    return YES;
}

// ****************************************************************************
-(void) didMoveToWindow
{
    // Make this view the first responder
    [self becomeFirstResponder];
    
    // Make this view the delegate for the accelerometer data
    UIAccelerometer *accelerometer = [UIAccelerometer sharedAccelerometer];
    accelerometer.delegate = self;
    accelerometer.updateInterval = 0.016;
    
    // Enable orientation change events & register for them
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged:)
                                                 name:UIDeviceOrientationDidChangeNotification object:nil];

}

// ****************************************************************************
- (void)orientationChanged:(NSNotification *)notification
{
    // Tell the GDK about the orientation change
    switch([UIDevice currentDevice].orientation)
    {
        case UIDeviceOrientationPortrait:
            Gdk::Device::Platform_SetDeviceOrientation(Gdk::DeviceOrientation::Portrait);
        case UIDeviceOrientationPortraitUpsideDown:
            Gdk::Device::Platform_SetDeviceOrientation(Gdk::DeviceOrientation::PortraitUpsideDown);
        case UIDeviceOrientationLandscapeLeft:
            Gdk::Device::Platform_SetDeviceOrientation(Gdk::DeviceOrientation::LandscapeLeft);
        case UIDeviceOrientationLandscapeRight:
            Gdk::Device::Platform_SetDeviceOrientation(Gdk::DeviceOrientation::LandscapeRight);
    }
           
}

// ****************************************************************************
- (void)dealloc
{
    [self deleteFramebuffer];    
    [context release];
    
    [super dealloc];
}

// ****************************************************************************
- (void)setContext:(EAGLContext *)newContext
{
    // Is this a different context than the one we are managing?
    if (context != newContext) 
    {
        // Delete the current GL setup
        [self deleteFramebuffer];
        
        // Use the new context
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}

// ****************************************************************************
- (void)createFramebuffer
{
    if (context && !frameBufferId) 
    {
        [EAGLContext setCurrentContext:context];
        
        // Create framebuffer object.
        glGenFramebuffers(1, &frameBufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
        
        // Create color render buffer
        glGenRenderbuffers(1, &colorRenderBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBufferId);
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
        
        // Create a depth render buffer
        glGenRenderbuffers(1, &depthRenderBufferId);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferId);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, framebufferWidth, framebufferHeight);
    
        // Attach the render buffers to the frame buffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBufferId);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferId);
        
        // Verify the framebuffer & render buffers were created
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        
        // Update the GDK with the updated OS buffers
        Gdk::Graphics::Platform_SetOSFrameBuffers(frameBufferId, colorRenderBufferId, depthRenderBufferId, 0);
        
        // Tell the GDK about the new screen size
        Gdk::Application::Platform_OnResize(framebufferWidth, framebufferHeight);
    }
}

// ****************************************************************************
- (void)deleteFramebuffer
{
    if (context) 
    {
        // Make our GL context active
        [EAGLContext setCurrentContext:context];
        
        // If we have a frame buffer, delete it
        if (frameBufferId) 
        {
            glDeleteFramebuffers(1, &frameBufferId);
            frameBufferId = 0;
        }
        
        // If we have a color render buffer, delete it
        if (colorRenderBufferId) 
        {
            glDeleteRenderbuffers(1, &colorRenderBufferId);
            colorRenderBufferId = 0;
        }
        
        // If we have a depth render buffer, delete it
        if (depthRenderBufferId) 
        {
            glDeleteRenderbuffers(1, &depthRenderBufferId);
            depthRenderBufferId = 0;
        }
    }
}

// ****************************************************************************
- (void)ensureGLContextReady
{
    if (context) 
    {
        // Make our GL Context active
        [EAGLContext setCurrentContext:context];
        
        // Create our GL Buffers if we need to
        if (frameBufferId == 0)
            [self createFramebuffer];
    }
}

// ****************************************************************************
- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (context) 
    {
        [EAGLContext setCurrentContext:context];
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBufferId);
        
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

// ****************************************************************************
- (void)layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next ensureGLContextReady method call.
    [self deleteFramebuffer];
}

// ===========================================================================================================
// == Touch Input Handling
// ===========================================================================================================

// ****************************************************************************
-(void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    float scaleFactor = [self contentScaleFactor];
    
    // Enumerate through the touches
    for(UITouch* touch in touches)
    {
        // Get the Touch ID & Position
        int touchId = (int) touch;
        Gdk::Vector2 touchPosition((float)[touch locationInView:self].x, (float)[touch locationInView:self].y);
        
        // Scale the touch position for any UI scaling
        touchPosition *= scaleFactor;
        
        // Tell the GDK touch system about this new touch
        Gdk::TouchInput::Platform_ProcessTouchUpdate(touchId, touchPosition, Gdk::TouchState::Began); 
    }
}

// ****************************************************************************
-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    float scaleFactor = [self contentScaleFactor];
    
    // Enumerate through the touches
    for(UITouch* touch in touches)
    {
        // Get the Touch ID & Position
        int touchId = (int) touch;
        Gdk::Vector2 touchPosition((float)[touch locationInView:self].x, (float)[touch locationInView:self].y);
        
        // Scale the touch position for any UI scaling
        touchPosition *= scaleFactor;
        
        // Tell the GDK touch system about this new touch
        Gdk::TouchInput::Platform_ProcessTouchUpdate(touchId, touchPosition, Gdk::TouchState::Moved); 
    }
}

// ****************************************************************************
-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    float scaleFactor = [self contentScaleFactor];
    
    // Enumerate through the touches
    for(UITouch* touch in touches)
    {
        // Get the Touch ID & Position
        int touchId = (int) touch;
        Gdk::Vector2 touchPosition((float)[touch locationInView:self].x, (float)[touch locationInView:self].y);
        
        // Scale the touch position for any UI scaling
        touchPosition *= scaleFactor;
        
        // Tell the GDK touch system about this new touch
        Gdk::TouchInput::Platform_ProcessTouchUpdate(touchId, touchPosition, Gdk::TouchState::Ended); 
    }
}

// ****************************************************************************
-(void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    float scaleFactor = [self contentScaleFactor];
    
    // Enumerate through the touches
    for(UITouch* touch in touches)
    {
        // Get the Touch ID & Position
        int touchId = (int) touch;
        Gdk::Vector2 touchPosition((float)[touch locationInView:self].x, (float)[touch locationInView:self].y);
        
        // Scale the touch position for any UI scaling
        touchPosition *= scaleFactor;
        
        // Tell the GDK touch system about this new touch
        Gdk::TouchInput::Platform_ProcessTouchUpdate(touchId, touchPosition, Gdk::TouchState::Ended); 
    }
}

// ===========================================================================================================
// == Motion Input Processing
// ===========================================================================================================

// ****************************************************************************
- (void)motionBegan:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
    // What type of motion event is this?
    if(motion == UIEventSubtypeMotionShake)
    {
        // Tell the GDK the device began shaking
        Gdk::MotionInput::Platform_ShakingBegan();
    }
}

// ****************************************************************************
- (void)motionEnded:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
    // What type of motion event is this?
    if(motion == UIEventSubtypeMotionShake)
    {
        // Tell the GDK the device stopped shaking
        Gdk::MotionInput::Platform_ShakingEnded();
    }
}

// ****************************************************************************
- (void)motionCancelled:(UIEventSubtype)motion withEvent:(UIEvent *)event
{
    // What type of motion event is this?
    if(motion == UIEventSubtypeMotionShake)
    {
        // Tell the GDK the device stopped shaking
        Gdk::MotionInput::Platform_ShakingEnded();
    }
}

// ===========================================================================================================
// == Media Control Input Processing
// ===========================================================================================================

// ****************************************************************************
- (void)remoteControlReceivedWithEvent:(UIEvent *)event
{
    // Get the event sub-type
    switch(event.subtype)
    {
        case UIEventSubtypeRemoteControlPlay:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::Play); 
            break;
        case UIEventSubtypeRemoteControlPause:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::Pause); 
            break;
        case UIEventSubtypeRemoteControlStop:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::Stop); 
            break;
        case UIEventSubtypeRemoteControlTogglePlayPause:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::TogglePlayPause); 
            break;
        case UIEventSubtypeRemoteControlNextTrack:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::NextTrack); 
            break;
        case UIEventSubtypeRemoteControlPreviousTrack:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::PreviousTrack); 
            break;
        case UIEventSubtypeRemoteControlBeginSeekingForward:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::BeginSeekForward); 
            break;
        case UIEventSubtypeRemoteControlEndSeekingForward:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::EndSeekForward); 
            break;
        case UIEventSubtypeRemoteControlBeginSeekingBackward:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::BeginSeekBackward); 
            break;
        case UIEventSubtypeRemoteControlEndSeekingBackward:   
            Gdk::Device::Platform_ProcessMediaControlAction(Gdk::MediaControlActions::EndSeekBackward); 
            break;
    }
}

// ===========================================================================================================
// == Accelerometer Input Processing
// ===========================================================================================================

// ****************************************************************************
- (void)accelerometer:(UIAccelerometer *)accelerometer
        didAccelerate:(UIAcceleration *)acceleration
{
    // Update the GDK's accelerometer data
    Gdk::Vector3 accel(acceleration.x, acceleration.y, acceleration.z);
    Gdk::MotionInput::Platform_UpdateAccelerometer(accel);
}


@end
