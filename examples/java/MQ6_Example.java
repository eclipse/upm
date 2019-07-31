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

public class MQ6_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		short[] buffer = new short[128];

		// Attach gas sensor to A0
		upm_gas.MQ6 sensor = new upm_gas.MQ6(0);

		upm_gas.thresholdContext ctx = new upm_gas.thresholdContext();
		ctx.setAverageReading(0);
		ctx.setRunningAverage(0);
		ctx.setAveragedOver(2);

		// Infinite loop, ends when script is cancelled
		// Repeatedly, take a sample every 2 microseconds;
		// find the average of 128 samples; and
		// print a running graph of asterisks as averages
		while (true) {
			int len = sensor.getSampledWindow(2, buffer);

			if (len != 0) {
				int thresh = sensor.findThreshold(ctx, 30, buffer);
				sensor.printGraph(ctx, (short) 5);
				if (thresh != 0) {
					// do something ....
				}
			}
		}
		// ! [Interesting]
	}
}
