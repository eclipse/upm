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
public class Adxl345Sample {

	static {
		try {
			System.loadLibrary("javaupm_adxl345");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

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
			System.out.println("Raw Values: X: " + val[0] + " Y: " + val[1]
					+ " Z: " + val[2]);
			System.out.println("Acceleration: X: " + accel[0] + "g Y: "
					+ accel[1] + "g Z: " + accel[2] + "g");

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}