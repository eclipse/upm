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

import java.time.LocalDateTime;

//NOT TESTED!!!
public class TM1637_Example {

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