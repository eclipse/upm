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
public class DS1307Sample {

	static {
		try {
			System.loadLibrary("javaupm_ds1307");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	static private void printTime(upm_ds1307.DS1307 rtc) {
		System.out.print("The time is: " + rtc.getMonth() + "/" + rtc.getDayOfMonth() + "/"
				+ rtc.getYear() + " " + rtc.getHours() + ":" + rtc.getMinutes() + ":"
				+ rtc.getSeconds());

		if (rtc.getAmPmMode()) {
			if (rtc.getAmPmMode())
				System.out.print(" PM");
			else
				System.out.print(" AM");
		}

		System.out.println();

		if (rtc.getAmPmMode())
			System.out.println("Clock is in AM/PM mode");
		else
			System.out.println("Clock is in 24h mode");

	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a DS1037 on I2C bus 0
		upm_ds1307.DS1307 rtc = new upm_ds1307.DS1307(0);

		// always do this first
		System.out.println("Loading the current time...");
		if (!rtc.loadTime()) {
			System.err.println("rtc->loadTime() failed.");
			System.exit(-1);
		}

		printTime(rtc);

		// set the year as an example
		System.out.println("setting the year to 50");
		rtc.setYear(50);
		rtc.setTime();

		// reload the time and print it
		rtc.loadTime();
		printTime(rtc);

		// ! [Interesting]
	}
}