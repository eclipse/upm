/*
 * Author: Stefan Andritoiu <stefan.andritoiu@gmail.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class RHUSB_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		String defaultDev = "/dev/ttyUSB0";

		// if an argument was specified, use it as the device instead
		if (args.length > 0)
			defaultDev = args[0];

		System.out.println("Using device " + defaultDev);
		System.out.println("Initializing...");

		// Instantiate an RHUSB instance on defaultDev
		upm_rhusb.RHUSB sensor = new upm_rhusb.RHUSB(defaultDev);

		// output the firmware ID
		System.out.println("Firmware ID: " + sensor.getFirmwareID());

		// update and print available values every second
		while (true) {
			// update our values from the sensor
			sensor.update();

			// we show both C and F for temperature
			System.out.println("Temperature: " + sensor.getTemperature() + " C / " + sensor.getTemperature(true) + " F");

			System.out.println("Humidity: " + sensor.getHumidity() + " %");

			Thread.sleep(1000);
		}

		// ! [Interesting]
	}
}
