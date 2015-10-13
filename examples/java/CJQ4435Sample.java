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

public class CJQ4435Sample {

	static {
		try {
			System.loadLibrary("javaupm_cjq4435");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// Instantiate a CJQ4435 MOSFET on a PWM capable digital pin D3
		upm_cjq4435.CJQ4435 mosfet = new upm_cjq4435.CJQ4435(3);

		mosfet.setPeriodMS(10);
		mosfet.enable(true);

		while (true) {
			// start with a duty cycle of 0.0 (off) and increment to 1.0 (on)
			for (float i = 0; i <= 1; i += 0.1) {
				mosfet.setDutyCycle(i);
				Thread.sleep(100);
			}
			Thread.sleep(1000);

			// Now take it back down
			// start with a duty cycle of 1.0 (on) and decrement to 0.0 (off)
			for (float i = 1; i >= 0; i -= 0.1) {
				mosfet.setDutyCycle(i);
				Thread.sleep(100);
			}
			Thread.sleep(1000);
		}
	}

}