package com.acme.demoaudioengine;

import android.content.Context;
import android.net.Uri;
import android.view.ViewGroup;

import java.lang.ref.WeakReference;
import java.util.ArrayList;

public class DemoAudioEngine
{
    static
    {
        System.loadLibrary("DemoAudioEngine");
    }

    public interface Listener
    {
        void filePlaybackFinished();
    }

    public DemoAudioEngine(Context context)
    {
        super();
        jniConstructAudioEngine(context);
    }

    @Override
    public void finalize() throws java.lang.Throwable
    {
        jniDestroyAudioEngine();
        super.finalize();
    }

    public void addListener(Listener listenerToAdd)
    {
        // add if not already there
        for (int i = listeners.size() - 1; i >= 0; --i)
        {
            Listener listener = listeners.get(i).get();

            if (listener == listenerToAdd)
                return;
        }

        listeners.add (new WeakReference<Listener>(listenerToAdd));
    }

    public void removeListener(Listener listenerToRemove)
    {
        for (int i = listeners.size() - 1; i >= 0; --i)
        {
            Listener listener = listeners.get(i).get();

            if (listener == listenerToRemove)
                listeners.remove (i);
        }
    }

    public void play(Uri uriToPlay)
    {
        jniPlay(uriToPlay.toString());
    }
    public void stop()
    {
        jniStop();
    }
    public void pause()
    {
        jniPause();
    }
    public void resume()
    {
        jniResume();
    }
    public void setRoomSize(float roomSize)
    {
        jniSetRoomSize(roomSize);
    }
    public void setLowpassCutoff(float lowpassCutoff)
    {
        jniSetLowpassCutoff(lowpassCutoff);
    }

    public void addWaveformComponentToNativeParentView (ViewGroup viewGroup)
    {
        jniAddWaveform(viewGroup);
    }

    public void removeWaveformComponentFromNativeParentView()
    {
        jniRemoveWaveform();
    }

    private void jniFilePlaybackFinished()
    {
        for (WeakReference<Listener> wearReference : listeners)
        {
            Listener listener = wearReference.get();

            if (listener != null)
                listener.filePlaybackFinished();
        }
    }

    private native void jniConstructAudioEngine(Context context);
    private native void jniDestroyAudioEngine();
    private native void jniPlay(String url);
    private native void jniStop();
    private native void jniPause();
    private native void jniResume();
    private native void jniSetRoomSize(float roomSize);
    private native void jniSetLowpassCutoff(float roomSize);
    private native void jniAddWaveform(ViewGroup viewGroup);
    private native void jniRemoveWaveform();

    private long cppCounterpartInstance = 0;
    private ArrayList<WeakReference<Listener>> listeners = new ArrayList<WeakReference<Listener>>();
};

