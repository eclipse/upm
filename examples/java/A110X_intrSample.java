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

import upm_a110x.IsrCallback;

public class A110X_intrSample {
	
	public static int counter=0;
		
	static {
		try {
			System.loadLibrary("javaupm_a110x");
		}catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		//! [Interesting]
		// Instantiate an A110X sensor on digital pin D2
		upm_a110x.A110X hall = new upm_a110x.A110X(2);

		// This example uses a user-supplied interrupt handler to count
		// pulses that occur when a magnetic field of the correct polarity
		// is detected.  This could be used to measure the rotations per
		// minute (RPM) of a rotor for example.

		IsrCallback callback = new A110XISR();
		hall.installISR(callback);
		
		while(true){
			System.out.println("Counter: " + counter);
			Thread.sleep(1000);
		}
		//! [Interesting]
	}
}

class A110XISR extends IsrCallback {
	public A110XISR(){
		super();
	}
	public void run(){
		A110X_intrSample.counter++;
	}
}