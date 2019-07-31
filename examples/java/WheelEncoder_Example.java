/*
 * Author: Stefan Andritoiu <stefan.andritoiu@gmail.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class WheelEncoder_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a DFRobot Wheel Encoder on digital pin D2
		upm_wheelencoder.WheelEncoder sensor = new upm_wheelencoder.WheelEncoder(2);

		// set the counter to 0 and start counting
		sensor.clearCounter();
		sensor.startCounter();

		while (true) {
			// output milliseconds passed and current sensor count
			System.out.println("Millis: " + sensor.getMillis() + "; Count: " + sensor.counter());
			Thread.sleep(1000);
		}

		// ! [Interesting]
	}
}

