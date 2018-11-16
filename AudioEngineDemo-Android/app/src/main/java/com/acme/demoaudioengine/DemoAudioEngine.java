package com.acme.demoaudioengine;

import android.content.Context;
import android.net.Uri;
import android.view.ViewGroup;

import java.lang.ref.WeakReference;

public class DemoAudioEngine
{
    public interface Listener { void filePlaybackFinished(); }

    //==============================================================================
    static
    {
        System.loadLibrary("DemoAudioEngine");
    }

    //==============================================================================
    public DemoAudioEngine(Context context)
    {
        super();

        // TODO Step 1: call constructAudioEngine
    }

    @Override
    public void finalize() throws java.lang.Throwable
    {
        // TODO Step 1: call destroyAudioEngine

        setListener (null);
        super.finalize();
    }

    //==============================================================================
    public void play(String uriToPlay) {}
    public void stop() {}
    public void pause() {}
    public void resume() {}
    public void setRoomSize(float roomSize) {}
    public void setLowpassCutoff(float lowpassCutoff) {}
    public void addWaveformComponentToNativeParentView (/* TODO Step 4: missing parameter */) {}
    public void removeWaveformComponentFromNativeParentView() {}

    //==============================================================================
    public void setListener(Listener listenerToAdd)
    {
        if (listenerToAdd != null)
            listener = new WeakReference<Listener>(listenerToAdd);
        else
            listener = null;
    }

    private void invokeListener()
    {
        if (listener != null)
        {
            Listener l = listener.get();

            if (l != null)
                l.filePlaybackFinished ();
        }
    }

    //==============================================================================
    private WeakReference<Listener> listener = null;
};

