/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_veml6070.VEML6070;

public class VEML6070_Example {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		//! [Interesting]
		// Instantiate a VEML6070 UV sensor
		VEML6070 veml = new VEML6070(0);
		
		while(true){
			System.out.println("UV Value: "+veml.getUVIntensity());
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("The following exception occurred: "+e.getMessage());
			}
		}
		//! [Interesting]
	}

}
