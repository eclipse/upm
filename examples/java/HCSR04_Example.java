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
import upm_hcsr04.*;

public class HCSR04_Example {

	// ! [Interesting]
	public static void main(String[] args) throws InterruptedException {
		HCSR04 sonar = new HCSR04((short) 2, (short) 4);

		Thread.sleep(1000);

		while (true) {
			System.out.println("Get distance");
			double distance = sonar.getDistance();
			System.out.println("Distance: " + distance);

			Thread.sleep(5000);
		}
	}
}
// ! [Interesting]