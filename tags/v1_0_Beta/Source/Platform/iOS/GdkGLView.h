/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#import <UIKit/UIKit.h>

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

@class EAGLContext;

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface GdkGLView : UIView <UIAccelerometerDelegate> {
@public
    // The pixel dimensions of the CAEAGLLayer.
    GLint framebufferWidth;
    GLint framebufferHeight;
    
    // The OpenGL ES names for the framebuffer and renderbuffers used to render to this view.
    GLuint frameBufferId;
    GLuint colorRenderBufferId;
    GLuint depthRenderBufferId;
}

@property (nonatomic, retain) EAGLContext *context;

- (void)ensureGLContextReady;
- (BOOL)presentFramebuffer;

- (void)accelerometer:(UIAccelerometer *)accelerometer
        didAccelerate:(UIAcceleration *)acceleration;

@end
