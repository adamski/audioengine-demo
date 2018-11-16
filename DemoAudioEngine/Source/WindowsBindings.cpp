#if _WIN32
#define JUCE_CORE_INCLUDE_COM_SMART_PTR 1
#define JUCE_CORE_INCLUDE_NATIVE_HEADERS 1

#include "../JuceLibraryCode/JuceHeader.h"

#include "DemoAudioEngine.h"

//==============================================================================
// TODO: Step 4: Copy & paste the interface idl code here, and change it so it's
// valid C++ code!

//==============================================================================
// TODO: Step 5: Inherit from DemoAudioEngineIntf
class DemoAudioEngineBindings
{
public:
    //==============================================================================
    DemoAudioEngineBindings()
    {
        demoAudioEngine.setPlaybackFinishedCallback ([this]() { filePlaybackFinished();  });
    }

    ~DemoAudioEngineBindings()
    {
        demoAudioEngine.setPlaybackFinishedCallback ({});
    }

    //==============================================================================
    // TODO: Step 6: Uncomment this
    /*HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** result) override
    {
        if (result == nullptr)
            return E_POINTER;

        if      (riid == DemoAudioEngineIntf::iid) *result = dynamic_cast<DemoAudioEngineIntf*> (this);
        else if (riid == IID_IUnknown)             *result = dynamic_cast<IUnknown*> (this);
        else                                                 return E_NOINTERFACE;

        AddRef();
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef() noexcept override
    {
        return ++refCnt;
    }

    ULONG STDMETHODCALLTYPE Release() noexcept override
    {
        auto value = --refCnt;

        if (value == 0)
            delete this;

        return value;
    }*/

    // TODO: Step 7: Implement the play method

private:
    void filePlaybackFinished()
    {
       // TODO
    }

    std::atomic<int> refCnt = { 1 };
    ScopedJuceInitialiser_GUI juceInitialiser;
    DemoAudioEngine demoAudioEngine;
};

// Entry point
extern "C" __declspec(dllexport) int* __stdcall CreateDemoAudioEngine()
{
    return nullptr;
}

#endif