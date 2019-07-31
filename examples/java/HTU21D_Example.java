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

public class HTU21D_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_htu21d.HTU21D sensor = new upm_htu21d.HTU21D(0);
		sensor.testSensor();

		while (true) {
			sensor.sampleData();

			System.out.println("Humidity: " + sensor.getHumidity() + "%");
			System.out.println("Compensated RH: " + sensor.getCompRH() + "%");
			System.out.println("Temperature: " + sensor.getTemperature() + "C");
			System.out.println("Dew Point: " + sensor.getDewPoint() + "C");

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}
