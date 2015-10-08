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
public class ULN200XASample {

	static {
		try {
			System.loadLibrary("javaupm_uln200xa");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Stepper motor on a ULN200XA Dual H-Bridge.
		// Wire the pins so that I1 is pin D8, I2 is pin D9, I3 is pin D10 and
		// I4 is pin D11
		upm_uln200xa.ULN200XA uln200xa = new upm_uln200xa.ULN200XA(4096, 8, 9, 10, 11);

		uln200xa.setSpeed(5);
		System.out.println("Rotating 1 revolution clockwise.");
		uln200xa.setDirection(upm_uln200xa.ULN200XA.ULN200XA_DIRECTION_T.DIR_CW);
		uln200xa.stepperSteps(4096);

		System.out.println("Sleeping for 2 seconds...");
		Thread.sleep(2000);

		System.out.println("Rotating 1/2 revolution counter clockwise.");
		uln200xa.setDirection(upm_uln200xa.ULN200XA.ULN200XA_DIRECTION_T.DIR_CCW);
		uln200xa.stepperSteps(2048);

		// turn off the power
		uln200xa.release();
		// ! [Interesting]

		System.out.println("Exiting...");
	}

}