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

public class LineFinder_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a  Line Finder sensor on digital pin D2
		upm_linefinder.LineFinder finder = new upm_linefinder.LineFinder(2);
		// check every second for the presence of white detection
		while (true) {
			boolean val = finder.whiteDetected();
			if (val) {
				System.out.println("White detected");
			} else {
				System.out.println("Black detected");
			}

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}