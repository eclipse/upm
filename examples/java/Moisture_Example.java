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

public class Moisture_Example {
	public static void main(String args[]) throws InterruptedException {
		// ! [Interesting]
		upm_moisture.Moisture gm = new upm_moisture.Moisture(1);

		while (true) {
			int moisture_val = gm.getMoisture();
			String result;

			if (moisture_val >= 0 && moisture_val < 300)
				result = "Dry";
			else if ((moisture_val >= 0 && moisture_val < 300))
				result = "Moist";
			else
				result = "Wet";

			System.out.println("Moisture Value: " + moisture_val + ", " + result);

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}