/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class KX122_Example {
	public static void main(String[] args) throws InterruptedException {
		//! [Interesting]
		// Instantiate a KX122
		upm_kx122.KX122 kx122 = new upm_kx122.KX122(0, -1, 24);

		// Reset the device
		kx122.softwareReset();

		// Initialize the device
		kx122.deviceInit(upm_kx122.KX122_ODR_T.KX122_ODR_50,
				upm_kx122.KX122_RES_T.HIGH_RES,
				upm_kx122.KX122_RANGE_T.KX122_RANGE_2G);

		// Print the acceleration
		while(true){
			upm_kx122.floatVector xyz = kx122.getAccelerationDataVector();
			System.out.println("Acceleration: x = " + xyz.get(0)
				+ " y = " + xyz.get(1)
				+ " z = " + xyz.get(2));

			Thread.sleep(1000);
		}
		//! [Interesting]
	}
}
