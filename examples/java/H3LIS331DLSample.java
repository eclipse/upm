/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
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

//NOT TESTED!!!
public class H3LIS331DLSample {
	
	static {
		try {
			System.loadLibrary("javaupm_h3lis331dl");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		//! [Interesting]
		int[] val;
		float[] accel;
		
		// Instantiate an H3LIS331DL on I2C bus 0
		upm_h3lis331dl.H3LIS331DL sensor = new upm_h3lis331dl.H3LIS331DL(0);
		
		// Initialize the device with default values
		sensor.init();
		
		while(true){
			sensor.update();
			
			val = sensor.getRawXYZ();
			System.out.println( "Raw: X: " + val[0] + " Y: " + val[1] + " Z: " + val[2]  );
			
			accel = sensor.getAcceleration();
			System.out.println( "Acceleration: X: " + accel[0] + " Y: " + accel[1] + " Z: " + accel[2] );
			
			Thread.sleep(1000);
		}
		//! [Interesting]
	}
}