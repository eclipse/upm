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

import java.util.AbstractList;

//NOT TESTED!!!
public class MMA7455_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_mma7455.MMA7455 sensor = new upm_mma7455.MMA7455(0);

		while (true) {
			AbstractList<Short> val = sensor.readData();
			System.out.println("Accelerometer X: " + val.get(0) + ", Y: " + val.get(1) + ", Z: " + val.get(2));

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}
