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

public class BISS0001_Example{

	public static void main(String[] args) throws InterruptedException {
//! [Interesting]
        // Instantiate a Grove Motion sensor on GPIO pin D7
		upm_biss0001.BISS0001 motion = new upm_biss0001.BISS0001(7);
	
		while (true) {
			boolean value = motion.value();
			
			if(value)
				System.out.println("Detecting moving object");
			else
				System.out.println("No moving objects detected");
			
			Thread.sleep(1000);
		}
//! [Interesting]
	}

}
