#pragma once

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>             // 2

typedef void (^Callback)();         // 3

@interface DemoAudioEngineBindings : NSObject

- (void) play: (NSString*) urlString;
- (void) stop;

- (void) pause;
- (void) resume;

- (void) setRoomSize: (float) roomSize;
- (void) setLowpassCutoff: (float) cutoff;

- (void) setPlaybackDidFinish: (Callback) callback;       // 3

- (void) setWaveformComponentBounds: (CGRect) bounds;               // 2
- (void) addWaveformComponentToView: (UIView*) viewToAttachTo;      // 2
- (void) removeWaveformComponentFromView: (UIView*) viewToAttachTo; // 2

@property (copy) Callback playbackFinishedCallback;       // 3

@end

#endif
