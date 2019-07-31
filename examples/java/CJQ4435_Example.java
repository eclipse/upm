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

public class CJQ4435_Example {

	public static void main(String[] args) throws InterruptedException {
		// Instantiate a CJQ4435 MOSFET on a PWM capable digital pin D3
		upm_cjq4435.CJQ4435 mosfet = new upm_cjq4435.CJQ4435(3);

		mosfet.setPeriodMS(10);
		mosfet.enable(true);

		while (true) {
			// start with a duty cycle of 0.0 (off) and increment to 1.0 (on)
			for (float i = 0; i <= 1; i += 0.1) {
				mosfet.setDutyCycle(i);
				Thread.sleep(100);
			}
			Thread.sleep(1000);

			// Now take it back down
			// start with a duty cycle of 1.0 (on) and decrement to 0.0 (off)
			for (float i = 1; i >= 0; i -= 0.1) {
				mosfet.setDutyCycle(i);
				Thread.sleep(100);
			}
			Thread.sleep(1000);
		}
	}

}