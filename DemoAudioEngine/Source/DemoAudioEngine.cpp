#include "../JuceLibraryCode/JuceHeader.h"
#include "DemoAudioEngine.h"

// shamelessly copied from the AudioPlaybackDemo
class DemoThumbnailComp  : public Component,
                           public ChangeListener,
                           public FileDragAndDropTarget,
                           public ChangeBroadcaster,
                           private ScrollBar::Listener,
                           private Timer
{
public:
    DemoThumbnailComp (AudioFormatManager& formatManager,
                       AudioTransportSource& source,
                       Slider& slider)
            : transportSource (source),
              zoomSlider (slider),
              thumbnail (512, formatManager, thumbnailCache)
    {
        thumbnail.addChangeListener (this);

        addAndMakeVisible (scrollbar);
        scrollbar.setRangeLimits (visibleRange);
        scrollbar.setAutoHide (false);
        scrollbar.addListener (this);

        currentPositionMarker.setFill (Colours::white.withAlpha (0.85f));
        addAndMakeVisible (currentPositionMarker);
    }

    ~DemoThumbnailComp()
    {
        scrollbar.removeListener (this);
        thumbnail.removeChangeListener (this);
    }

    void setURL (const URL& url)
    {
        InputSource* inputSource = nullptr;

#if ! JUCE_IOS
        if (url.isLocalFile())
        {
            inputSource = new FileInputSource (url.getLocalFile());
        }
        else
#endif
        {
            if (inputSource == nullptr)
                inputSource = new URLInputSource (url);
        }

        if (inputSource != nullptr)
        {
            thumbnail.setSource (inputSource);

            Range<double> newRange (0.0, thumbnail.getTotalLength());
            scrollbar.setRangeLimits (newRange);
            setRange (newRange);

            startTimerHz (40);
        }
    }

    URL getLastDroppedFile() const noexcept { return lastFileDropped; }

    void setZoomFactor (double amount)
    {
        if (thumbnail.getTotalLength() > 0)
        {
            auto newScale = jmax (0.001, thumbnail.getTotalLength() * (1.0 - jlimit (0.0, 0.99, amount)));
            auto timeAtCentre = xToTime (getWidth() / 2.0f);

            setRange ({ timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5 });
        }
    }

    void setRange (Range<double> newRange)
    {
        visibleRange = newRange;
        scrollbar.setCurrentRange (visibleRange);
        updateCursorPosition();
        repaint();
    }

    void setFollowsTransport (bool shouldFollow)
    {
        isFollowingTransport = shouldFollow;
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::darkgrey);
        g.setColour (Colours::lightblue);

        if (thumbnail.getTotalLength() > 0.0)
        {
            auto thumbArea = getLocalBounds();

            thumbArea.removeFromBottom (scrollbar.getHeight() + 4);
            thumbnail.drawChannels (g, thumbArea.reduced (2),
                                    visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
        }
        else
        {
            g.setFont (14.0f);
            g.drawFittedText ("(No audio file selected)", getLocalBounds(), Justification::centred, 2);
        }
    }

    void resized() override
    {
        scrollbar.setBounds (getLocalBounds().removeFromBottom (14).reduced (2));
    }

    void changeListenerCallback (ChangeBroadcaster*) override
    {
        // this method is called by the thumbnail when it has changed, so we should repaint it..
        repaint();
    }

    bool isInterestedInFileDrag (const StringArray& /*files*/) override
    {
        return true;
    }

    void filesDropped (const StringArray& files, int /*x*/, int /*y*/) override
    {
        lastFileDropped = URL (File (files[0]));
        sendChangeMessage();
    }

    void mouseDown (const MouseEvent& e) override
    {
        mouseDrag (e);
    }

    void mouseDrag (const MouseEvent& e) override
    {
        if (canMoveTransport())
            transportSource.setPosition (jmax (0.0, xToTime ((float) e.x)));
    }

    void mouseUp (const MouseEvent&) override
    {
        transportSource.start();
    }

    void mouseWheelMove (const MouseEvent&, const MouseWheelDetails& wheel) override
    {
        if (thumbnail.getTotalLength() > 0.0)
        {
            auto newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
            newStart = jlimit (0.0, jmax (0.0, thumbnail.getTotalLength() - (visibleRange.getLength())), newStart);

            if (canMoveTransport())
                setRange ({ newStart, newStart + visibleRange.getLength() });

            if (wheel.deltaY != 0.0f)
                zoomSlider.setValue (zoomSlider.getValue() - wheel.deltaY);

            repaint();
        }
    }


private:
    AudioTransportSource& transportSource;
    Slider& zoomSlider;
    ScrollBar scrollbar  { false };

    AudioThumbnailCache thumbnailCache  { 5 };
    AudioThumbnail thumbnail;
    Range<double> visibleRange;
    bool isFollowingTransport = false;
    URL lastFileDropped;

    DrawableRectangle currentPositionMarker;

    float timeToX (const double time) const
    {
        if (visibleRange.getLength() <= 0)
            return 0;

        return getWidth() * (float) ((time - visibleRange.getStart()) / visibleRange.getLength());
    }

    double xToTime (const float x) const
    {
        return (x / getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
    }

    bool canMoveTransport() const noexcept
    {
        return ! (isFollowingTransport && transportSource.isPlaying());
    }

    void scrollBarMoved (ScrollBar* scrollBarThatHasMoved, double newRangeStart) override
    {
        if (scrollBarThatHasMoved == &scrollbar)
            if (! (isFollowingTransport && transportSource.isPlaying()))
                setRange (visibleRange.movedToStartAt (newRangeStart));
    }

    void timerCallback() override
    {
        if (canMoveTransport())
            updateCursorPosition();
        else
            setRange (visibleRange.movedToStartAt (transportSource.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
    }

    void updateCursorPosition()
    {
        currentPositionMarker.setVisible (transportSource.isPlaying() || isMouseButtonDown());

        currentPositionMarker.setRectangle (Rectangle<float> (timeToX (transportSource.getCurrentPosition()) - 0.75f, 0,
                                                              1.5f, (float) (getHeight() - scrollbar.getHeight())));
    }
};

class WaveformComponent : public Component, public ChangeListener
{
public:
    WaveformComponent (AudioFormatManager& formatManager,
                       AudioTransportSource& transportSource)
    {
        const MessageManagerLock mm;
        setOpaque (true);

        addAndMakeVisible (zoomSlider);
        zoomSlider.setRange (0, 1, 0);
        zoomSlider.onValueChange = [this] { thumbnail->setZoomFactor (zoomSlider.getValue()); };
        zoomSlider.setSkewFactor (2);

        thumbnail.reset (new DemoThumbnailComp (formatManager, transportSource, zoomSlider));
        addAndMakeVisible (thumbnail.get());
        thumbnail->addChangeListener (this);
    }

    ~WaveformComponent()
    {
        thumbnail->removeChangeListener (this);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::darkgrey);
    }

    void resized() override
    {
        auto r = getLocalBounds();

        zoomSlider.setBounds (r.removeFromBottom(25));
        thumbnail->setBounds(r);
    }

    void setURL(const URL& url)
    {
        thumbnail->setURL (url);
    }

    void changeListenerCallback(ChangeBroadcaster*) override
    {

    }

private:
    Slider zoomSlider { Slider::LinearHorizontal, Slider::NoTextBox };
    std::unique_ptr<DemoThumbnailComp> thumbnail;
};

struct DemoAudioEngine::Pimpl   : private AudioIODeviceCallback, private TimeSliceThread, private AsyncUpdater
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
            auto sampleRate = reader->sampleRate;

            hasInformedListeners.store (false);

            audioFile.reset(new AudioFormatReaderSource(reader.release(), true));

            transportSource.setSource(audioFile.get(), 48000, this, sampleRate, 2);
            transportSource.start();

            if (waveformComponent != nullptr)
                waveformComponent->setURL (fileToLoad);

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
        dm.closeAudioDevice();
        dm.removeAudioCallback(this);
        transportSource.stop();
        transportSource.setSource(nullptr);

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

    void addListener(DemoAudioEngine::Listener* listener)
    {
        listeners.add (listener);
    }

    void removeListener(DemoAudioEngine::Listener* listener)
    {
        listeners.remove (listener);
    }

    void* addWaveformComponentToNativeParentView (void* nativeView)
    {
        if (waveformComponent == nullptr)
        {
            waveformComponent = std::make_unique<WaveformComponent> (fm, transportSource);
        }
        
        waveformComponent->setVisible (true);
        waveformComponent->addToDesktop (0, nativeView);

        return waveformComponent->getPeer()->getNativeHandle();
    }

    void removeWaveformComponentFromNativeParentView()
    {
        waveformComponent->removeFromDesktop();
    }

    void handleAsyncUpdate() override
    {
        stop();

        if (! hasInformedListeners.load())
            listeners.call([] (DemoAudioEngine::Listener& listener) { listener.filePlaybackFinished(); });

        hasInformedListeners.store (true);
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

        if (transportSource.hasStreamFinished() && ! hasInformedListeners.load())
            triggerAsyncUpdate();
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
    std::unique_ptr<WaveformComponent> waveformComponent;

    AudioSampleBuffer scratchBuffer;

    std::unique_ptr<PositionableAudioSource> audioFile;

    Reverb reverb;
    Reverb::Parameters reverbParams;

    dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> lowpass;
    std::unique_ptr<dsp::IIR::Coefficients<float>> lowpassCoefficientStorage;
    std::atomic<dsp::IIR::Coefficients<float>*> currentLowpassCoefficients;

    std::atomic<bool> hasInformedListeners = {false};
    ListenerList<DemoAudioEngine::Listener> listeners;
};

DemoAudioEngine::Listener::~Listener() {}
DemoAudioEngine::DemoAudioEngine() : pimpl (new Pimpl)   {}
DemoAudioEngine::~DemoAudioEngine()                      { delete pimpl; pimpl = nullptr; }
void DemoAudioEngine::play(const char* url)              { pimpl->play(url); }
void DemoAudioEngine::stop()                             { pimpl->stop(); }
void DemoAudioEngine::pause()                            { pimpl->pause(); }
void DemoAudioEngine::resume()                           { pimpl->resume(); }
void DemoAudioEngine::setRoomSize(float roomSize)        { pimpl->setRoomSize(roomSize); }
void DemoAudioEngine::setLowpassCutoff(float lpCutOff)   { pimpl->setLowpassCutoff(lpCutOff); }
void DemoAudioEngine::addListener(Listener* listener)    { pimpl->addListener (listener); }
void DemoAudioEngine::removeListener(Listener* listener) { pimpl->removeListener (listener); }
void* DemoAudioEngine::addWaveformComponentToNativeParentView (void* nativeView) { return pimpl->addWaveformComponentToNativeParentView (nativeView); }
void DemoAudioEngine::removeWaveformComponentFromNativeParentView()             { pimpl->removeWaveformComponentFromNativeParentView(); }
