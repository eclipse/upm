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
public class NRF24L01_receiver_Example {

	static private final byte[] local_address = {0x01, 0x01, 0x01, 0x01, 0x01};
	static private final byte[] broadcast_address = {(byte) 0xFF, (byte) 0xFF, (byte) 0xFF,
			(byte) 0xFF, (byte) 0xFF};

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