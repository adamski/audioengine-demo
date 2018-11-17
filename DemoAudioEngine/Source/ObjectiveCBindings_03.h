#pragma once

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

typedef void (^Callback)();

@interface DemoAudioEngineBindings : NSObject

- (void) play: (NSString*) urlString;
- (void) stop;

- (void) pause;
- (void) resume;

- (void) setRoomSize: (float) roomSize;
- (void) setLowpassCutoff: (float) cutoff;

- (void) addWaveformComponentToView: (NSView*) viewToAttachTo;      // 2
- (void) removeWaveformComponentFromView;						    // 2

- (void) setPlaybackDidFinish: (Callback) callback;                 // 3

@property (copy) Callback playbackFinishedCallback;

@end

#endif
