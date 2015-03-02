/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <string.h>
#include <unistd.h>
#include <iostream>
#include "st7735.h"
#include <signal.h>

int
main(int argc, char **argv)
{
//! [Interesting]
    upm::ST7735 * lcd = new upm::ST7735(7, 4, 9, 8);
    lcd->fillScreen (ST7735_RED);
    lcd->refresh ();

    lcd->fillScreen (ST7735_CYAN);
    lcd->refresh ();

    lcd->fillScreen (ST7735_BLACK);
    lcd->refresh ();

    lcd->drawLine(10, 10, 10, 100, ST7735_MAGENTA);
    lcd->drawLine(20, 20, 10, 100, ST7735_YELLOW);
    lcd->drawLine(30, 30, 50, 100, ST7735_WHITE);
    lcd->refresh ();

    lcd->drawPixel (20, 20, ST7735_GREEN);
    lcd->refresh ();

    lcd->drawTriangle (50, 50, 80, 80, 60, 90, ST7735_GREEN);
    lcd->refresh ();

    lcd->drawCircle (100, 110, 10, ST7735_BLUE);
    lcd->refresh ();


    lcd->setTextWrap(0x0);

    lcd->setCursor(0, 30);
    lcd->setTextColor(ST7735_RED, ST7735_RED);
    lcd->setTextSize(1);
    lcd->print("Hello World!");

    lcd->setCursor(10, 50);
    lcd->setTextColor(ST7735_RED, ST7735_YELLOW);
    lcd->setTextSize(2);
    lcd->print("BIG");

    lcd->refresh ();

    std::cout << "exiting application" << std::endl;

    delete lcd;
//! [Interesting]
    return 0;
}
