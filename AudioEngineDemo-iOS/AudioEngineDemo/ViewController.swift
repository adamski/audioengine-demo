import UIKit

class ViewController:
        UIViewController,
        UIDocumentPickerDelegate,
        UINavigationControllerDelegate {
    
    @IBOutlet var playStopButton: UIButton!
    @IBOutlet var statusLabel: UILabel!
    @IBOutlet var roomSizeValueLabel: UILabel!
    @IBOutlet var lowCutoffValueLabel: UILabel!

    @IBOutlet var roomSizeSlider: UISlider!
    @IBOutlet var lowCutoffSlider: UISlider!

    var fileURL: URL?
    
    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        roomSizeValueLabel.text = "\(roomSizeSlider.value)"
        lowCutoffValueLabel.text = "\(lowCutoffSlider.value)"
    }

    public func documentPicker(_ controller: UIDocumentPickerViewController, didPickDocumentAt url: URL) {
        self.fileURL = url as URL
        print("import result : \(String(describing: fileURL))")
    }

    func documentPickerWasCancelled(_ controller: UIDocumentPickerViewController) {
        print("document picker was cancelled")
        dismiss(animated: true, completion: nil)
    }

    @IBAction func loadFileButtonClicked() {
        
        let documentPicker: UIDocumentPickerViewController = UIDocumentPickerViewController(documentTypes: ["public.audio"], in: UIDocumentPickerMode.open)
        
        documentPicker.delegate = self
        documentPicker.modalPresentationStyle = .formSheet
        documentPicker.title = "Choose an audio file"
        
        self.present(documentPicker, animated: true, completion: nil)
    }
    
    func didFinishPlaying()
    {
        self.playStopButton.setTitle("Play", for: UIControl.State.normal)
        self.statusLabel.text = "Stopped"
    }

    @IBAction func playButtonClicked() {
        self.playStopButton.setTitle("Stop", for: UIControl.State.normal)
        self.statusLabel.text = "Playing file..."
    }
    
    @IBAction func pauseButtonClicked() {
        
    }
    
    @IBAction func resumeButtonClicked() {
        
    }
    
    @IBAction func showButtonClicked() {
        
    }
    
    @IBAction func hideButtonClicked() {
        
    }
    
    @IBAction func roomSizeSliderValueChanged(sender: UISlider) {
        let roomSizeValue = sender.value
        print("Room Size slider changing to \(roomSizeValue)")
        
        DispatchQueue.main.async {
            self.roomSizeValueLabel.text = "\(roomSizeValue)"
        }
        
    }

    @IBAction func lowCutoffSliderValueChanged(sender: UISlider) {
        let lowCutoffValue = sender.value
        print("Cutoff slider changing to \(lowCutoffValue)")
        
        DispatchQueue.main.async {
            self.lowCutoffValueLabel.text = "\(lowCutoffValue)"
        }
        
    }
}

