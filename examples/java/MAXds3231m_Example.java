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
import upm_maxds3231m.MAXDS3231M;
import upm_maxds3231m.Time3231;

public class MAXds3231m_Example {

	static {
		try {
			System.loadLibrary("javaupm_maxds3231m");
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
		MAXDS3231M sensor = new MAXDS3231M(upm_maxds3231m.javaupm_maxds3231mConstants.ADDR);
		Time3231 t = new Time3231();

		t.setSecond((short)1);
		t.setMinute((short)3);
		t.setHour((short)3);
		t.setDay((short)3);
		t.setMonth((short)3);
		t.setYear((short)3);
		t.setWeekDay((short)3);

		sensor.setDate(t);

		try {
			Thread.sleep(500);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			System.out.println("The following exception occurred: "+e.getMessage());
		}

		while(true){
			if(sensor.getDate(t)){
				System.out.println(t.getHour()+":"+t.getMinute()+":"+t.getSecond());
			}
			System.out.println("Temperature: "+sensor.getTemperature());
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println("The following exception occcured: "+e.getMessage());
			}
		}
		//! [Interesting]
	}

}