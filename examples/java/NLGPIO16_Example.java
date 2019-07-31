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

public class NLGPIO16_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Instantiate a NLGPIO16 Module on the default UART (/dev/ttyACM0)
		upm_nlgpio16.NLGPIO16 sensor = new upm_nlgpio16.NLGPIO16();

		// get the Version
		System.out.println("Device Version: " + sensor.getVersion());
		// read the gpio at pin 3
		System.out.println("GPIO 3 Value: " + sensor.gpioRead(3));
		// read the analog voltage at pin 5
		System.out.println("Analog 5 Voltage: " + sensor.analogReadVolts(5));
		// set the gpio at pin 14 to HIGH
		sensor.gpioSet(14);

		// ! [Interesting]
	}
}