/*
* Author: Abhishek Malik <abhishek.malik@intel.com>
* Copyright (c) 2016 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/
import upm_ad8232.AD8232;

public class Ad8232_Example {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//! [Interesting]
		// Instantiate a Ad8232 sensor on digital pins 10 (LO+), 11 (LO-)
		// and an analog pin, 0 (OUTPUT)
		AD8232 ad8232 = new AD8232(10, 11, 0);
		while(true){
			// Output the raw numbers from the ADC, for plotting elsewhere.
			// A return of 0 indicates a Lead Off (LO) condition.
			// In theory, this data could be fed to software like Processing
			// (https://www.processing.org/) to plot the data just like an
			// EKG you would see in a hospital.
			System.out.println(ad8232.value());
			try {
				Thread.sleep(1);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("The following error has occurred: "+e.getMessage());
			}
		}
	}

}
