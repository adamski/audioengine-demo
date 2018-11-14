//
//  AudioEngineWrapper.m
//  AudioEngine - Static Library
//
//  Created by Adam Wilson on 07/11/2018.
//



#include "JuceHeader.h"

#if JUCE_IOS || JUCE_MAC

#import "ObjectiveCBindings.h"
#import "DemoAudioEngine.h"


@interface DemoAudioEngineBindings() {
    ScopedJuceInitialiser_GUI juceApp;  // This is needed for JUCE GUI
    DemoAudioEngine audioEngine;
}
@end

@implementation DemoAudioEngineBindings

- (void) play: (NSString*) urlString
{
    audioEngine.play (urlString.UTF8String); 
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

- (void) setPlaybackDidFinish: (Callback) callback      // 2
{
    // Here we need to store the block/lambda in our Obj-C wrapper otherwise it will get deleted
    self.playbackFinishedCallback = callback;
    audioEngine.setPlaybackFinishedCallback(callback);
}

@end

#endif
