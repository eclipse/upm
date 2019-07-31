/*
* Author: Keelan Lightfoot <keelanlightfoot@gmail.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
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
