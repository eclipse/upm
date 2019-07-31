/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_waterlevel.WaterLevel;

public class WaterLevelSensor_Example {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//! [Interesting]
		// Instantiating the sensor on Digital Pin 2
		WaterLevel level = new WaterLevel(2);

		while(true){
			if(level.isSubmerged()){
				System.out.println("The sensor is submerged");
			}
			else{
				System.out.println("The sensor is above water level");
			}
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("The following exception occurred: "+e.getMessage());
			}
		}
		//! [Interesting]
	}

}
