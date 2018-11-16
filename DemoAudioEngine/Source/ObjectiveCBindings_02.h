#pragma once

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface DemoAudioEngineBindings : NSObject

- (void) play: (NSString*) urlString;
- (void) stop;

- (void) pause;
- (void) resume;

- (void) setRoomSize: (float) roomSize;
- (void) setLowpassCutoff: (float) cutoff;

- (void) addWaveformComponentToView: (ViewType) viewToAttachTo;      // 2
- (void) removeWaveformComponentFromView;

@end

#endif
