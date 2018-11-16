#define JUCE_CORE_INCLUDE_COM_SMART_PTR 1
#define JUCE_CORE_INCLUDE_NATIVE_HEADERS 1

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_WINDOWS
#include "DemoAudioEngine.h"

struct DemoAudioEngineCallbacksIntf : IUnknown
{
    static constexpr UUID iid = { 0xda981f98, 0x3749, 0x4af0, {0x88, 0x13, 0xf7, 0xcd, 0xd5, 0x9e, 0x16, 0x90 } };
    virtual HRESULT STDMETHODCALLTYPE filePlaybackFinished() noexcept = 0;
};

struct DemoAudioEngineIntf : IUnknown
{
    static constexpr UUID iid = { 0xc1a01b40, 0xede0, 0x44ff, {0x93, 0xf8, 0xb5, 0x93, 0x96, 0xff, 0x67, 0x8d } };

    virtual HRESULT STDMETHODCALLTYPE play(const char* url) noexcept = 0;
    virtual HRESULT STDMETHODCALLTYPE stop() noexcept = 0;

    //==============================================================================
    virtual HRESULT STDMETHODCALLTYPE pause() noexcept = 0;
    virtual HRESULT STDMETHODCALLTYPE resume() noexcept = 0;

    //==============================================================================
    virtual HRESULT STDMETHODCALLTYPE setRoomSize(float roomSize) noexcept = 0;
    virtual HRESULT STDMETHODCALLTYPE setLowpassCutoff(float cutoff) noexcept = 0;

    //==============================================================================
    virtual HRESULT STDMETHODCALLTYPE setCallback(DemoAudioEngineCallbacksIntf* callback) noexcept = 0;

    //==============================================================================
    virtual HRESULT STDMETHODCALLTYPE addWaveformComponentToHWND(/*TODO Step 9: ensure parameters match idl*/) noexcept = 0;
    virtual HRESULT STDMETHODCALLTYPE removeWaveformComponentFromHWND() noexcept = 0;
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
        return ++refCnt;
    }

    ULONG STDMETHODCALLTYPE Release() noexcept override
    {
        auto value = --refCnt;

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
    HRESULT STDMETHODCALLTYPE stop() noexcept override                  { demoAudioEngine.stop();    return S_OK; }

    //==============================================================================
    HRESULT STDMETHODCALLTYPE pause() noexcept override                 { demoAudioEngine.pause();  return S_OK; }
    HRESULT STDMETHODCALLTYPE resume() noexcept override                { demoAudioEngine.resume(); return S_OK; }

    //==============================================================================
    HRESULT STDMETHODCALLTYPE setRoomSize(float roomSize) noexcept override    { demoAudioEngine.setRoomSize(roomSize);    return S_OK; }
    HRESULT STDMETHODCALLTYPE setLowpassCutoff(float cutoff) noexcept override { demoAudioEngine.setLowpassCutoff(cutoff); return S_OK; }

    HRESULT STDMETHODCALLTYPE setCallback(DemoAudioEngineCallbacksIntf*) noexcept override
    {
        return E_NOTIMPL;
    }

    //==============================================================================
    HRESULT STDMETHODCALLTYPE addWaveformComponentToHWND(/* TODO Step 10: ensure parameters match interface */) noexcept override
    {
        // TODO Step 11: call the underlying demoAudioEngine's addWaveformComponentToNativeParentView method
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE removeWaveformComponentFromHWND() noexcept override
    {
        // TODO Step 12: call the underlying demoAudioEngine's removeWaveformComponentFromNativeParentView method
        return E_NOTIMPL;
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