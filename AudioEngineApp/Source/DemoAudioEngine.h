#pragma once

class DemoAudioEngine
{
public:
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
    void setLowpassCutoff(float cutoff); // cutoff goes from 0 to 1 (1 being nyguist frequecncy)
    // TODO add more DSP methods

    //==============================================================================
    DemoAudioEngine(const DemoAudioEngine&) = delete;
    DemoAudioEngine(DemoAudioEngine&&) = delete;
    DemoAudioEngine& operator=(const DemoAudioEngine&) = delete;
    DemoAudioEngine& operator=(DemoAudioEngine&&) = delete;
private:
    struct Pimpl;
    Pimpl* pimpl;
};
