#pragma once

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

typedef void (^Callback)();

@interface DemoAudioEngineBindings : NSObject

- (void) play: (NSString*) urlString;
- (void) stop;

- (void) pause;
- (void) resume;

- (void) setRoomSize: (float) roomSize;
- (void) setLowpassCutoff: (float) cutoff;

- (void) addWaveformComponentToView: (UIView*) viewToAttachTo;      // 2
- (void) removeWaveformComponentFromView;                            // 2

- (void) setPlaybackDidFinish: (Callback) callback;

@property (copy) Callback playbackFinishedCallback;

@end

#endif
