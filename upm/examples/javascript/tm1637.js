/*
* Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
* Copyright (c) 2015 Intel Corporation.
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
