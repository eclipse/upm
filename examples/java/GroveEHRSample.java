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
public class GroveEHRSample {

	static {
		try {
			System.loadLibrary("javaupm_groveehr");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Grove Ear-clip Heart Rate sensor on digital pin D2
		upm_groveehr.GroveEHR heart = new upm_groveehr.GroveEHR(2);

		// set the beat counter to 0, init the clock and start counting beats
		heart.clearBeatCounter();
		heart.initClock();
		heart.startBeatCounter();

		while (true) {
			long millis = heart.getMillis();
			long beats = heart.beatCounter();

			// heartRate() requires that at least 5 seconds pass before
			// returning anything other than 0
			int hr = heart.heartRate();

			// output milliseconds passed, beat count, and computed heart rate
			System.out.println("Millis: " + millis + ", Beats: " + beats + ", Heart rate: " + hr);

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}