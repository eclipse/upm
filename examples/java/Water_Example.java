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

public class Water_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Water sensor on digital pin D2
		upm_water.Water water = new upm_water.Water(2);

		while (true) {
			boolean val = water.isWet();
			if (val) {
				System.out.println("Sensor is wet");
			} else {
				System.out.println("Sensor is dry");
			}

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}
