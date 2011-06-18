/*
 */

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

@interface GdkGLView : NSOpenGLView {
	CVDisplayLinkRef displayLink;
    NSTrackingArea* trackingArea;

}

@end
