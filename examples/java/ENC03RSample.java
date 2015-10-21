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
public class ENC03RSample {
	private static final long CALIBRATION_SAMPLES = 1000;

	static {
		try {
			System.loadLibrary("javaupm_enc03r");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

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
			long val = gyro.value();
			double av = gyro.angularVelocity(val);

			System.out.println("Raw value: " + val + ", angular velocity: " + av + " deg/s");

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}