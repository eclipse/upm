/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>

#include "st7735.hpp"

int
main(int argc, char** argv)
{
    //! [Interesting]
    upm::ST7735 lcd(7, 4, 9, 8);
    lcd.fillScreen(ST7735_RED);
    lcd.refresh();

    lcd.fillScreen(ST7735_CYAN);
    lcd.refresh();

    lcd.fillScreen(ST7735_BLACK);
    lcd.refresh();

    lcd.drawLine(10, 10, 10, 100, ST7735_MAGENTA);
    lcd.drawLine(20, 20, 10, 100, ST7735_YELLOW);
    lcd.drawLine(30, 30, 50, 100, ST7735_WHITE);
    lcd.refresh();

    lcd.drawPixel(20, 20, ST7735_GREEN);
    lcd.refresh();

    lcd.drawTriangle(50, 50, 80, 80, 60, 90, ST7735_GREEN);
    lcd.refresh();

    lcd.drawCircle(100, 110, 10, ST7735_BLUE);
    lcd.refresh();

    lcd.setTextWrap(0x0);

    lcd.setCursor(0, 30);
    lcd.setTextColor(ST7735_RED, ST7735_RED);
    lcd.setTextSize(1);
    lcd.print("Hello World!");

    lcd.setCursor(10, 50);
    lcd.setTextColor(ST7735_RED, ST7735_YELLOW);
    lcd.setTextSize(2);
    lcd.print("BIG");

    lcd.refresh();

    std::cout << "exiting application" << std::endl;

    //! [Interesting]
    return 0;
}
