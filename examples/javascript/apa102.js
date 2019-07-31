/*
* Author:  Yannick Adam <yannick.adam@gmail.com>
* Copyright (c) 2016 Yannick Adam
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var lib = require('jsupm_apa102');

// Instantiate a strip of 30 LEDs on SPI Bus 0
var ledStrip = new lib.APA102(30, 0);

// Set all LEDs to blue
ledStrip.setAllLeds(31, 0, 0, 255);

// Set a mid-section to red
ledStrip.setLeds(10,20, 31, 255, 0, 0);

// Set a single led to green
ledStrip.setLed(15, 31, 0, 255, 0);


// Exit
ledStrip = null;
process.exit(0);
