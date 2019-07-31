/*
 * Author: Andrei Vasiliu <andrei.vasiliu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class Speaker_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		// Instantiate a Speaker on digital pin D2
		upm_speaker.Speaker speaker = new upm_speaker.Speaker(2);

		// Play all 7 of the lowest notes
		speaker.playAll();

		// Play a medium C-sharp
		speaker.playSound('c', true, "med");
		// ! [Interesting]
	}

}
