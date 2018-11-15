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

@end

#endif
