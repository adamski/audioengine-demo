package com.acme.androidaudioenginedemo;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.SeekBar;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements DemoAudioEngine.Listener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        demoAudioEngine = new DemoAudioEngine(this);
        demoAudioEngine.setListener(this);

        SeekBar roomSizeSlider = (SeekBar) findViewById(R.id.roomSizeSlider);
        SeekBar lowpassSlider  = (SeekBar) findViewById(R.id.lowpassSlider);

        demoAudioEngine.setRoomSize(((float) roomSizeSlider.getProgress())/((float) roomSizeSlider.getMax()));
        demoAudioEngine.setLowpassCutoff(((float) lowpassSlider.getProgress())/((float) lowpassSlider.getMax()));

        roomSizeSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                demoAudioEngine.setRoomSize(((float) i)/((float) seekBar.getMax()));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        lowpassSlider.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                demoAudioEngine.setLowpassCutoff(((float) i)/((float) seekBar.getMax()));
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });
    }

    @Override
    protected void onDestroy()
    {
        demoAudioEngine.setListener (null);
        demoAudioEngine = null;
        super.onDestroy();
    }

    public void chooseFileButtonClicked(View sender)
    {
        if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED)
        {
            Intent intent_upload = new Intent();
            intent_upload.setType("audio/*");
            String[] mimetypes = {"audio/mpeg", "audio/ogg", "audio/aac", "audio/x-matroska", "audio/mp4", "audio/flac"};
            intent_upload.putExtra(Intent.EXTRA_MIME_TYPES, mimetypes);
            intent_upload.setAction(Intent.ACTION_GET_CONTENT);
            startActivityForResult(intent_upload, 1);
        }
        else
        {
            requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 128);
        }
    }

    public void pauseButtonClicked(View sender)
    {
        TextView playbackStatus = (TextView) findViewById(R.id.playbackStatus);
        playbackStatus.setText("Playback is paused!");

        demoAudioEngine.pause();
    }

    public void resumeButtonClicked(View sender)
    {
        TextView playbackStatus = (TextView) findViewById(R.id.playbackStatus);
        playbackStatus.setText("File is playing...");

        demoAudioEngine.resume();
    }

    public void stopButtonClicked(View sender)
    {
        TextView playbackStatus = (TextView) findViewById(R.id.playbackStatus);
        playbackStatus.setText("Playback is stopped!");

        demoAudioEngine.stop();
    }

    public void showButtonClicked(View sender)
    {
        ViewGroup waveformViewGroup = (ViewGroup) findViewById (R.id.waveFormViewGroup);
        demoAudioEngine.addWaveformComponentToNativeParentView (waveformViewGroup);
    }

    public void hideButtonClicked(View sender)
    {
        demoAudioEngine.removeWaveformComponentFromNativeParentView ();
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults)
    {
        if (requestCode == 128)
        {
            if (grantResults.length > 0
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                chooseFileButtonClicked(null);
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode,int resultCode,Intent data){
        if(requestCode == 1){
            if(resultCode == RESULT_OK) {
                TextView playbackStatus = (TextView) findViewById(R.id.playbackStatus);
                playbackStatus.setText("File is playing...");

                demoAudioEngine.play(data.getData().toString());
            }
        }

        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void filePlaybackFinished()
    {
        TextView playbackStatus = (TextView) findViewById(R.id.playbackStatus);
        playbackStatus.setText("Playback is stopped!");
    }

    private DemoAudioEngine demoAudioEngine = null;
}
