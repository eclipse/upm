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
import upm_sm130.SM130;

public class SM130_Example {

	static {
		try {
			System.loadLibrary("javaupm_sm130");
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
		// Instantiate a UART based SM130 RFID Module using defaults
		SM130 sensor = new SM130();
		
		// set the baud rate.  19200 baud is the default.
		if(sensor.setBaudRate(19200) == 1){
			System.out.println("Failed to set baud rate");
			return;
		}
		
		System.out.println("Resetting...");
		sensor.reset();
		
		System.out.println("Firmware revision: "+sensor.getFirmwareVersion());
		System.out.println("Waiting up to 5 seconds for a tag...");
		
		if(sensor.waitForTag(5000)){
			System.out.println("Found tag, UID: "+sensor.string2HexString(sensor.getUID()));
			System.out.println("Tag Type: "+sensor.tag2String(sensor.getTagType()));
		}
		else{
			// error
			System.out.println("waitForTag failed: "+sensor.getLastErrorString());
		}
		//! [Interesting]
	}

}