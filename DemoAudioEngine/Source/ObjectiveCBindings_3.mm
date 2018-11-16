#include "JuceHeader.h"

#if JUCE_IOS || JUCE_MAC

#import "ObjectiveCBindings.h"
#import "DemoAudioEngine.h"

@interface DemoAudioEngineBindings() {
    ScopedJuceInitialiser_GUI juceInit;
    DemoAudioEngine audioEngine;
}
@end

@implementation DemoAudioEngineBindings
- (void) play: (NSString*) urlString
{
    audioEngine.play(urlString.UTF8String);
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
    CGRect bounds = viewToAttachTo.bounds;
    audioEngine.setWaveformComponentBounds (0,
                                            0,
                                            bounds.size.width,
                                            bounds.size.height);
    audioEngine.addWaveformComponentToNativeParentView (viewToAttachTo);
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
