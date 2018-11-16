#if _WIN32
#define JUCE_CORE_INCLUDE_COM_SMART_PTR 1
#define JUCE_CORE_INCLUDE_NATIVE_HEADERS 1

#include "../JuceLibraryCode/JuceHeader.h"

#include "DemoAudioEngine.h"

//==============================================================================
class DemoAudioEngineBindings
{
public:
    //==============================================================================
    DemoAudioEngineBindings()
    {
        demoAudioEngine.setPlaybackFinishedCallback([this]() { filePlaybackFinished(); });
    }

    ~DemoAudioEngineBindings()
    {
        demoAudioEngine.setPlaybackFinishedCallback({});
    }

private:
    void filePlaybackFinished()
    {
       // TODO
    }

    ScopedJuceInitialiser_GUI juceInitialiser;
    DemoAudioEngine demoAudioEngine;
};

extern "C" __declspec(dllexport) IUnknown* __stdcall CreateDemoAudioEngine()
{
    // TODO!
    return nullptr;
}

#endif