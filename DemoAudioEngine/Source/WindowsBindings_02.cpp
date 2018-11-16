#define JUCE_CORE_INCLUDE_COM_SMART_PTR 1
#define JUCE_CORE_INCLUDE_NATIVE_HEADERS 1

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_WINDOWS
#include "DemoAudioEngine.h"

struct DemoAudioEngineIntf : IUnknown
{
    static constexpr UUID iid = { 0xc1a01b40, 0xede0, 0x44ff, {0x93, 0xf8, 0xb5, 0x93, 0x96, 0xff, 0x67, 0x8d } };

    virtual HRESULT STDMETHODCALLTYPE play(const char* url) noexcept = 0;
};

//==============================================================================
class DemoAudioEngineBindings : public DemoAudioEngineIntf, private DemoAudioEngine::Listener
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

    //==============================================================================
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** result) override
    {
        if (result == nullptr)
            return E_POINTER;

        if      (riid == DemoAudioEngineIntf::iid) *result = dynamic_cast<DemoAudioEngineIntf*> (this);
        else if (riid == IID_IUnknown)             *result = dynamic_cast<IUnknown*> (this);
        else                                               return E_NOINTERFACE;

        AddRef();
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef() noexcept override
    {
        return refCnt.fetch_add(1, std::memory_order_relaxed) + 1;
    }

    ULONG STDMETHODCALLTYPE Release() noexcept override
    {
        auto value = refCnt.fetch_sub(1, std::memory_order_relaxed) - 1;

        if (value == 0)
            delete this;

        return value;
    }

    //==============================================================================
    HRESULT STDMETHODCALLTYPE play(const char* url) noexcept override
    {
        demoAudioEngine.play (url);
        return S_OK;
    }

private:
    void filePlaybackFinished() override
    {
       // TODO
    }

    ScopedJuceInitialiser_GUI juceInitialiser;
    std::atomic<int> refCnt = { 1 };
    DemoAudioEngine demoAudioEngine;
};

extern "C" __declspec(dllexport) IUnknown* __stdcall CreateDemoAudioEngine()
{
    return new DemoAudioEngineBindings;
}

#endif