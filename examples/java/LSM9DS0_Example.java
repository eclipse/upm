/*
 * Author: Stefan Andritoiu <stefan.andritoiu@gmail.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class LSM9DS0_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting

		// Instantiate an LSM9DS0 using default parameters (bus 1, gyro addr 6b,
		// xm addr 1d)

		upm_lsm9ds0.LSM9DS0 sensor = new upm_lsm9ds0.LSM9DS0();

		sensor.init();

		while (true) {
			sensor.update();

			float[] accel = sensor.getAccelerometer();
			System.out.println("Accelerometer: ");
			System.out.println("AX: " + accel[0] + "; AY: " + accel[1] + "; AZ: " + accel[2]);

			float[] gyro = sensor.getGyroscope();
			System.out.println("Gyroscope: ");
			System.out.println("GX: " + gyro[0] + "; GY: " + gyro[1] + "; GZ: " + gyro[2]);

			float[] magnet = sensor.getMagnetometer();
			System.out.println("Magnetometer: ");
			System.out.println("MX: " + magnet[0] + "; MY: " + magnet[1] + "; MZ: " + magnet[2]);

			System.out.println("Temperature: " + sensor.getTemperature());

			Thread.sleep(500);
		}

		// ! [Interesting]
	}
}
