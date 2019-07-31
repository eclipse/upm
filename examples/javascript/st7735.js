/*
* Author:  Takahiro Poly Horikawa <horikawa.takahiro@gmail.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/
var st7735 = require('jsupm_st7735');

var lcd = new st7735.ST7735(10, 7, 9, 8);
lcd.fillScreen(st7735.ST7735_RED);
lcd.refresh();

lcd.fillScreen(st7735.ST7735_CYAN);
lcd.refresh();

lcd.fillScreen(st7735.ST7735_BLACK);
lcd.refresh();

lcd.drawLine(10, 10, 10, 100, st7735.ST7735_MAGENTA);
lcd.drawLine(20, 20, 10, 100, st7735.ST7735_YELLOW);
lcd.drawLine(30, 30, 10, 100, st7735.ST7735_WHITE);
lcd.refresh();

lcd.drawPixel(20, 20, st7735.ST7735_GREEN);
lcd.refresh();

lcd.drawTriangle(50, 50, 80, 80, 60, 90, st7735.ST7735_GREEN);
lcd.refresh();

lcd.drawCircle(100, 110, 10, st7735.ST7735_BLUE);
lcd.refresh();

lcd.setTextWrap(0x0);

lcd.setCursor(0, 30);
lcd.setTextColor(st7735.ST7735_RED, st7735.ST7735_RED);
lcd.setTextSize(1);
lcd.print('Hello World!');

lcd.setCursor(10, 50);
lcd.setTextColor(st7735.ST7735_RED, st7735.ST7735_RED);
lcd.setTextSize(2);
lcd.print('BIG');

lcd.refresh();

