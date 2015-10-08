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

public class GroveMDSample {
	private static final short speed50 = 127;
	private static final short speed0 = 0;

	static {
		try {
			System.loadLibrary("javaupm_grovemd");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate an I2C Grove Motor Driver on I2C bus 0
		upm_grovemd.GroveMD motors = new upm_grovemd.GroveMD();

		// set direction to clockwise (CW) and set speed to 50%
		System.out.println("Spin M1 and M2 at half speed for 3 seconds");
		motors.setMotorDirections(upm_grovemd.GroveMD.DC_DIRECTION_T.DIR_CW,
				upm_grovemd.GroveMD.DC_DIRECTION_T.DIR_CW);
		motors.setMotorSpeeds(speed50, speed50);
		Thread.sleep(3000);

		// counter clockwise (CCW)
		System.out.println("Reversing M1 and M2 for 3 seconds");
		motors.setMotorDirections(upm_grovemd.GroveMD.DC_DIRECTION_T.DIR_CCW,
				upm_grovemd.GroveMD.DC_DIRECTION_T.DIR_CCW);
		Thread.sleep(3000);

		// stop motors
		System.out.println("Stopping motors");
		motors.setMotorSpeeds(speed0, speed0);
		// ! [Interesting]
	}

}