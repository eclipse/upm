/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

//NOT TESTED!!!
public class DS1307_Example {

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