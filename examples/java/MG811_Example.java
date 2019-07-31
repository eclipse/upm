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

public class MG811_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Instantiate an MG811 on analog pin A0, and digital pin D2 with an
		// analog reference voltage of 5.0V
		upm_mg811.MG811 sensor = new upm_mg811.MG811(0, 2, (float) 5.0);

		// Every tenth of a second, sample the sensor and output it's
		// detected CO2 concentration in parts per million (ppm)

		while (true) {
			System.out.println("CO2 concentration in PPM: " + sensor.ppm());
			Thread.sleep(100);
		}

		// ! [Interesting]
	}
}
