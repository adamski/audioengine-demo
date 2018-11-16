#pragma once

#ifdef __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
 #import <UIKit/UIKit.h>
 typedef UIView* ViewType;
#elif TARGET_OS_MAC
 #import <AppKit/AppKit.h>
 typedef NSView* ViewType;
#endif

#import <Foundation/Foundation.h>

typedef void (^Callback)();

@interface DemoAudioEngineBindings : NSObject

- (void) play: (NSString*) urlString;
- (void) stop;

- (void) pause;
- (void) resume;

- (void) setRoomSize: (float) roomSize;
- (void) setLowpassCutoff: (float) cutoff;

- (void) setWaveformComponentBounds: (CGRect) bounds;               // 2
- (void) addWaveformComponentToView: (ViewType) viewToAttachTo;      // 2
- (void) removeWaveformComponentFromView;

- (void) setPlaybackDidFinish: (Callback) callback;  

@property (copy) Callback playbackFinishedCallback;

@end

#endif
