/*
 * Author: Andrei Vasiliu <andrei.vasiliu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class LED_Example {
	public static void main (String args[]) throws InterruptedException {
		//! [Interesting]
        upm_led.Led led = new upm_led.Led(2);
		
		for (int i = 0; i < 10; ++i) {
			led.on();
			Thread.sleep(1000);
			led.off();
			Thread.sleep(1000);
		}
		led.delete();
        //! [Interesting]
	}
}
