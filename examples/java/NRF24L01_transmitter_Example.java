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

import upm_nrf24l01.Callback;

//NOT TESTED!!!
public class NRF24L01_transmitter_Example {

	static private final byte[] destAddress = {0x01, 0x01, 0x01, 0x01, 0x01};
	static private final byte[] srcAddress = {0x01, 0x01, 0x01, 0x01, 0x01};
	static private byte[] tx_buffer = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
			0x00};

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