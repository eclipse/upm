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

public class Microphone_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		short[] buffer = new short[128];

		// Attach microphone to analog port A0
		upm_mic.Microphone sensor = new upm_mic.Microphone(0);

		upm_mic.thresholdContext ctx = new upm_mic.thresholdContext();
		ctx.setAverageReading(0);
		ctx.setRunningAverage(0);
		ctx.setAveragedOver(2);

		// Repeatedly, take a sample every 2 microseconds;
		// find the average of 128 samples; and
		// print a running graph of the averages
		while (true) {
			int len = sensor.getSampledWindow(2, buffer);

			if (len != 0) {
				int thresh = sensor.findThreshold(ctx, 30, buffer);
				sensor.printGraph(ctx);
				if (thresh != 0) {
					System.out.println("---Threshold reached---  " + ctx.getRunningAverage() + "  "
							+ ctx.getAverageReading());
				}
			}
		}
		// ! [Interesting]
	}
}