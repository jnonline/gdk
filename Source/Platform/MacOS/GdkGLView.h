/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */


#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

@interface GdkGLView : NSOpenGLView {
	CVDisplayLinkRef displayLink;
    NSTrackingArea* trackingArea;

}

@end
