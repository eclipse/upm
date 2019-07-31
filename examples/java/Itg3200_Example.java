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
public class Itg3200_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		short[] rot;
		float[] ang;

		// Note: Sensor not supported on Intel Edison with Arduino breakout
		upm_itg3200.Itg3200 gyro = new upm_itg3200.Itg3200(0);

		while (true) {
			gyro.update();
			rot = gyro.getRawValues();
			ang = gyro.getRotation();

			System.out.println("Raw Values: X: " + rot[0] + " Y: " + rot[1] + " Z: " + rot[2]);
			System.out.println("Angular Velocities: X: " + ang[0] + " Y: " + ang[1] + " Z: "
					+ ang[2]);

			System.out.println("Temp: " + gyro.getTemperature() + ", Raw: " + gyro.getRawTemp());

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}