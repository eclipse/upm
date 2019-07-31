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
public class MPL3115A2_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a MPL3115A2 sensor on I2C
		upm_mpl3115a2.MPL3115A2 sensor = new upm_mpl3115a2.MPL3115A2(0);

		while (true) {
			System.out.println("Pressure: " + sensor.getPressure());
			System.out.println("Altitude: " + sensor.getAltitude());
			System.out.println("Sealevel pressure: " + sensor.getSealevelPressure());
			System.out.println("Temperature: " + sensor.getTemperature());
			System.out.println();

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}