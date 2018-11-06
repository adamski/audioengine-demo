//
//  ViewController.swift
//  AudioEngineDemo
//
//  Created by Adam Wilson on 06/11/2018.
//  Copyright © 2018 YourCompany. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    @IBOutlet var roomSizeValue: UILabel!
    @IBOutlet var lowCutoffValue: UILabel!

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
    }
    
    @IBAction func roomSizeSliderValueChanged(sender: UISlider) {
        let currentValue = sender.value
        print("Room Size slider changing to \(currentValue) ?")
        DispatchQueue.main.async {
            self.roomSizeValue.text = "\(currentValue)"
        }
    }

    @IBAction func lowCutoffSliderValueChanged(sender: UISlider) {
        let currentValue = sender.value
        print("Cutoff slider changing to \(currentValue) ?")
        DispatchQueue.main.async {
            self.lowCutoffValue.text = "\(currentValue)"
        }
    }

}

