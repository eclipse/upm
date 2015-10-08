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
public class GROVESCAMSample {

	static {
		try {
			System.loadLibrary("javaupm_grovescam");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Grove Serial Camera on UART 0
		upm_grovescam.GROVESCAM camera = new upm_grovescam.GROVESCAM(0);

		// make sure port is initialized properly. 115200 baud is the default.
		if (!camera.setupTty()) {
			System.err.println("Failed to setup tty port parameters");
			System.exit(-1);
		}

		if (camera.init())
			System.out.println("Initialized...");
		else
			System.out.println("Initialization failed");

		if (camera.preCapture())
			System.out.println("preCapture succeeded...");
		else
			System.out.println("preCapture failed.");

		if (camera.doCapture())
			System.out.println("doCapture succeeded...");
		else
			System.out.println("doCapture failed.");

		if (camera.getImageSize() > 0) {
			System.out.println("Storing image.jpg...");

			if (camera.storeImage("image.jpg"))
				System.out.println("storeImage succeeded...");
			else
				System.out.println("storeImage failed.");

		}
		// ! [Interesting]
	}

}