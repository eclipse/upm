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
public class MHZ16_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a MHZ16 serial CO2 sensor on uart 0.
		upm_mhz16.MHZ16 co2 = new upm_mhz16.MHZ16(0);

		System.out.println("Make sure that the sensor has had at least 3 minutes to warm up");
		System.out.println("or you will not get valid results.");
		System.out.println("The temperature reported is not the ambient temperature,");
		System.out.println("but rather the temperature of the sensor elements.");

		while (true) {
			if (!co2.getData()) {
				System.out.println("Failed to retrieve data");
				continue;
			}
			System.out.println("CO2 concentration: " + co2.getGas() + "PPM, Temperature (in C): "
					+ co2.getTemperature());

			Thread.sleep(2000);
		}
		// ! [Interesting]
	}
}