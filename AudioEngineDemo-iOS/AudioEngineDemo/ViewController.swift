//
//  ViewController.swift
//  AudioEngineDemo
//
//  Created by Adam Wilson on 06/11/2018.
//  Copyright © 2018 YourCompany. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    var roomSizeValue: Float
    @IBOutlet var roomSizeValueLabel: UILabel!
    
    var lowCutoffValue: Float
    @IBOutlet var lowCutoffValueLabel: UILabel!
    
    var audioEngine: AudioEngineWrapper!   // 1
    
    required init?(coder aDecoder: NSCoder) {

        roomSizeValue = 0.0
        lowCutoffValue = 1.0
        audioEngine = AudioEngineWrapper()
        
        super.init(coder: aDecoder)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }
    
    @IBAction func roomSizeSliderValueChanged(sender: UISlider) {
        roomSizeValue = sender.value
        print("Room Size slider changing to \(roomSizeValue) ?")
        
        DispatchQueue.main.async {
            self.roomSizeValueLabel.text = "\(self.roomSizeValue)"
        }
        
        
        self.audioEngine.setRoomSize (roomSizeValue)
        
    }

    @IBAction func lowCutoffSliderValueChanged(sender: UISlider) {
        lowCutoffValue = Float (sender.value)
        print("Cutoff slider changing to \(lowCutoffValue) ?")
        self.audioEngine.setLowpassCutoff (lowCutoffValue)
        DispatchQueue.main.async {
            self.lowCutoffValueLabel.text = "\(self.lowCutoffValue)"
            
        }
    }

}

