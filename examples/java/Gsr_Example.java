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

import upm_gsr.GSR;

public class Gsr_Example {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		//! [Interesting]
		// Instantiate a Grove GSR sensor on analog pin A0
		GSR gsr = new GSR(0);
		System.out.println("Calibrating...");
		gsr.calibrate();
		
		while(true){
			System.out.println("Value: "+gsr.value());
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
