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

public class RPR220_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// This example uses a simple method to determine current status

		// Instantiate an RPR220 digital pin D2
		upm_rpr220.RPR220 sensor = new upm_rpr220.RPR220(2);

		while (true) {
			if (sensor.blackDetected())
				System.out.println("Black detected");
			else
				System.out.println("Black NOT detected");

			Thread.sleep(100);
		}
		// ! [Interesting]
	}
}