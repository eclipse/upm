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
public class EHR_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a  Ear-clip Heart Rate sensor on digital pin D2
		upm_ehr.EHR heart = new upm_ehr.EHR(2);

		// set the beat counter to 0, init the clock and start counting beats
		heart.clearBeatCounter();
		heart.initClock();
		heart.startBeatCounter();

		while (true) {
			long millis = heart.getMillis();
			long beats = heart.beatCounter();

			// heartRate() requires that at least 5 seconds pass before
			// returning anything other than 0
			int hr = heart.heartRate();

			// output milliseconds passed, beat count, and computed heart rate
			System.out.println("Millis: " + millis + ", Beats: " + beats + ", Heart rate: " + hr);

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}