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

public class MD_Example {
	private static final short speed50 = 127;
	private static final short speed0 = 0;

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate an I2C Motor Driver on I2C bus 0
		upm_md.MD motors = new upm_md.MD();

		// set direction to clockwise (CW) and set speed to 50%
		System.out.println("Spin M1 and M2 at half speed for 3 seconds");
		motors.setMotorDirections(upm_md.MD_DC_DIRECTION_T.MD_DIR_CW,
                                          upm_md.MD_DC_DIRECTION_T.MD_DIR_CW);
		motors.setMotorSpeeds(speed50, speed50);
		Thread.sleep(3000);

		// counter clockwise (CCW)
		System.out.println("Reversing M1 and M2 for 3 seconds");
		motors.setMotorDirections(upm_md.MD_DC_DIRECTION_T.MD_DIR_CCW,
                                          upm_md.MD_DC_DIRECTION_T.MD_DIR_CCW);
		Thread.sleep(3000);

		// stop motors
		System.out.println("Stopping motors");
		motors.setMotorSpeeds(speed0, speed0);
		// ! [Interesting]
	}

}
