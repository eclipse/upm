/*
 * Author: Stefan Andritoiu <stefan.andritoiu@intel.com>
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

//NOT TESTED!!!
public class ST7735Sample {

	static {
		try {
			System.loadLibrary("javaupm_st7735");
		} catch (UnsatisfiedLinkError e) {
			System.err.println("error in loading native library");
			System.exit(-1);
		}
	}

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