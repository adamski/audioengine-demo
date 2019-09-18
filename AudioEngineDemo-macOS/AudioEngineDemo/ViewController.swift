import Cocoa

class ViewController: NSViewController {
    
    @IBOutlet var statusLabel: NSTextField!
    
    @IBOutlet var waveformView: NSView!
    
    var fileURL: URL?
    
    var audioEngine: DemoAudioEngineBindings

    required init?(coder aDecoder: NSCoder) {
        
        audioEngine = DemoAudioEngineBindings()
        super.init(coder: aDecoder)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        
        audioEngine.setPlaybackDidFinish({() -> Void in
            self.didFinishPlaying()
        })
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    @IBAction func browseFile(sender: AnyObject) {
        
        let dialog = NSOpenPanel();
        
        dialog.title                   = "Choose an audio file";
        dialog.showsResizeIndicator    = true;
        dialog.showsHiddenFiles        = false;
        dialog.canChooseDirectories    = true;
        dialog.canCreateDirectories    = true;
        dialog.allowsMultipleSelection = false;
        dialog.allowedFileTypes        = ["wav", "aiff", "mp3", "ogg"];
        
        if (dialog.runModal() == NSApplication.ModalResponse.OK) {
            fileURL = dialog.url
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    func didFinishPlaying()
    {
        self.statusLabel.stringValue = "Stopped"
    }
    
    @IBAction func playButtonClicked(_ sender: Any) {
        self.statusLabel.stringValue = "Playing file..."
        
        audioEngine.play(fileURL?.absoluteString)
    }
    
    @IBAction func stopButtonClicked(_ sender: Any) {
        audioEngine.stop()
    }
    
    @IBAction func pauseButtonClicked(_ sender: Any) {
        audioEngine.pause()
    }
    
    @IBAction func resumeButtonClicked(_ sender: Any) {
        audioEngine.resume()
    }
    
    @IBAction func showButtonClicked(_ sender: Any) {
        audioEngine.addWaveformComponent(to: waveformView)
    }
    
    @IBAction func hideButtonClicked(_ sender: Any) {
        audioEngine.removeWaveformComponentFromView()
    }
    
    @IBAction func roomSizeSliderValueChanged(sender: NSSlider) {
        audioEngine.setRoomSize(sender.floatValue)
    }
    
    @IBAction func lowCutoffSliderValueChanged(sender: NSSlider) {
        audioEngine.setLowpassCutoff(sender.floatValue)
    }
}

