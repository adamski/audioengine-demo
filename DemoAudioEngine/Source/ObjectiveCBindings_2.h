//
//  AudioEngineWrapper.h
//  AudioEngine
//
//  Created by Adam Wilson on 07/11/2018.
//

#pragma once

#ifdef __APPLE__

#import <Foundation/Foundation.h>

typedef void (^Callback)();         // 2

@interface DemoAudioEngineBindings : NSObject

- (void) play: (NSString*) urlString;
- (void) stop;

- (void) pause;
- (void) resume;

- (void) setRoomSize: (float) roomSize;
- (void) setLowpassCutoff: (float) cutoff;

- (void) setPlaybackDidFinish: (Callback) callback;       // 2

- (void) setWaveformComponentBounds: (CGRect) bounds;

@property (copy) Callback playbackFinishedCallback;       // 2

@end

#endif
