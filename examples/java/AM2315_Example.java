/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
import upm_am2315.AM2315;

public class AM2315_Example {

	static {
		try {
			System.loadLibrary("javaupm_am2315");
			System.loadLibrary("mraajava");
		} catch (UnsatisfiedLinkError e) {
			System.err.println(
					"Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\n" +
							e);
			System.exit(1);
		}
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//! [Interesting]
		float humidity = 0;
		float temperature = 0;
		
		// Instantiate the sensor
		AM2315 sensor = new AM2315(0, upm_am2315.javaupm_am2315Constants.AM2315_I2C_ADDRESS);
		sensor.testSensor();
		
		while(true){
			// retrieving the humidity and temperature values
			humidity = sensor.getHumidity();
			temperature = sensor.getTemperature();
			
			System.out.println("Temperature : "+temperature+" Humidity: "+humidity);
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
