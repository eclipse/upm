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

public class Temperature_Example {
	public static void main (String args[]) throws InterruptedException {
		//! [Interesting]
        upm_temperature.Temperature temp = new upm_temperature.Temperature(3);
		
		for (int i = 0; i < 10; ++i) {
			
			int celsius = temp.value();
			int fahrneheit = celsius * 2 + 32;

			System.out.println("Celsius: " + celsius);
			System.out.println("Fahrneheit: " + fahrneheit);
			
			Thread.sleep(1000);
		}
		temp.delete();
        //! [Interesting]
	}
}
