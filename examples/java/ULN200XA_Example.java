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

public class ULN200XA_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Stepper motor on a ULN200XA Dual H-Bridge.
		// Wire the pins so that I1 is pin D8, I2 is pin D9, I3 is pin D10 and
		// I4 is pin D11
		upm_uln200xa.ULN200XA uln200xa = new upm_uln200xa.ULN200XA(4096, 8, 9, 10, 11);

		uln200xa.setSpeed(5);
		System.out.println("Rotating 1 revolution clockwise.");
		uln200xa.setDirection(upm_uln200xa.ULN200XA_DIRECTION_T.ULN200XA_DIR_CW);
		uln200xa.stepperSteps(4096);

		System.out.println("Sleeping for 2 seconds...");
		Thread.sleep(2000);

		System.out.println("Rotating 1/2 revolution counter clockwise.");
		uln200xa.setDirection(upm_uln200xa.ULN200XA_DIRECTION_T.ULN200XA_DIR_CCW);
		uln200xa.stepperSteps(2048);

		// turn off the power
		uln200xa.release();
		// ! [Interesting]

		System.out.println("Exiting...");
	}

}
