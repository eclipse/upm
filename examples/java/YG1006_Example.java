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

public class YG1006_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a yg1006 flame sensor on digital pin D2
		upm_yg1006.YG1006 flame = new upm_yg1006.YG1006(2);

		while (true) {
			boolean val = flame.flameDetected();
			if (val) {
				System.out.println("Flame detected");
			} else {
				System.out.println("No flame detected");
			}

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}