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

public class Joystick12_Example {

	public static void main(String[] args) throws InterruptedException {
		//! [Interesting]
		// Instantiate a joystick on analog pins A0 and A1
		upm_joystick12.Joystick12 joystick = new upm_joystick12.Joystick12(0,1);

		// Print the X and Y input values every second
		while(true){
			float x = joystick.getXInput();
			float y = joystick.getYInput();

			System.out.println("Driving X: " + x + " , and Y: " + y );

			Thread.sleep(1000);
		}
		//! [Interesting]
	}

}
