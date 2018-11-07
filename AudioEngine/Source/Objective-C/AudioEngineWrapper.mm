//
//  AudioEngineWrapper.m
//  AudioEngine - Static Library
//
//  Created by Adam Wilson on 07/11/2018.
//

#if __APPLE__

#import "../DemoAudioEngine.h"
#import "AudioEngineWrapper.h"

@interface AudioEngineWrapper() {
    DemoAudioEngine audioEngine;
}
@end

@implementation AudioEngineWrapper

- (void) play: (NSString*) urlString
{
    audioEngine.play (urlString.cString);
}

- (void) stop
{
    audioEngine.stop();
}

- (void) pause
{
    audioEngine.pause();
}

- (void) resume
{
    audioEngine.resume();
}

- (void) setRoomSize: (float) roomSize
{
    audioEngine.setRoomSize (roomSize);
}

- (void) setLowpassCutoff: (float) cutoff
{
    audioEngine.setLowpassCutoff (cutoff);
}


@end

#endif
