/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


var sensorObj = require('jsupm_t3311');


/************** Main code **************/

var defaultDev = "/dev/ttyUSB0";

// if an argument was specified, use it as the device instead
if (process.argv.length > 2)
{
    defaultDev = process.argv[2];
}

console.log("Initializing...");

// Instantiate an T3311 instance, using MODBUS slave address 1, and
// default comm parameters (9600, 8, N, 2)
var sensor = new sensorObj.T3311(defaultDev, 1);

// output the serial number and firmware revision
console.log("Serial Number:", sensor.getSerialNumber());
console.log("Firmware Revision:", sensor.getFirmwareMajor() + "." +
            sensor.getFirmwareMinor());
console.log("");

// update and print available values every second
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    // we show both C and F for temperature
    console.log("Temperature:", sensor.getTemperature(),
                "C /", sensor.getTemperature(true), "F");

    console.log("Humidity:", sensor.getHumidity(), "%");

    // this value depends on the sensor configuration -- by default
    // it is the dew point temperature
    console.log("Computed Value:", sensor.getComputedValue());

    // with FW revisions > 2.44, extended computed data is available
    if (sensor.extendedDataAvailable())
    {
        console.log("Dew Point Temperature:", sensor.getDewPointTemperature(),
                    "C /", sensor.getDewPointTemperature(true), "F");

        console.log("Absolute Humidity:", sensor.getAbsoluteHumidity(),
                    "g/m3");

        console.log("Specific Humidity:", sensor.getSpecificHumidity(),
                    "g/kg");

        console.log("Mixing Ratio:", sensor.getMixingRatio(),
                    "g/kg");

        console.log("Specific Enthalpy:", sensor.getSpecificEnthalpy(),
                    "kJ/kg");
    }

    console.log("");

}, 1000);


process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});
