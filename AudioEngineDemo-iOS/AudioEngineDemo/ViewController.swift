//
//  ViewController.swift
//  AudioEngineDemo
//
//  Created by Adam Wilson on 06/11/2018.
//  Copyright © 2018 YourCompany. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    @IBOutlet var roomSizeValueLabel: UILabel!
    @IBOutlet var lowCutoffValueLabel: UILabel!
    
    var audioEngine: AudioEngineWrapper   // 1
    
    required init?(coder aDecoder: NSCoder) {

        audioEngine = AudioEngineWrapper()
        
        super.init(coder: aDecoder)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }
    
    @IBAction func roomSizeSliderValueChanged(sender: UISlider) {
        let roomSizeValue = sender.value
        print("Room Size slider changing to \(roomSizeValue) ?")
        
        DispatchQueue.main.async {
            self.roomSizeValueLabel.text = "\(roomSizeValue)"
        }
        
        
        self.audioEngine.setRoomSize (roomSizeValue)
        
    }

    @IBAction func lowCutoffSliderValueChanged(sender: UISlider) {
        let lowCutoffValue = sender.value
        print("Cutoff slider changing to \(lowCutoffValue) ?")
        self.audioEngine.setLowpassCutoff (lowCutoffValue)
        DispatchQueue.main.async {
            self.lowCutoffValueLabel.text = "\(lowCutoffValue)"
            
        }
    }

}

