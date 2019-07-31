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

public class TSL2561_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a TSL2561 Digital Light Sensor on I2C
		upm_tsl2561.TSL2561 sensor = new upm_tsl2561.TSL2561();

		while (true) {
			System.out.println("Lux = " + sensor.getLux());

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}