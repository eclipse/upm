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

import java.time.LocalDateTime;

//NOT TESTED!!!
public class TM1637Sample {

	static {
		try {
			System.loadLibrary("javaupm_tm1637");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// TM1637 on pins 0 (clk) and 1 (dio)
		upm_tm1637.TM1637 myDisplay = new upm_tm1637.TM1637(0, 1);

		// Start a box using 7-segment encoding
		myDisplay.write(0x39, 0x09, 0x09);

		// Finish box using writeAt function
		myDisplay.writeAt(3, ']');

		// Wait 3 seconds
		Thread.sleep(3000);

		LocalDateTime now;
		while (true) {
			now = LocalDateTime.now();
			int hour = now.getHour();
			int min = now.getMinute();
			int sec = now.getSecond();

			System.out.println(hour + ":" + min + ":" + sec);
			myDisplay.writeString(hour + ":" + min);

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}