import Cocoa

class ViewController: NSViewController {
    
    @IBOutlet var statusLabel: NSTextField!
    
    @IBOutlet var waveformView: NSView!
    
    var fileURL: URL?

    required init?(coder aDecoder: NSCoder) {
        
        super.init(coder: aDecoder)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.

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
        
    }
    
    @IBAction func stopButtonClicked(_ sender: Any) {

    }
    
    @IBAction func pauseButtonClicked(_ sender: Any) {

    }
    
    @IBAction func resumeButtonClicked(_ sender: Any) {

    }
    
    @IBAction func showButtonClicked(_ sender: Any) {

    }
    
    @IBAction func hideButtonClicked(_ sender: Any) {

    }
    
    @IBAction func roomSizeSliderValueChanged(sender: NSSlider) {

    }
    
    @IBAction func lowCutoffSliderValueChanged(sender: NSSlider) {

    }
}

