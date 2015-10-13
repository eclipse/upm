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

public class MicrophoneSample {

	static {
		try {
			System.loadLibrary("javaupm_mic");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		short[] buffer = new short[128];

		// Attach microphone to analog port A0
		upm_mic.Microphone sensor = new upm_mic.Microphone(0);

		upm_mic.thresholdContext ctx = new upm_mic.thresholdContext();
		ctx.setAverageReading(0);
		ctx.setRunningAverage(0);
		ctx.setAveragedOver(2);

		// Repeatedly, take a sample every 2 microseconds;
		// find the average of 128 samples; and
		// print a running graph of the averages
		while (true) {
			int len = sensor.getSampledWindow(2, buffer);

			if (len != 0) {
				int thresh = sensor.findThreshold(ctx, 30, buffer);
				sensor.printGraph(ctx);
				if (thresh != 0) {
					System.out.println("---Threshold reached---  " + ctx.getRunningAverage() + "  "
							+ ctx.getAverageReading());
				}
			}
		}
		// ! [Interesting]
	}
}