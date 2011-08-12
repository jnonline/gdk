/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

// ===========================================================
@interface GdkViewController : UIViewController {
@private
    EAGLContext *context;
    bool updateLoopEnabled;
}

@property (readonly, nonatomic, getter=isUpdateLoopEnabled) bool updateLoopEnabled;

- (void)queueNextUpdateLoop;
- (void)startUpdateLoop;
- (void)stopUpdateLoop;

@end
