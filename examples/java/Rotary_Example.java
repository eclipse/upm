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

public class Rotary_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_rotary.Rotary knob = new upm_rotary.Rotary(0);

		while (true) {
			float abs_value = knob.abs_value(); // Absolute raw value
			float abs_deg = knob.abs_deg(); // Absolute degrees
			float abs_rad = knob.abs_rad(); // Absolute radians
			float rel_value = knob.rel_value(); // Relative raw value
			float rel_deg = knob.rel_deg(); // Relative degrees
			float rel_rad = knob.rel_rad(); // Relative radians

			System.out.println("Absolute: " + abs_value + " raw, " + abs_deg + " deg, " + abs_rad
					+ " rad");
			System.out.println("Relative: " + rel_value + " raw, " + rel_deg + " deg, " + rel_rad
					+ " rad");

			Thread.sleep(3000);
		}
		// ! [Interesting]
	}

}
