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
public class M24LR64ESample {

	static {
		try {
			System.loadLibrary("javaupm_m24lr64e");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate an M24LR64E on I2C bus 0
		upm_m24lr64e.M24LR64E nfcTag = new upm_m24lr64e.M24LR64E(0);

		// This example accesses the device in the 'user' (default) mode,
		// reads the last byte of data in the EEPROM, inverts it, writes
		// it back, and then re-reads it.

		// Read the last byte of the EEPROM area
		int addr = upm_m24lr64e.M24LR64E.EEPROM_I2C_LENGTH - 1;
		System.out.println("Address: " + addr);
		short read = nfcTag.readByte(addr);
		System.out.println("Read: " + read);

		// Now change it to it's opposite and write it
		read = (short) (~read & 0xff);
		nfcTag.writeByte(addr, read);
		System.out.println("Wrote: " + read);

		// Now read it back
		read = nfcTag.readByte(addr);
		System.out.println("Read: " + read);

		// ! [Interesting]
	}

}