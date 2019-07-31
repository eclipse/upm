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


public class ENC03R_Example {
	private static final long CALIBRATION_SAMPLES = 1000;

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Instantiate a ENC03R on analog pin A0
		upm_enc03r.ENC03R gyro = new upm_enc03r.ENC03R(0);

		System.out.println("Please place the sensor in a stable location, and do not");
		System.out.println("move it while calibration takes place");
		System.out.println("This may take a couple of minutes.");

		gyro.calibrate(CALIBRATION_SAMPLES);
		System.out.println("Calibration complete.  Reference value: " + gyro.calibrationValue());

		// Read the input and print both the raw value and the angular velocity,
		// waiting 1 second between readings
		while (true) {
			gyro.update();

			System.out.println("Angular velocity: "
                                           +  gyro.angularVelocity()
                                           + " deg/s");

			Thread.sleep(100);
		}
		// ! [Interesting]
	}

}
