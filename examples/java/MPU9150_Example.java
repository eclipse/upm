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
