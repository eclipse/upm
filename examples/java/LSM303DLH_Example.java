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
public class LSM303DLH_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate LSM303DLH compass on I2C
		upm_lsm303dlh.LSM303DLH sensor = new upm_lsm303dlh.LSM303DLH(0);

		// Get the coordinate data
		sensor.getCoordinates();
		short[] coor = sensor.getRawCoorData(); // in XYZ order.
		// The sensor returns XZY, but the driver compensates and makes it XYZ

		// Print out the X, Y, and Z coordinate data using two different methods
		System.out.println("coor: rX " + coor[0] + " - rY " + coor[1] + " - rZ " + coor[2]);
		System.out.println("coor: gX " + sensor.getCoorX() + " - gY " + sensor.getCoorY()
				+ " - gZ " + sensor.getCoorZ());

		// Get and print out the heading
		System.out.println("heading: " + sensor.getHeading());

		// Get the acceleration
		sensor.getAcceleration();
		short[] accel = sensor.getRawAccelData();

		// Print out the X, Y, and Z acceleration data using two different
		// methods
		System.out.println("acc: rX " + accel[0] + " - rY " + accel[1] + " - rZ " + accel[2]);
		System.out.println("acc: gX " + sensor.getAccelX() + " - gY " + sensor.getAccelY()
				+ " - gZ " + sensor.getAccelZ());

		// ! [Interesting]
	}

}
