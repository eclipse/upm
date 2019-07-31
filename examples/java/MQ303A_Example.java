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

public class MQ303A_Example {

	public static void main(String[] args) throws InterruptedException {
		// Instantiate an mq303a sensor on analog pin A0

		// This device uses a heater powered from an analog I/O pin.
		// If using A0 as the data pin, then you need to use A1, as the heater
		// pin (if using a grove mq303a). For A1, we can use the D15 gpio,
		// setup as an output, and drive it low to power the heater.
		upm_mq303a.MQ303A mq303a = new upm_mq303a.MQ303A(1, 15);

		System.out.println("Enabling heater and waiting 2 minutes for warmup.");
		mq303a.heaterEnable(true);
		Thread.sleep(120000);

		System.out.println("This sensor may need to warm until the value drops below about 450.");

		for (int i = 1; i < 10; i++) {
			int val = mq303a.value();
			System.out.println("Alcohol detected (higher means stronger alcohol): " + val);

			Thread.sleep(1000);
		}

		mq303a.heaterEnable(false);
		System.out.println("Exiting");
	}

}
