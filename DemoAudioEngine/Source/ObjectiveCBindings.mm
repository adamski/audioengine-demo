//
//  AudioEngineWrapper.m
//  AudioEngine - Static Library
//
//  Created by Adam Wilson on 07/11/2018.
//

#import "DemoAudioEngineBindings.h"

#include "JuceHeader.h"

#if JUCE_IOS || JUCE_MAC

#import "../DemoAudioEngine.h"


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

- (void) setPlaybackFinishedCallback: (Callback) callbackBlock
{
    // Here we need to store the block/lambda in our Obj-C wrapper so we can call it later
    self.playbackFinishedCallback = callbackBlock;
    //self->audioEngine.playbackFinishedCallback (self.showHeadphoneDialogBlock);
}

@end

#endif
