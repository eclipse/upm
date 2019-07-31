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

public class RPR220_intr_Example {

	private static int counter = 0;

	public static void incrementCounter() {
        counter++;
    }
	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// This example uses an interrupt handler to increment a counter

		// Instantiate an RPR220 digital pin D2
		upm_rpr220.RPR220 sensor = new upm_rpr220.RPR220(2);

		RPRISR callback = new RPRISR();
		sensor.installISR(callback);

		while (true) {
			System.out.println("Counter: " + counter);
			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}

class RPRISR implements Runnable {
	public RPRISR() {
		super();
	}
	public void run() {
		RPR220_intr_Example.incrementCounter();
	}
}