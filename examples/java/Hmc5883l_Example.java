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

public class Hmc5883l_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate on I2C
		upm_hmc5883l.Hmc5883l compas = new upm_hmc5883l.Hmc5883l(0);

		short[] pos;

		// Set your declination from true north in radians
		compas.set_declination(0.2749f);

		while (true) {
			// Update the coordinates
			compas.update();

			pos = compas.coordinates();
			System.out.println("Coor: " + pos[0] + " " + pos[1] + " " + pos[2]);
			System.out.println("Heading: " + compas.heading() + " Direction:" + compas.direction());

			Thread.sleep(2000);
		}
		// ! [Interesting]
	}

}