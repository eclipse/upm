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
public class MAX44000_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_max44000.MAX44000 sensor = new upm_max44000.MAX44000(0);

		while (true) {
			System.out.println("proximity value = " + sensor.getAmbient());
			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}