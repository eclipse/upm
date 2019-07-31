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

public class Relay_Example {
	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Create the button object using UART
		upm_relay.Relay relay = new upm_relay.Relay(5);

		for (int i = 0; i < 3; i++) {
			relay.on();
			if (relay.isOn())
				System.out.println("Relay is on");
			Thread.sleep(1000);

			relay.off();
			if (relay.isOff())
				System.out.println("Relay is off");
			Thread.sleep(1000);
		}
		// ! [Interesting]
	}
}
