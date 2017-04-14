/*
* Author: Keelan Lightfoot <keelanlightfoot@gmail.com>
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

/*
* On its own, a i2c switch doesn't really do anything, so the best I can do is
* flips some ports on and off to demonstrate the capabilities of the library.
*/

var tca = require('jsupm_tca9548a');

// Assumes default bus and address
var sw = new tca.TCA9548A(tca.TCA9548A_I2C_BUS,
                          tca.TCA9548A_DEFAULT_ADDR);

// Start with clean slate
sw.disableAllPorts();
console.log("All ports disabled.");
showAllPorts();

// Enable port 4
sw.setPort(4, tca.TCA9548A.ENABLED, tca.TCA9548A.EXCLUSIVE);
console.log("Port 4 enabled with exclusive access.");
showAllPorts();

// Enable port 5
sw.setPort(5, tca.TCA9548A.ENABLED, tca.TCA9548A.INCLUSIVE);
console.log("Port 5 enabled with inclusive access.");
showAllPorts();

// Disable port 5
sw.setPort(5, tca.TCA9548A.DISABLED, tca.TCA9548A.INCLUSIVE);
console.log("Port 5 disabled with inclusive access.");
showAllPorts();

// Enable all ports
sw.enableAllPorts();
console.log("All ports enabled.");
showAllPorts();

// Clean up our mess, in an oblique way.
sw.setPort(4, tca.TCA9548A.DISABLED, tca.TCA9548A.EXCLUSIVE);
console.log("Port 4 disabled with exclusive access.");
showAllPorts();

// fin
process.exit(0);

function showAllPorts() {
    var ports = "";
    for (var i = 0; i<8; i++) {
	ports += (sw.getPort(i)?i:".");
    }
    console.log("Ports: "+ports);
}
