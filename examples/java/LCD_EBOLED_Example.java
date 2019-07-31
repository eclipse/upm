/*
 * Author: Stefan Andritoiu <stefan.andritoiu@gmail.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

public class LCD_EBOLED_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]

		// Instantiate an Edison Block OLED using default values
		upm_lcd.EBOLED lcd = new upm_lcd.EBOLED();

		lcd.clear();
		lcd.setCursor(10, 15);
		lcd.write("Hello");
		lcd.setCursor(30, 15);
		lcd.write("World!");
		lcd.refresh();

		System.out.println("Sleeping for 5 secconds...");
		Thread.sleep(5000);

		lcd.delete();

		// ! [Interesting]
	}
}
