/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class A110X_Example {
	
	public static void main(String[] args) throws InterruptedException {
		//! [Interesting]
		// Instantiate an A110X sensor on digital pin D2
		upm_a110x.A110X hall = new upm_a110x.A110X(2);
		
		// check every second for the presence of a magnetic field (south polarity)
		while(true){
			if(hall.magnetDetected())
				System.out.println("Magnet (south polarity) detected.");
			else
				System.out.println("No magnet detected.");
			
			Thread.sleep(1000);
		}
		//! [Interesting]
	}
}
