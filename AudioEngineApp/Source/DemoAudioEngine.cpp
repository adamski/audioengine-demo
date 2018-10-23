#include "../JuceLibraryCode/JuceHeader.h"
#include "DemoAudioEngine.h"

struct DemoAudioEngine::Pimpl   : private AudioIODeviceCallback, private TimeSliceThread
{
    Pimpl() : TimeSliceThread("AudioFileReaderThread"),
        lowpassCoefficientStorage (new dsp::IIR::Coefficients<float>(*dsp::IIR::Coefficients<float>::makeLowPass(48000.0, 24000.0f)))
    {
        reverbParams.roomSize = 0.0f;

        currentLowpassCoefficients.store(lowpassCoefficientStorage.get());

        fm.registerBasicFormats();
        startThread();
    }

    ~Pimpl()
    {
        stop();
    }

    void play(const char* urlOfFileToPlay)
    {
        stop();

        URL fileToLoad(urlOfFileToPlay);
        std::unique_ptr<AudioFormatReader> reader;

        if (fileToLoad.isLocalFile())
        {
            reader.reset(fm.createReaderFor(fileToLoad.getLocalFile()));
        }
        else
        {
            std::unique_ptr<InputStream> urlStream(fileToLoad.createInputStream(false));
            if (urlStream != nullptr)
                reader.reset(fm.createReaderFor(urlStream.release()));
        }

        if (reader != nullptr)
        {
            auto sampleRate = reader->  sampleRate;

            audioFile.reset(new AudioFormatReaderSource(reader.release(), true));
            audioFile->setLooping(true);

            transportSource.setSource(audioFile.get(), 48000, this, sampleRate, 2);
            transportSource.start();

            dm.addAudioCallback(this);
            dm.initialiseWithDefaultDevices(0, 2);
        }
        else
        {
            DBG("AudioPlayer: unable to load");
        }
    }

    void stop()
    {
        transportSource.stop();
        transportSource.setSource(nullptr);
        dm.closeAudioDevice();
        dm.removeAudioCallback(this);

        audioFile = nullptr;
    }

    void pause()
    {
        transportSource.stop();
    }

    void resume()
    {
        transportSource.start();
    }

    void setRoomSize(float roomSize)
    {
        reverbParams.roomSize = roomSize;
    }

    void setLowpassCutoff(float cutoff)
    {
        cutoff = jlimit(0.01f, 1.0f, cutoff);

        std::unique_ptr<dsp::IIR::Coefficients<float>> newCoeffs(new dsp::IIR::Coefficients<float>(*dsp::IIR::Coefficients<float>::makeLowPass(48000.0, 24000.0f *cutoff)));

        dsp::IIR::Coefficients<float>* currentCoefficients;

        do
        {
            currentCoefficients = lowpassCoefficientStorage.get();
        } while (! currentLowpassCoefficients.compare_exchange_weak(currentCoefficients, newCoeffs.get()));

        lowpassCoefficientStorage = std::move(newCoeffs);
    }

    void audioDeviceIOCallback(const float** /*inputs*/, int /*inChannels*/, float** outputs, int outChannels, int n) override
    {
        jassert(outChannels == 2);

        scratchBuffer.clear();

        //AudioSampleBuffer inputAudio(scratchBuffer.getArrayOfWritePointers(), 2, n);
        AudioSampleBuffer outputAudio(outputs, 2, n);

        transportSource.getNextAudioBlock(AudioSourceChannelInfo(outputAudio));
        reverb.setParameters(reverbParams);

        reverb.processStereo(outputs[0], outputs[1], n);

        auto* lowpassParams = currentLowpassCoefficients.exchange(nullptr);
        *lowpass.state = *lowpassParams;
        currentLowpassCoefficients.store(lowpassParams);

        dsp::AudioBlock<float> outBlock(outputAudio);
        dsp::ProcessContextReplacing<float> context(outBlock);
        lowpass.process(context);
    }

    void audioDeviceAboutToStart(AudioIODevice* device) override
    {
        auto maxNumSamples = device->getCurrentBufferSizeSamples();

        scratchBuffer.setSize(2, maxNumSamples);
        transportSource.prepareToPlay(maxNumSamples, device->getCurrentSampleRate());

        dsp::ProcessSpec spec{ device->getCurrentSampleRate(), static_cast<uint32> (maxNumSamples), 2 };
        lowpass.prepare(spec);
        reverb.setSampleRate(device->getCurrentSampleRate());

        lowpass.reset();
        reverb.reset();
    }

    void audioDeviceStopped() override
    {
        transportSource.releaseResources();
    }

    bool playbackInitialised = false;
    AudioFormatManager fm;
    AudioDeviceManager dm;
    AudioTransportSource transportSource;

    AudioSampleBuffer scratchBuffer;

    std::unique_ptr<PositionableAudioSource> audioFile;

    Reverb reverb;
    Reverb::Parameters reverbParams;

    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowpass;
    std::unique_ptr<dsp::IIR::Coefficients<float>> lowpassCoefficientStorage;
    std::atomic<dsp::IIR::Coefficients<float>*> currentLowpassCoefficients;

};

DemoAudioEngine::DemoAudioEngine() : pimpl (new Pimpl) {}
DemoAudioEngine::~DemoAudioEngine()                    { delete pimpl; pimpl = nullptr; }
void DemoAudioEngine::play(const char* url)            { pimpl->play(url); }
void DemoAudioEngine::stop()                           { pimpl->stop(); }
void DemoAudioEngine::pause()                          { pimpl->pause(); }
void DemoAudioEngine::resume()                         { pimpl->resume(); }
void DemoAudioEngine::setRoomSize(float roomSize)      { pimpl->setRoomSize(roomSize); }
void DemoAudioEngine::setLowpassCutoff(float lpCutOff) { pimpl->setLowpassCutoff(lpCutOff); }
