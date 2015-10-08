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

import upm_hcsr04.IsrCallback;

//NOT TESTED!!!
public class HCSR04Sample {

	static {
		try {
			System.loadLibrary("javaupm_hcsr04");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	// ! [Interesting]
	public static void main(String[] args) throws InterruptedException {
		HCSR04ISR callback = new HCSR04ISR();

		upm_hcsr04.HCSR04 sonar = new upm_hcsr04.HCSR04((short) 5, (short) 6, callback);
		callback.setSonar(sonar);
		Thread.sleep(1000);

		while (true) {
			System.out.println("Get distance");
			double distance = sonar.getDistance(upm_hcsr04.javaupm_hcsr04Constants.CM);
			System.out.println("Distance: " + distance);

			Thread.sleep(5000);
		}
	}
}

class HCSR04ISR extends IsrCallback {

	private upm_hcsr04.HCSR04 sonar = null;

	public HCSR04ISR() {
		super();
	}

	public void setSonar(upm_hcsr04.HCSR04 sonar) {
		this.sonar = sonar;
	}

	public void run() {
		if (sonar != null)
			sonar.ackEdgeDetected();
		else
			System.out.println("No HCSR04ISR instance given to callback");
	}
}
// ! [Interesting]