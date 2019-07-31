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
public class HMTRP_Example {

	static private final int bufferLength = 255;

	private static void printUsage() {
		System.out.println("Usage:");
		System.out.println("Pass a commandline argument (any argument) to this program");
		System.out.println("to query the radio configuration and output it.  NOTE: the");
		System.out.println("radio must be in CONFIG mode for this to work.");
		System.out.println("Running this program without arguments will simply transmit");
		System.out.println("'Hello World!' every second, and output any data received from");
		System.out.println("another radio.");
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a HMTRP radio device on uart 0
		upm_hmtrp.HMTRP radio = new upm_hmtrp.HMTRP(0);

		// make sure port is initialized properly. 9600 baud is the default.
		if (!radio.setupTty()) {
			System.err.println("Failed to setup tty port parameters");
			System.exit(-1);
		}
		printUsage();

		// By default, this radio simply transmits data sent via writeData()
		// and reads any available data via readData().

		// It can be placed into a configuration mode by grounding the
		// CONFIG pin on the module. When this is done, the various
		// configuration query and config methods can be used. In this
		// example, by default, we just read any data available fom the
		// device, and periodically transmit "Hello World".

		// If any argument was specified on the command line, do a simple
		// configuration query and output the results. The radio must be in
		// CONFIG mode for this to work.

		if (args.length > 0) {
			// config mode
			long[] freq = {0};
			long[] dataRate = {0};
			int[] rxBandwidth = {0};
			short[] modulation = {0};
			short[] txPower = {0};
			long[] uartBaud = {0};

			if (radio.getConfig(freq, dataRate, rxBandwidth, modulation, txPower, uartBaud)) {
				System.out.println("Radio configuration:");
				System.out.println("freq: " + freq[0] + " dataRate: " + dataRate[0]
						+ " rxBandwidth: " + rxBandwidth[0] + "Khz");
				System.out.println("modulation: " + modulation[0] + "Khz txPower: " + txPower[0]
						+ " uartBaud: " + uartBaud[0]);
			} else {
				System.err.println("getConfig() failed.  Make sure the radio is in CONFIG mode.");
			}
		} else {
			// normal read/write mode
			byte[] radioBuffer = new byte[bufferLength];
			byte[] hello = "Hello World".getBytes();
			int counter = 0;

			System.out.println("Running in normal read/write mode.");

			while (true) {
				// we don't want the read to block in this example, so always
				// check to see if data is available first.
				if (radio.dataAvailable()) {
					int rv = radio.readData(radioBuffer);

					if (rv > 0) {
						System.out.print("Received: ");
						for (int i = 0; i < radioBuffer.length; i++)
							System.out.print((char) radioBuffer[i]);
						System.out.println();
					} else {
						System.err.println("Port read error.");
						break;
					}
					continue;
				}

				Thread.sleep(100);

				counter++;
				// every second, transmit "Hello World!"
				if (counter > 10) {
					System.out.println("Transmitting hello world...");
					radio.writeData(hello);
					counter = 0;
				}
			}
		}
		// ! [Interesting]
	}
}
