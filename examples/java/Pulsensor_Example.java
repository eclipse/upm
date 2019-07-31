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

import upm_pulsensor.*;

public class Pulsensor_Example {
	static class PulsensorCallback extends Callback {
		public PulsensorCallback() {
			super();
		}
		public void run (clbk_data arg) {
			System.out.println();
		}
	}

	public static void main (String[] args) throws InterruptedException {
		// ! [Interesting]
		Callback obj_call = new PulsensorCallback();
		Pulsensor p = new Pulsensor(obj_call);
		p.start_sampler();
		Thread.sleep(1000);
		p.stop_sampler();
		// ! [Interesting]
	}
}
