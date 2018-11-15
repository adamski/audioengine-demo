//
//  AudioEngineWrapper.h
//  AudioEngine
//
//  Created by Adam Wilson on 07/11/2018.
//

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

- (void) setPlaybackDidFinish: (Callback) callback;

- (void) setWaveformComponentBounds: (CGRect) bounds;
- (void) addWaveformComponentToView: (UIView*) viewToAttachTo;

@property (copy) Callback playbackFinishedCallback;

@end

#endif
