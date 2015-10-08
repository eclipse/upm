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
public class MHZ16Sample {

	static {
		try {
			System.loadLibrary("javaupm_mhz16");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		int[] gas = new int[1];
		int[] temp = new int[1];

		// Instantiate a MHZ16 serial CO2 sensor on uart 0.
		upm_mhz16.MHZ16 co2 = new upm_mhz16.MHZ16(0);

		System.out.println("Make sure that the sensor has had at least 3 minutes to warm up");
		System.out.println("or you will not get valid results.");
		System.out.println("The temperature reported is not the ambient temperature,");
		System.out.println("but rather the temperature of the sensor elements.");

		while (true) {
			co2.getData(gas, temp);
			System.out.println("CO2 concentration: " + gas[0] + "PPM, Temperature (in C): "
					+ temp[0]);

			Thread.sleep(2000);
		}
		// ! [Interesting]
	}
}