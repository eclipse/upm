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
public class Adxl345_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		short[] val;
		float[] accel;

		// Note: Sensor only works at 3.3V on the Intel Edison with Arduino
		// breakout
		upm_adxl345.Adxl345 sensor = new upm_adxl345.Adxl345(0);

		while (true) {
			sensor.update();
			val = sensor.getRawValues();
			accel = sensor.getAcceleration();

			System.out.println("Current scale: " + sensor.getScale());
			System.out.println("Raw Values: X: " + val[0] + " Y: " + val[1] + " Z: " + val[2]);
			System.out.println("Acceleration: X: " + accel[0] + "g Y: " + accel[1] + "g Z: "
					+ accel[2] + "g");

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}