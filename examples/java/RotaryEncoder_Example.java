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

public class RotaryEncoder_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Grove Rotary Encoder, using signal pins D2 and D3
		upm_rotaryencoder.RotaryEncoder rotaryencoder = new upm_rotaryencoder.RotaryEncoder(2, 3);

		while (true) {
			System.out.println("Position: " + rotaryencoder.position());

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}