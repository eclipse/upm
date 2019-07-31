/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

import upm_at42qt1070.AT42QT1070;

public class GroveQTouch_Example {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
		// Instantiating the Q Touch sensor on the I2C bus 0
		AT42QT1070 touch = new AT42QT1070(0, (short) 0x1b);
		short pressedButtons;

		// ! [Interesting]
		while(true){
			touch.updateState();
			printButtons(touch.getButtons());
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				System.out.println(e.toString());
			}
		}
		// ! [Interesting]
	}

	/**
	 * This function prints out the button numbers from 0 through 6
	 * @param buttonNumber
	 */
	public static void printButtons(int buttonNumber){
		boolean buttonPressed = false;
		System.out.print("Button Pressed: ");
		for(int i=0;i<7;i++){
			if((buttonNumber & (1<<i)) != 0){
				System.out.println(i+" ");
				buttonPressed = true;
			}
		}



		if(!buttonPressed){
			System.out.println("None ");
		}
	}
}
