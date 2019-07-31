/*
 * Author: Zion Orent <zorent@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

// Load lcd module on I2C
var LCD = require('jsupm_jhd1313m1');

// Initialize Jhd1313m1 at 0x62 (RGB_ADDRESS) and 0x3E (LCD_ADDRESS) 
var myLcd = new LCD.Jhd1313m1 (0, 0x3E, 0x62);

myLcd.setCursor(0,0);
// RGB Blue
//myLcd.setColor(53, 39, 249);
// RGB Red
myLcd.setColor(255, 0, 0);
myLcd.write('Hello World');  
myLcd.setCursor(1,2);
myLcd.write('Hello World');
