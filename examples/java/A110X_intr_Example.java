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

public class A110X_intr_Example {
	
	private static int counter=0;
    
    public static void incrementCounter() {
        counter++;
    }	
	public static void main(String[] args) throws InterruptedException {
		//! [Interesting]
		// Instantiate an A110X sensor on digital pin D2
		upm_a110x.A110X hall = new upm_a110x.A110X(2);

		// This example uses a user-supplied interrupt handler to count
		// pulses that occur when a magnetic field of the correct polarity
		// is detected.  This could be used to measure the rotations per
		// minute (RPM) of a rotor for example.

		A110XISR callback = new A110XISR();
		hall.installISR(callback);
		
		while(true){
			System.out.println("Counter: " + counter);
			Thread.sleep(1000);
		}
		//! [Interesting]
	}
}

class A110XISR implements Runnable {
	public A110XISR(){
		super();
	}
	public void run(){
		A110X_intr_Example.incrementCounter();
	}
}