/*
* Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

// Some vars
var colon = true;
var interval;

// Load display
var tm1637 = require('jsupm_tm1637');

// Instantiate on pins 0 (Clk) and 1 (Dio)
var display = new tm1637.TM1637(0, 1);

// Get the current time
var now = new Date();
console.log("System time: " + now.getHours() + ":" + ("0" + now.getMinutes()).slice(-2));
console.log("Time zone can be changed by setting the TZ environment variable.");

// Display and time update function
function update(){
    now = new Date();
    var time = now.getHours().toString() + ("0" + now.getMinutes().toString()).slice(-2);
    display.writeString(time);
    display.setColon(colon = !colon);
}

// Start with a 7-segment encoded box on the display
display.write(0x39, 0x09, 0x09, 0x0f);

// Start displaying the clock after 3 seconds
setTimeout(function(){
    // And update every second thereafter
    interval = setInterval(update, 1000);
}, 3000)

// Exit handler
process.on('SIGINT', function()
{
    clearInterval(interval);
    display = null;
    tm1637.cleanUp();
    tm1637 = null;
    console.log("Interrupt received, exiting...");
    process.exit(0);
});
