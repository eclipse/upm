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
public class ADC121C021_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate an ADC121C021 on I2C bus 0
		upm_adc121c021.ADC121C021 adc = new upm_adc121c021.ADC121C021(0);

		// An analog sensor, such as a Grove light sensor,
		// must be attached to the adc
		// Prints the value and corresponding voltage every 50 milliseconds
		while (true) {
			int val = adc.value();

			System.out.println("ADC value: " + val + "Volts = " + adc.valueToVolts(val));

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}
