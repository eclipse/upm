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

public class TP401Sample {
	static {
		try {
			System.loadLibrary("javaupm_gas");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	private static String airQuality(int value) {
		if (value < 50)
			return "Fresh Air";
		if (value < 200)
			return "Normal Indoor Air";
		if (value < 400)
			return "Low Pollution";
		if (value < 600)
			return "High Pollution - Action Recommended";
		return "Very High Pollution - Take Action Immediately";
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate new grove air quality sensor on analog pin A0
		upm_gas.TP401 airSensor = new upm_gas.TP401(0);

		System.out.println(airSensor.name());

		System.out.println("Heating sensor for 3 minutes...");
		// wait 3 minutes for sensor to warm up
		for (int i = 0; i < 3; i++) {
			if (i != 0)
				System.out.println("Please wait, " + i + " minute(s) passed..");
			Thread.sleep(60000);
		}
		System.out.println("Sensor ready!");

		while (true) {
			int value = airSensor.getSample(); // Read raw value
			float ppm = airSensor.getPPM(); // Read CO ppm (can vary slightly
											// from previous read)

			System.out.println("raw: " + value + " ppm: " + ppm + "  " + airQuality(value));

			Thread.sleep(100);
		}
		// ! [Interesting]
	}

}