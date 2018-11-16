#define JUCE_CORE_INCLUDE_COM_SMART_PTR 1
#define JUCE_CORE_INCLUDE_NATIVE_HEADERS 1

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_WINDOWS
#include "DemoAudioEngine.h"

//==============================================================================
class DemoAudioEngineBindings : private DemoAudioEngine::Listener
{
public:
    //==============================================================================
    DemoAudioEngineBindings()
    {
        demoAudioEngine.addListener (this);
    }

    ~DemoAudioEngineBindings()
    {
        demoAudioEngine.removeListener (this);
    }

private:
    void filePlaybackFinished() override
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