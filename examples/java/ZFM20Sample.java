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
public class ZFM20Sample {

	static {
		try {
			System.loadLibrary("javaupm_zfm20");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a ZFM20 Fingerprint reader on UART 0
		upm_zfm20.ZFM20 fp = new upm_zfm20.ZFM20(0);

		// make sure port is initialized properly. 57600 baud is the default
		if (!fp.setupTty()) {
			System.err.println("Failed to setup tty port parameters");
			System.exit(-1);
		}

		// first, set the default password and address
		fp.setPassword(upm_zfm20.javaupm_zfm20.ZFM20_DEFAULT_PASSWORD);
		fp.setAddress(upm_zfm20.javaupm_zfm20.ZFM20_DEFAULT_ADDRESS);

		// now verify the password. If this fails, any other commands
		// will be ignored, so we just bail.
		if (fp.verifyPassword()) {
			System.out.println("Password verified.");
		} else {
			System.err.println("Password verification failed.");
			System.exit(-1);
		}

		// how many valid stored templates (fingerprints) do we have?
		System.out.println("Total stored templates: " + fp.getNumTemplates());

		// now spin waiting for a fingerprint to successfully image
		System.out.println("Waiting for finger print...");
		while (fp.generateImage() == upm_zfm20.ZFM20.ZFM20_ERRORS_T.ERR_NO_FINGER.swigValue());

		// in theory, we have an image
		System.out.println("Image captured, converting...");
		short rv = fp.image2Tz(1);
		if (rv != upm_zfm20.ZFM20.ZFM20_ERRORS_T.ERR_OK.swigValue()) {
			System.err.println("Image conversion failed with error code " + rv);
			System.exit(-1);
		}
		System.out.println("Image conversion succeeded");

		// we search for a print matching slot 1, where we shored our last
		// converted fingerprint
		int[] id = new int[1];
		int[] score = new int[1];
		rv = fp.search(1, id, score);
		if (rv != upm_zfm20.ZFM20.ZFM20_ERRORS_T.ERR_OK.swigValue()) {
			if (rv == upm_zfm20.ZFM20.ZFM20_ERRORS_T.ERR_FP_NOTFOUND.swigValue()) {
				System.out.println("Fingerprint not found");
				System.exit(0);
			} else {
				System.err.println("Search failed with error code " + rv);
				System.exit(-1);
			}
		}

		System.out.println("Fingerprint found!");
		System.out.println("ID: " + id + ", Score: " + score);

		// ! [Interesting]
	}
}