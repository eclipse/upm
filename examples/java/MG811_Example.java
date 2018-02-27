/*
 * Author: Stefan Andritoiu <stefan.andritoiu@gmail.com>
 * Copyright (c) 2017 Intel Corporation.
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

public class MG811_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Instantiate an MG811 on analog pin A0, and digital pin D2 with an
		// analog reference voltage of 5.0V
		upm_mg811.MG811 sensor = new upm_mg811.MG811(0, 2, (float) 5.0);

		// Every tenth of a second, sample the sensor and output it's
		// detected CO2 concentration in parts per million (ppm)

		while (true) {
			System.out.println("CO2 concentration in PPM: " + sensor.ppm());
			Thread.sleep(100);
		}

		// ! [Interesting]
	}
}
