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

 import java.util.AbstractList;
 
//NOT TESTED!!!
public class MPU9150_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_mpu9150.MPU9150 sensor = new upm_mpu9150.MPU9150();

		sensor.init();

		while (true) {
			sensor.update();

// These don't exist
//			float[] accel = sensor.getAccelerometer();
//			System.out.println("Accelerometer: " + "AX: " + accel[0] + " AY: " + accel[1] + " AZ: "
//					+ accel[2]);
//
//			float[] gyro = sensor.getGyroscope();
//			System.out.println("Gryoscope: " + "GX: " + gyro[0] + " GY: " + gyro[1] + " GZ: "
//					+ gyro[2]);

			AbstractList<Float> magn = sensor.getMagnetometer();
			System.out.println("Magnetometer: " + "MX: " + magn.get(0) + " MY: " + magn.get(1) + " MZ: " + magn.get(2));

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}
