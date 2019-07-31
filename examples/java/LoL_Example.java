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
public class LoL_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_lol.LoL sensor = new upm_lol.LoL();

		int x = 0, y = 0;
		while (true) {
			// revert pixel
			sensor.setPixel(x, y, sensor.getPixel(x, y));

			if (++x == 13) {
				x = 0;
				y++;
			}
			if (y == 9)
				y = 0;
			Thread.sleep(10);
		}
		// ! [Interesting]
	}
}
