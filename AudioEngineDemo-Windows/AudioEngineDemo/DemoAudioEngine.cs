using System;
using System.Runtime.InteropServices;

namespace DemoAudioEngine
{
    [Guid("DA981F98-3749-4AF0-8813-F7CDD59E1690")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface DemoAudioEngineCallbacksIntf
    {
        void filePlaybackFinished();
    }

    [Guid("C1A01B40-EDE0-44FF-93F8-B59396FF678D")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface DemoAudioEngineIntf
    {
        void play(string url);
        void stop();
        void pause();
        void resume();
        void setRoomSize(float roomSize);
        void setLowpassCutoff(float cutoff);
        void setCallback(DemoAudioEngineCallbacksIntf callbackObj);
        IntPtr addWaveformComponentToHWND(IntPtr host);
        void removeWaveformComponentFromHWND();
    }
}