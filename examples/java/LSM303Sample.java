/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

//NOT TESTED!!!
public class LSM303Sample {

	static {
		try {
			System.loadLibrary("javaupm_lsm303");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate LSM303 compass on I2C
		upm_lsm303.LSM303 sensor = new upm_lsm303.LSM303(0);

		// Get the coordinate data
		sensor.getCoordinates();
		int[] coor = sensor.getRawCoorData(); // in XYZ order.·
		// The sensor returns XZY, but the driver compensates and makes it XYZ

		// Print out the X, Y, and Z coordinate data using two different methods
		System.out.println("coor: rX " + coor[0] + " - rY " + coor[1] + " - rZ " + coor[2]);
		System.out.println("coor: gX " + sensor.getCoorX() + " - gY " + sensor.getCoorY()
				+ " - gZ " + sensor.getCoorZ());

		// Get and print out the heading
		System.out.println("heading: " + sensor.getHeading());

		// Get the acceleration
		sensor.getAcceleration();
		int[] accel = sensor.getRawAccelData();

		// Print out the X, Y, and Z acceleration data using two different
		// methods
		System.out.println("acc: rX " + accel[0] + " - rY " + accel[1] + " - rZ " + accel[2]);
		System.out.println("acc: gX " + sensor.getAccelX() + " - gY " + sensor.getAccelY()
				+ " - gZ " + sensor.getAccelZ());

		// ! [Interesting]
	}

}