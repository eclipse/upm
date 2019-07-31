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

public class OTP538U_Example {
	private static final float OTP538U_AREF = 5;

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a OTP538U on analog pins A0 and A1
		// A0 is used for the Ambient Temperature and A1 is used for the Object
		// temperature.
		upm_otp538u.OTP538U temps = new upm_otp538u.OTP538U(0, 1, OTP538U_AREF);

		while (true) {
			System.out.println("Ambient temp: " + temps.ambientTemperature() + " C");
			System.out.println("Object temp: " + temps.objectTemperature() + " C");

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}