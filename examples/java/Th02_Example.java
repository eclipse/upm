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
import upm_th02.TH02;

public class Th02_Example {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		float temperature = 0;
		float humidity = 0;
		
		TH02 sensor = new TH02();
		while(true){
			temperature = sensor.getTemperature();
			humidity = sensor.getHumidity();
			System.out.println("Temperature = "+temperature+", Humidity = "+humidity);
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("The following exception has occurred: "+e.getMessage());
			}
		}
	}

}
