/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

//NOT TESTED!!!
public class M24LR64E_Example {

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