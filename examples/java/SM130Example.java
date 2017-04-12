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
import upm_sm130.SM130;

public class SM130Example {

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