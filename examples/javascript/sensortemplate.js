/*
 * The MIT License (MIT)
 *
 * Author: Your Full Name <your@email.address>
 * Copyright (c) <year> <copyright holder>
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


var jsupm_sensortemplate = require('jsupm_sensortemplate');

// Create an instance of the sensor
var sensor = new upm_sensortemplate.SensorTemplate(0);

loop();

function loop()
{
    // Call a method from the sensortemplate
    console.log("SensorTemplate says: " + sensor.helloWorld());

    // Call loop every 2 seconds
    setTimeout(loop, 2000);
}
