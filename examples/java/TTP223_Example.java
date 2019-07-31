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

public class TTP223_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_ttp223.TTP223 touch = new upm_ttp223.TTP223(7);

		while (true) {
			if (touch.isPressed())
				System.out.println(touch.name() + " is pressed");
			else
				System.out.println(touch.name() + " is not pressed");

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}