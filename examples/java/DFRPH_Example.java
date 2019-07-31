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

public class DFRPH_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Instantiate a DFRPH sensor on analog pin A0, with an analog
		// reference voltage of 5.0 V
		upm_dfrph.DFRPH sensor = new upm_dfrph.DFRPH(0, 5.0f);

		// After calibration, set the offset (based on calibration with a pH
		// 7.0 buffer solution). See the UPM sensor documentation for
		// calibrations instructions.
		sensor.setOffset(0.065f);

		// Every second, sample the pH and output it's corresponding
		// analog voltage.
		while (true) {
			System.out.println("Detected volts:  = " + sensor.volts());
			System.out.println("pH value: " + sensor.pH());
			Thread.sleep(1000);
		}

		// ! [Interesting]
	}
}
