/**
 * Author: Shawn Hymel
 * Copyright (c) 2016 SparkFun Electronics
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

#include <unistd.h>

#include "ili9341.hpp"

int main(int argc, char **argv) {
//! [Interesting]

    // Pins (Edison)
    // CS_LCD   GP44 (MRAA 31)
    // CS_SD    GP43 (MRAA 38) unused
    // DC       GP12 (MRAA 20)
    // RESEST   GP13 (MRAA 14)
    upm::ILI9341 * lcd = new upm::ILI9341(31, 38, 20, 14);

    // Fill the screen with a solid color
    lcd->fillScreen(lcd->color565(0, 40, 16));

    // Draw some shapes
    lcd->drawFastVLine(10, 10, 100, ILI9341_RED);
    lcd->drawFastHLine(20, 10, 50, ILI9341_CYAN);
    lcd->drawLine(160, 30, 200, 60, ILI9341_GREEN);
    lcd->fillRect(20, 30, 75, 60, ILI9341_ORANGE);
    lcd->drawCircle(70, 50, 20, ILI9341_PURPLE);
    lcd->fillCircle(120, 50, 20, ILI9341_PURPLE);
    lcd->drawTriangle(50, 100, 10, 140, 90, 140, ILI9341_YELLOW);
    lcd->fillTriangle(150, 100, 110, 140, 190, 140, ILI9341_YELLOW);
    lcd->drawRoundRect(20, 150, 50, 30, 10, ILI9341_RED);
    lcd->drawRoundRect(130, 150, 50, 30, 10, ILI9341_RED);
    lcd->fillRoundRect(75, 150, 50, 30, 10, ILI9341_RED);

    // Write some text
    lcd->setCursor(0, 200);
    lcd->setTextColor(ILI9341_LIGHTGREY);
    lcd->setTextWrap(true);
    lcd->setTextSize(1);
    lcd->print("Text 1\n");
    lcd->setTextSize(2);
    lcd->print("Text 2\n");
    lcd->setTextSize(3);
    lcd->print("Text 3\n");
    lcd->setTextSize(4);
    lcd->print("Text 4\n");
 
    // Test screen rotation
    for(int r = 0; r < 4; r++) {
        lcd->setRotation(r);
        lcd->fillRect(0, 0, 5, 5, ILI9341_WHITE);
        sleep(1);
    }

    // Invert colors, wait, then revert back
    lcd->invertDisplay(true);
    sleep(2);
    lcd->invertDisplay(false);

    // Don't forget to free up that memory!
    delete lcd;
//! [Interesting]
    return 0;
}