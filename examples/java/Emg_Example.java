/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
import upm_emg.EMG;

public class Emg_Example {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//! [Interesting]
		// Instantiating the Grove EMG sensor on Analog pin 0
		EMG emg = new EMG(0);
		System.out.println("Calibrating ... ");
		emg.calibrate();

		while(true){
			System.out.println("EMG Val: "+emg.value());
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("The following exception occurred: "+e.getMessage());
			}
		}
	}
	//! [Interesting]
}
