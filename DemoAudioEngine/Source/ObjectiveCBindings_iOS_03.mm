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


- (void) addWaveformComponentToView: (UIView*) viewToAttachTo            // 2
{
    CGSize size = viewToAttachTo.bounds.size;
    audioEngine.setWaveformComponentSize(size.width, size.height);
    UIView* waveformView = (UIView*) audioEngine.addWaveformComponentToNativeParentView (viewToAttachTo);
}

- (void) removeWaveformComponentFromView                                // 2
{
    audioEngine.removeWaveformComponentFromNativeParentView();
}

- (void) setPlaybackDidFinish: (Callback) callback      // 3
{
    // Here we need to store the block/lambda in our Obj-C wrapper otherwise it will get deleted
    self.playbackFinishedCallback = callback;
    audioEngine.setPlaybackFinishedCallback(callback);
}


@end

#endif
