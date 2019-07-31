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
public class MQ2_Example {
	private static final short resolution = 5;

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		short[] buffer = new short[128];

		// Attach gas sensor to A0
		upm_gas.MQ2 sensor = new upm_gas.MQ2(0);

		upm_gas.thresholdContext ctx = new upm_gas.thresholdContext();
		ctx.setAverageReading(0);
		ctx.setRunningAverage(0);
		ctx.setAveragedOver(2);

		while (true) {
			int len = sensor.getSampledWindow(2, buffer);

			if (len != 0) {
				int thresh = sensor.findThreshold(ctx, 30, buffer);
				sensor.printGraph(ctx, resolution);
				if (thresh != 0) {
					System.out.println("---Threshold reached---");
				}
			}

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}