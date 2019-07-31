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

public class ES08A_Example {
	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_servo.ES08A servo = new upm_servo.ES08A(6);

		// Sets the shaft to 180, then to 90, then to 0,
		// then back to 90, and finally back to 180,
		// pausing for a second in between each angle
		servo.setAngle(180);
		System.out.println("Set angle to 180");
		Thread.sleep(1000);

		servo.setAngle(90);
		System.out.println("Set angle to 90");
		Thread.sleep(1000);

		servo.setAngle(0);
		System.out.println("Set angle to 0");
		Thread.sleep(1000);

		servo.setAngle(90);
		System.out.println("Set angle to 90");
		Thread.sleep(1000);

		servo.setAngle(180);
		System.out.println("Set angle to 180");
		// ! [Interesting]
	}
}