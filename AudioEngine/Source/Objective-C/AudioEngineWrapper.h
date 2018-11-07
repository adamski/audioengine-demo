//
//  AudioEngineWrapper.h
//  AudioEngine
//
//  Created by Adam Wilson on 07/11/2018.
//

#pragma once

#import <Foundation/Foundation.h>

#ifdef __APPLE__

@interface AudioEngineWrapper : NSObject

- (void) play: (NSString*) urlString;
- (void) stop;

- (void) pause;
- (void) resume;

- (void) setRoomSize: (float) roomSize;
- (void) setLowpassCutoff: (float) cutoff;

@end

#endif
