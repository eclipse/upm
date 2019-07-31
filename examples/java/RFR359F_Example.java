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

public class RFR359F_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate an RFR359F digital pin D2
		upm_rfr359f.RFR359F dInterruptor = new upm_rfr359f.RFR359F(2);

		while (true) {
			if (dInterruptor.objectDetected()) {
				System.out.println("Object detected");
			} else {
				System.out.println("Area is clear!");
			}

			Thread.sleep(100);
		}
		// ! [Interesting]
	}

}