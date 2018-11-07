/*
 * Author: Stefan Andritoiu <stefan.andritoiu@gmail.com>
 * Copyright (c) 2017 Intel Corporation.
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

// NOT TESTED!!!
public class PN532_writeurl_Example {

	static private final String url = "iotdk.intel.com";

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate an PN532 on I2C bus 0 (default) using gpio 3 for the
		// IRQ, and gpio 2 for the reset pin.

		upm_pn532.PN532 nfc = new upm_pn532.PN532(3, 2);

		if (!nfc.init())
			System.out.println("init() failed");

		long vers = nfc.getFirmwareVersion();

		if (vers != 0)
			System.out.println("Got firmware version: " + String.format("0x%08x", vers));
		else {
			System.out.println("Could not identify PN532\n");
			return;
		}

		// Now scan and identify any cards that come in range (1 for now)

		// Retry forever
		nfc.setPassiveActivationRetries((short) 0xff);

		nfc.SAMConfig();

		short[] uidSize = new short[1];
		byte[] uid = new byte[7];

		boolean foundCard = false;
		while (!foundCard) {
			if (nfc.readPassiveTargetID(PN532.BAUD_T.BAUD_MIFARE_ISO14443A, uid, uidSize, 2000)) {
				// found a card
				System.out.println("Found a card: UID len " + uidSize[0]);
				System.out.print("UID: ");

				for (int i = 0; i < uidSize[0]; i++)
					System.out.print(String.format("%02x", uid[i]) + " ");

				System.out.println("\nSAK: " + String.format("0x%02x", nfc.getSAK()));
				System.out.println("ATQA: " + String.format("0x%04x\n\n", nfc.getATQA()));

				foundCard = true;
			} else {
				System.out.println("Waiting for a card...");
			}
		}

		if (uidSize[0] != 7) {
			System.out.println("This example will only write an NDEF URI to preformatted");
			System.out.println("Mifare Ultralight or NTAG2XX tags\n");
			return;
		}

		// 48 bytes is maximum data area on ultralight cards, so we use that
		// as the maximum datasize here. Obviously if you have a bigger
		// card, you can write more data.
		if (!nfc.ntag2xx_WriteNDEFURI(PN532.NDEF_URI_T.NDEF_URIPREFIX_HTTP, url, (short) 48)) {
			// failure
			System.out.println("Failed to write NDEF record tag.\n");
			return;
		}

		System.out.println("Success, URL record written to tag.\n");

		// ! [Interesting]
	}
}
