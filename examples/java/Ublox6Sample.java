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

public class Ublox6Sample {
	private static final int BUFFERLENGTH = 256;

	static {
		try {
			System.loadLibrary("javaupm_ublox6");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Ublox6 GPS device on uart 0
		upm_ublox6.Ublox6 nmea = new upm_ublox6.Ublox6(0);

		// make sure port is initialized properly. 9600 baud is the default.
		if (!nmea.setupTty()) {
			throw new RuntimeException("Failed to setup tty port parameters");
		}

		// Collect and output NMEA data. There are various libraries out on
		// the Internet, that can handle decoding NMEA data and presenting
		// it in a more easily accessible format. This example will just
		// check for, and read raw NMEA data from the device and output it
		// on standard output.

		// This device also supports numerous configuration options, which
		// you can set with writeData(). Please refer to the Ublox-6 data
		// sheet for further information on the formats of the data sent and
		// received, and the various operating modes available.

		byte[] nmeaBuffer = new byte[BUFFERLENGTH];

		while (true) {
			// we don't want the read to block in this example, so always
			// check to see if data is available first.
			if (nmea.dataAvailable()) {
				int rv = nmea.readData(nmeaBuffer);

				if (rv > 0)
					for (int i = 0; i < rv; i++)
						System.out.print((char) nmeaBuffer[i]);

				if (rv < 0) { // some sort of read error occurred
					System.err.println("Port read error.");
					break;
				}
				continue;
			}

			Thread.sleep(1000);
		}
		// ! [Interesting]
	}

}