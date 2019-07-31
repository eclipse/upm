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

import upm_hp20x.HP20X;

public class HP20x_Example {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		//! [Interesting]
		// Instantiate the sensor on the default I2C bus and sensor
		HP20X hp20x = new HP20X();
		
		// Initializing the sensor with the default values
		hp20x.init();

		while(true){
			System.out.println("Temperature: "+hp20x.getTemperature()+"\tPressure: "+hp20x.getPressure()+"\tAltitude: "+hp20x.getAltitude());
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
