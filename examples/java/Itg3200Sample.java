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
public class Itg3200Sample {

	static {
		try {
			System.loadLibrary("javaupm_itg3200");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		int[] rot;
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