/*
 * Author: Abhishek Malik <abhishek.malik@intel.com>
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

import upm_at42qt1070.AT42QT1070;

public class GroveQTouch {

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
				e.printStackTrace();
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
