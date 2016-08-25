/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
import upm_maxds3231m.MAXDS3231M;
import upm_maxds3231m.Time3231;

public class MAXds3231mExample {

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
			System.out.println("The following exception occured: "+e.getMessage());
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