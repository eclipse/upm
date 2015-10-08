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
public class NRF24L01_receiverSample {

	static private final byte[] local_address = {0x01, 0x01, 0x01, 0x01, 0x01};
	static private final byte[] broadcast_address = {(byte) 0xFF, (byte) 0xFF, (byte) 0xFF,
			(byte) 0xFF, (byte) 0xFF};

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

		Callback callback = new ReceiverCallback(comm);

		comm.setSourceAddress(local_address);
		comm.setDestinationAddress(broadcast_address);
		comm.setPayload((short) upm_nrf24l01.javaupm_nrf24l01Constants.MAX_BUFFER);
		comm.configure();
		comm.setSpeedRate(upm_nrf24l01.speed_rate_t.NRF_250KBPS);
		comm.setChannel((short) 99);
		comm.setDataReceivedHandler(callback);

		while (true) {
			comm.pollListener();
		}

		// ! [Interesting]
	}
}

class ReceiverCallback extends Callback {

	private upm_nrf24l01.NRF24L01 comm = null;

	public ReceiverCallback(upm_nrf24l01.NRF24L01 comm) {
		super();
		this.comm = comm;
	}
	public void run() {
		if (comm != null) {
			short[] rx_buffer = comm.getM_rxBuffer();
			System.out.print("Received: ");
			for (int i = 0; i < rx_buffer.length; i++)
				System.out.print(rx_buffer[i]);
			System.out.println();
		} else {
			System.out.println("No NRF24L01 instance given to callback");
		}
	}
}