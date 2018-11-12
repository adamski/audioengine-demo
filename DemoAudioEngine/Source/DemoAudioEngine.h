#pragma once

class DemoAudioEngine
{
public:
    class Listener
    {
    public:
        ~Listener();
        virtual void filePlaybackFinished() = 0;
    };

    //==============================================================================
    DemoAudioEngine();
    ~DemoAudioEngine();

    //==============================================================================
    void play(const char* url);
    void stop();

    //==============================================================================
    void pause();
    void resume();

    //==============================================================================
    void setRoomSize(float roomSize);    // roomSize goes from 0 to 1
    void setLowpassCutoff(float cutoff); // cutoff goes from 0 to 1 (1 being nyquist frequency)
    // TODO add more DSP methods

    //==============================================================================
    void addListener(Listener*);
    void removeListener(Listener*);

    //==============================================================================
    // You can only have the waveform component be in single parent view

    // returns a point to the native handle of the component: only needed on windows
    void* addWaveformComponentToNativeParentView (void* nativeView);
    void removeWaveformComponentFromNativeParentView();

    //==============================================================================
    DemoAudioEngine(const DemoAudioEngine&) = delete;
    DemoAudioEngine(DemoAudioEngine&&) = delete;
    DemoAudioEngine& operator=(const DemoAudioEngine&) = delete;
    DemoAudioEngine& operator=(DemoAudioEngine&&) = delete;
private:
    struct Pimpl;
    Pimpl* pimpl;
};
