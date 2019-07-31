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

public class Button_intr_Example {

	private static int counter = 0;

	public static void incrementCounter() {
        counter++;
    }
    public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_button.Button b = new upm_button.Button(2);

		ButtonISR callback = new ButtonISR();
		b.installISR(2, callback);

		while (true) {
			System.out.println("Counter: " + counter);
			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}

class ButtonISR implements Runnable {
	public ButtonISR() {
		super();
	}

	public void run() {
		Button_intr_Example.incrementCounter();
		System.out.println("Button pressed!");
	}
}
