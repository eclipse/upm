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

public class Button_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Create the button object using GPIO pin 2
		upm_button.Button button = new upm_button.Button(2);

		while (true) {
			System.out.println(button.name() + " value is " + button.value());

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}
