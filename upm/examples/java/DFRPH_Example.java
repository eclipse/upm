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

public class DFRPH_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Instantiate a DFRPH sensor on analog pin A0, with an analog
		// reference voltage of 5.0 V
		upm_dfrph.DFRPH sensor = new upm_dfrph.DFRPH(0, 5.0f);

		// After calibration, set the offset (based on calibration with a pH
		// 7.0 buffer solution). See the UPM sensor documentation for
		// calibrations instructions.
		sensor.setOffset(0.065f);

		// Every second, sample the pH and output it's corresponding
		// analog voltage.
		while (true) {
			System.out.println("Detected volts:  = " + sensor.volts());
			System.out.println("pH value: " + sensor.pH());
			Thread.sleep(1000);
		}

		// ! [Interesting]
	}
}
