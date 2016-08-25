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
import upm_gp2y0a.GP2Y0A;

public class Gp2y0aExample {
	
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