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

public class BMA220_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Instantiate an BMA220 using default parameters (bus 0, addr 0x0a)
		upm_bma220.BMA220 sensor = new upm_bma220.BMA220();

		while (true) {
			float acc[];

			sensor.update();
			acc = sensor.getAccelerometer();
			System.out.print("Accelerometer: ");
			System.out.println("AX: " + acc[0] + "AY: " + acc[1] + "AZ: " + acc[2]);

			Thread.sleep(1000);
		}

		// ! [Interesting]
	}
}
