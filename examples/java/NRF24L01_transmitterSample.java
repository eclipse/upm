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

import upm_nrf24l01.Callback;

//NOT TESTED!!!
public class NRF24L01_transmitterSample {

	static private final byte[] destAddress = {0x01, 0x01, 0x01, 0x01, 0x01};
	static private final byte[] srcAddress = {0x01, 0x01, 0x01, 0x01, 0x01};
	static private byte[] tx_buffer = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
			0x00};

	static {
		try {
			System.loadLibrary("javaupm_nrf24l01");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_nrf24l01.NRF24L01 comm = new upm_nrf24l01.NRF24L01((short) 7, (short) 8);

		Callback callback = new TransmitterCallback();

		comm.setSourceAddress(srcAddress);
		comm.setDestinationAddress(destAddress);
		comm.setPayload((short) upm_nrf24l01.javaupm_nrf24l01Constants.MAX_BUFFER);
		comm.setChannel((short) 99);
		comm.configure();
		comm.setDataReceivedHandler(callback);

		while (true) {
			comm.send(tx_buffer);
			System.out.println("devi2 :: sending data ....");
			for (int i = 0; i < tx_buffer.length; i++)
				System.out.print(tx_buffer[i]);
			System.out.println();
			Thread.sleep(3000);
		}
		// ! [Interesting]
	}
}

class TransmitterCallback extends Callback {
	public TransmitterCallback() {
		super();
	}
	public void run() {
	}
}