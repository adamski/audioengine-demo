#include "../JuceLibraryCode/JuceHeader.h"
#include "DemoAudioEngine.h"

//==============================================================================
class AudioEngineAppApplication  : public JUCEApplication
{
public:
    //==============================================================================
    AudioEngineAppApplication() {}

    const String getApplicationName() override           { return ProjectInfo::projectName; }
    const String getApplicationVersion() override        { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override           { return true; }
    void initialise (const String&) override             { mainWindow.reset (new MainWindow); }
    void shutdown() override                             { mainWindow = nullptr; }
    void systemRequestedQuit() override                  { quit(); }
    void anotherInstanceStarted (const String&) override {}

    //==============================================================================
    class MainComponent : public Component, private Button::Listener, private Slider::Listener
    {
    public:
        MainComponent()
        {
            chooseFileButton.addListener (this);
            roomSizeSlider.addListener (this);
            lowpassSlider.addListener (this);
            playPauseButton.addListener (this);

            roomSizeSlider.setRange(0.0, 1.0);
            lowpassSlider.setRange(0.0, 1.0);
            lowpassSlider.setValue(1.0, NotificationType::dontSendNotification);

            addAndMakeVisible (chooseFileButton);
            addAndMakeVisible (roomSizeLabel);
            addAndMakeVisible (roomSizeSlider);
            addAndMakeVisible (lowpassLabel);
            addAndMakeVisible (lowpassSlider);
            addAndMakeVisible (playPauseButton);

            setOpaque (true);
            setSize (800, 600);
        }

        void paint (Graphics& g) override
        {
            g.fillAll(Desktop::getInstance().getDefaultLookAndFeel()
                      .findColour (ResizableWindow::backgroundColourId));
        }

        void resized() override
        {
            auto r = getLocalBounds();
            auto rowHeight = r.getHeight() / 4;

            {
                auto row = r.removeFromTop(rowHeight);
                playPauseButton.setBounds(row.removeFromRight(rowHeight));
                chooseFileButton.setBounds(row);
            }

            {
                auto row = r.removeFromTop(rowHeight);
                roomSizeLabel.setBounds(row.removeFromLeft(100));
                roomSizeSlider.setBounds(row);
            }

            {
                auto row = r.removeFromTop(rowHeight);
                lowpassLabel.setBounds(row.removeFromLeft(100));
                lowpassSlider.setBounds(row);
            }
        }

    private:
        void buttonClicked(Button* btn) override
        {
            if (btn == &chooseFileButton)
            {
                if (fc == nullptr)
                {
                    fc.reset(new FileChooser({}, {}));

                    fc->launchAsync(FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles,
                                    [this](const FileChooser &)
                                    {
                                        auto &results = fc->getURLResults();
                                        if (results.size() > 0)
                                            demoAudioEngine.play(results.getReference(0).toString(true).toRawUTF8());

                                        fc = nullptr;
                                    });
                }
            }
            else if (btn == &playPauseButton)
            {
                if (isPaused)
                    demoAudioEngine.resume();
                else
                    demoAudioEngine.pause();

                isPaused = ! isPaused;
            }
        }

        void sliderValueChanged(Slider* slider) override
        {
            if (slider == &roomSizeSlider)
            {
                demoAudioEngine.setRoomSize(static_cast<float>(roomSizeSlider.getValue()));
            }
            else if (slider == &lowpassSlider)
            {
                demoAudioEngine.setLowpassCutoff(static_cast<float>(lowpassSlider.getValue()));
            }
        }

        DemoAudioEngine demoAudioEngine;
        TextButton chooseFileButton{ "Choose Audio File..." };
        TextButton playPauseButton {"Play/Pause"};

        Label roomSizeLabel{ "roomSizeLabel", "Room Size:" };
        Slider roomSizeSlider;

        Label lowpassLabel{ "lowpassLabel", "Low Cutoff:" };
        Slider lowpassSlider;

        bool isPaused = false;

        std::unique_ptr<FileChooser> fc;
    };

    //==============================================================================
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow()  : DocumentWindow (ProjectInfo::projectName,
                                        Desktop::getInstance().getDefaultLookAndFeel()
                                        .findColour (ResizableWindow::backgroundColourId),
                                        DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
START_JUCE_APPLICATION (AudioEngineAppApplication)
