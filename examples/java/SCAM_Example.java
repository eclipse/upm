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
public class SCAM_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Serial Camera on UART 0
		upm_scam.SCAM camera = new upm_scam.SCAM(0);

		// make sure port is initialized properly. 115200 baud is the default.
		if (!camera.setupTty()) {
			System.err.println("Failed to setup tty port parameters");
			System.exit(-1);
		}

		if (camera.init())
			System.out.println("Initialized...");
		else
			System.out.println("Initialization failed");

		if (camera.preCapture())
			System.out.println("preCapture succeeded...");
		else
			System.out.println("preCapture failed.");

		if (camera.doCapture())
			System.out.println("doCapture succeeded...");
		else
			System.out.println("doCapture failed.");

		if (camera.getImageSize() > 0) {
			System.out.println("Storing image.jpg...");

			if (camera.storeImage("image.jpg"))
				System.out.println("storeImage succeeded...");
			else
				System.out.println("storeImage failed.");

		}
		// ! [Interesting]
	}

}
