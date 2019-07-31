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

//NOT TESTED!!!
public class LDT0028_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Create the LDT0-028 Piezo Vibration Sensor object using AIO pin 0
		upm_ldt0028.LDT0028 sensor = new upm_ldt0028.LDT0028(0);

		System.out.println("Sensor name: " + sensor.name());

		while (true) {
			System.out.println("Sample value: " + sensor.getSample());
			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}