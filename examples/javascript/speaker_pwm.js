/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

//Load Speaker module
var Speaker = require('jsupm_speaker');
// Instantiate a Speaker (PWM) pin D3 in PWM mode
var speaker = new Speaker.Speaker(3, true);

// emit a series of frequencies for 500ms each
speaker.emit(50, 500);
speaker.emit(75, 500);
speaker.emit(125, 500);
speaker.emit(250, 500);
speaker.emit(500, 500);
speaker.emit(1000, 500);
speaker.emit(2000, 500);
speaker.emit(3000, 500);
speaker.emit(5000, 500);
speaker.emit(10000, 500);

// Print message when exiting
process.on('SIGINT', function()
{
	console.log("Exiting...");
	process.exit(0);
});
