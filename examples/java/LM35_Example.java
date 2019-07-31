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

public class LM35_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Instantiate a LM35 on analog pin A0, with a default analog
		// reference voltage of 5.0
		upm_lm35.LM35 sensor = new upm_lm35.LM35(0);

		// Every half second, sample the sensor and output the temperature

		while (true) {
			System.out.println("Temperature: " + sensor.getTemperature() + " C");
			Thread.sleep(500);
		}

		// ! [Interesting]
	}
}
