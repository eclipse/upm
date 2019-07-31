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

public class TP401_Example {
	private static String airQuality(int value) {
		if (value < 50)
			return "Fresh Air";
		if (value < 200)
			return "Normal Indoor Air";
		if (value < 400)
			return "Low Pollution";
		if (value < 600)
			return "High Pollution - Action Recommended";
		return "Very High Pollution - Take Action Immediately";
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate new grove air quality sensor on analog pin A0
		upm_gas.TP401 airSensor = new upm_gas.TP401(0);

		System.out.println(airSensor.name());

		System.out.println("Heating sensor for 3 minutes...");
		// wait 3 minutes for sensor to warm up
		for (int i = 0; i < 3; i++) {
			if (i != 0)
				System.out.println("Please wait, " + i + " minute(s) passed..");
			Thread.sleep(60000);
		}
		System.out.println("Sensor ready!");

		while (true) {
			int value = airSensor.getSample(); // Read raw value
			float ppm = airSensor.getPPM(); // Read CO ppm (can vary slightly
											// from previous read)

			System.out.println("raw: " + value + " ppm: " + ppm + "  " + airQuality(value));

			Thread.sleep(100);
		}
		// ! [Interesting]
	}

}