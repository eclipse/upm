/*
* Author: Sergey Kiselev <sergey.kiselev@intel.com>
* Copyright (c) 2017 Sergey Kiselev
* Based on the p9813 driver writen by Yannick Adam <yannick.adam@gmail.com>
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var lib = require('jsupm_p9813');

// Instantiate a chain of 2 LEDs using pin 2 for clock and pin 3 for data
var ledChain = new lib.P9813(2, 2, 3);

// Set all LEDs to blue
ledChain.setAllLeds(0, 0, 255);

// Set a single led to green
ledChain.setLed(1, 0, 255, 0);


// Exit
ledChain = null;
process.exit(0);
