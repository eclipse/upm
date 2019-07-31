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
import upm_max5487.MAX5487;

public class MAX5487_Example {

	static {
		try {
			System.loadLibrary("javaupm_max5487");
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
		MAX5487 sensor = new MAX5487(7);

		// Power LED UP
		for(int i=0; i<255; i++){
			sensor.setWiperA((short)i);
			try {
				Thread.sleep(5);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("The following Exception occurred: "+e.getMessage());
			}
		}

		// Power LED DOWN
		for(int i=0; i<255; i++){
			sensor.setWiperA((short)(255 - i));
			try {
				Thread.sleep(5);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("The following exception occurred: "+e.getMessage());
			}
		}

		System.out.println("Exiting...");
		//! [Interesting]
	}

}