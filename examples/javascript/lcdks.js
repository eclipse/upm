/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var lcdksObj = require('jsupm_lcdks');

// Instantiate a LCDKS (LCD Keypad Shield) using default pins

// NOTE: The default pins do not include support for a gpio
// controlled backlight.  If you need one, you will need to specify
// all neccessary pins to the constructor.
var lcd = new lcdksObj.LCDKS();

lcd.setCursor(0,0);
lcd.write("LCDKS driver");
lcd.setCursor(1,2);
lcd.write("Hello World");

// output current key value every second.
setInterval(function()
{
    console.log("Button value: " + lcd.getKeyValue());
}, 1000);

// exit on ^C
process.on('SIGINT', function()
{
    lcd = null;
    lcdksObj.cleanUp();
    lcdksObj = null;
    console.log("Exiting.");
    process.exit(0);
});
