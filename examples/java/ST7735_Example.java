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

//NOT TESTED!!!
public class ST7735_Example {

	public static void main(String[] args) throws InterruptedException {
		// ! [Interesting]
		upm_st7735.ST7735 lcd = new upm_st7735.ST7735((short) 7, (short) 4, (short) 9, (short) 8);

		lcd.fillScreen(upm_st7735.javaupm_st7735Constants.ST7735_RED);
		lcd.refresh();

		lcd.fillScreen(upm_st7735.javaupm_st7735Constants.ST7735_CYAN);
		lcd.refresh();

		lcd.fillScreen(upm_st7735.javaupm_st7735Constants.ST7735_BLACK);
		lcd.refresh();

		lcd.drawLine((short) 10, (short) 10, (short) 10, (short) 100,
				upm_st7735.javaupm_st7735Constants.ST7735_MAGENTA);
		lcd.drawLine((short) 20, (short) 20, (short) 10, (short) 100,
				upm_st7735.javaupm_st7735Constants.ST7735_YELLOW);
		lcd.drawLine((short) 30, (short) 30, (short) 50, (short) 100,
				upm_st7735.javaupm_st7735Constants.ST7735_WHITE);
		lcd.refresh();

		lcd.drawPixel((short) 20, (short) 20, upm_st7735.javaupm_st7735Constants.ST7735_GREEN);
		lcd.refresh();

		lcd.drawTriangle((short) 50, (short) 50, (short) 80, (short) 80, (short) 60, (short) 90,
				upm_st7735.javaupm_st7735Constants.ST7735_GREEN);
		lcd.refresh();

		lcd.drawCircle((short) 100, (short) 110, (short) 10,
				upm_st7735.javaupm_st7735Constants.ST7735_BLUE);
		lcd.refresh();

		lcd.setTextWrap((short) 0);

		lcd.setCursor((short) 0, (short) 30);
		lcd.setTextColor(upm_st7735.javaupm_st7735Constants.ST7735_RED,
				upm_st7735.javaupm_st7735Constants.ST7735_RED);
		lcd.setTextSize((short) 1);
		lcd.print("Hello World!");

		lcd.setCursor((short) 10, (short) 50);
		lcd.setTextColor(upm_st7735.javaupm_st7735Constants.ST7735_RED,
				upm_st7735.javaupm_st7735Constants.ST7735_YELLOW);
		lcd.setTextSize((short) 2);
		lcd.print("BIG");

		lcd.refresh();
		// ! [Interesting]
	}
}