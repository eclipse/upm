/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var sensorObj = require('jsupm_ims');

// Instantiate a IMS instance using bus 0 and default i2c address
var sensor = new sensorObj.IMS(0);

setInterval(function()
{
    console.log("Version: "
                + sensor.get_version()
                + " light: "
                + sensor.get_light()
                + " moisture: "
                + sensor.get_moisture()
                + " temp: "
                + sensor.get_temperature().toFixed(2)
                + " C");
}, 1000);

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting.");
    process.exit(0);
});
