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

import java.util.AbstractList;
import java.lang.Float;

public class H3LIS331DL_Example {
	
	public static void main(String[] args) throws InterruptedException {
		//! [Interesting]
		
		// Instantiate an H3LIS331DL on I2C bus 0
		upm_h3lis331dl.H3LIS331DL sensor = new upm_h3lis331dl.H3LIS331DL(0);
		
		// Initialize the device with default values
		sensor.init();
		
		while(true){
			sensor.update();
			
			AbstractList<Integer> val = sensor.getRawXYZ();
			System.out.println( "Raw: X: " + val.get(0) + " Y: " + val.get(1) + " Z: " + val.get(2)  );
			
			AbstractList<Float> accel = sensor.getAcceleration();
			System.out.println( "Acceleration: X: " + accel.get(0) + " Y: " + accel.get(1) + " Z: " + accel.get(2) );
			
			Thread.sleep(1000);
		}
		//! [Interesting]
	}
}
