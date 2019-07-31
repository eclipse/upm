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

public class Loudness_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Loudness sensor on analog pin A0, with an analog
		// reference voltage of 5.0V
		upm_loudness.Loudness loud = new upm_loudness.Loudness(0, (float) 5.0);

		// Every tenth of a second, sample the loudness and output it's
		// corresponding analog voltage.

		while (true) {
			System.out.println("Detected loudness (volts): " + loud.loudness());
			Thread.sleep(100);
		}
		// ! [Interesting]
	}
}
