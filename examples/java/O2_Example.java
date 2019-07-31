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

import upm_o2.O2;

public class O2_Example {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//! [Interesting]
		// Initializing the Grove O2 sensor on the A) analog pin
		O2 o2 = new O2(0);

		while(true){
			System.out.println("The output voltage is: "+o2.voltageValue());
			try {
				Thread.sleep(100);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("The following exception has occurred: "+e.getMessage());
			}
		}
		//! [Interesting]
	}
}
