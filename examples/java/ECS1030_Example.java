/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
import upm_ecs1030.ECS1030;

public class ECS1030_Example {

	static {
		try {
			System.loadLibrary("javaupm_ecs1030");
			System.loadLibrary("mraajava");
		} catch (UnsatisfiedLinkError e) {
			System.err.println(
					"Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\n" +
							e);
			System.exit(1);
		}
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		//! [Interesting]
		ECS1030 sensor = new ECS1030((short)0);
		
		while(true){
			System.out.println("I = "+sensor.getCurrency_A()+", Power = "+sensor.getPower_A());
			System.out.println("I = "+sensor.getCurrency_B()+", Power = "+sensor.getPower_B());
		}
		//! [Interesting]
	}

}