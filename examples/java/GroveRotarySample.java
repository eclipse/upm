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

public class GroveRotarySample {

	static {
		try {
			System.loadLibrary("javaupm_grove");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_grove.GroveRotary knob = new upm_grove.GroveRotary(0);

		while (true) {
			float abs_value = knob.abs_value(); // Absolute raw value
			float abs_deg = knob.abs_deg(); // Absolute degrees
			float abs_rad = knob.abs_rad(); // Absolute radians
			float rel_value = knob.rel_value(); // Relative raw value
			float rel_deg = knob.rel_deg(); // Relative degrees
			float rel_rad = knob.rel_rad(); // Relative radians

			System.out.println("Absolute: " + abs_value + " raw, " + abs_deg + " deg, " + abs_rad
					+ " rad");
			System.out.println("Relative: " + rel_value + " raw, " + rel_deg + " deg, " + rel_rad
					+ " rad");

			Thread.sleep(3000);
		}
		// ! [Interesting]
	}

}