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
import upm_gp2y0a.GP2Y0A;

public class Gp2y0a_Example {
	
	public static final float GP2Y0A_AREF 		= 5;
	public static final short SAMPLES_PER_QUERY = 20;

	static {
		try {
			System.loadLibrary("javaupm_gp2y0a");
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
		// Note, for the Grove 80cm version of this sensor, due to the way
		// it is wired, you need to plug this into the A0 port, where it
		// will use the available A1 pin for data.

		// Instantiate a GP2Y0A on analog pin A1
		GP2Y0A gp2y0a = new GP2Y0A(1);

		// The higher the voltage (closer to AREF) the closer the object is.  NOTE:
		// the measured voltage will probably not exceed 3.3 volts.
		// Every second, print the averaged voltage value (averaged over 20 samples).
		while(true){
			System.out.println("AREF: " + GP2Y0A_AREF + "\t, Voltage Value (Higher means Closer): "+ gp2y0a.value(GP2Y0A_AREF, SAMPLES_PER_QUERY));
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