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


var sensorObj = require('jsupm_h803x');


/************** Main code **************/

var defaultDev = "/dev/ttyUSB0";

// if an argument was specified, use it as the device instead
if (process.argv.length > 2)
{
    defaultDev = process.argv[2];
}

console.log("Using device " + defaultDev);
console.log("Initializing...");

// Instantiate an H803X instance, using MODBUS slave address 1, and
// default comm parameters (9600, 8, N, 2)
var sensor = new sensorObj.H803X(defaultDev, 1);

// output the Slave ID (manufacturer, model, serno)
console.log("Slave ID:", sensor.getSlaveID());

console.log("");

// update and print available values every second
setInterval(function()
{
    // update our values from the sensor
    sensor.update();

    // H8035 / H8036
    console.log("Consumption (kWh):", sensor.getConsumption());
    console.log("Real Power (kW):", sensor.getRealPower());

    if (sensor.isH8036())
    {
        // The H8036 has much more data available...

          console.log("Reactive Power (kVAR):", sensor.getReactivePower());
          console.log("Apparent Power (kVA):", sensor.getApparentPower());
          console.log("Power Factor:", sensor.getPowerFactor());
          console.log("Volts Line to Line:", sensor.getVoltsLineToLine());
          console.log("Volts Line to Neutral:", sensor.getVoltsLineToNeutral());

          console.log("Current:", sensor.getCurrent());

          console.log("Real Power Phase A (kW):", sensor.getRealPowerPhaseA());
          console.log("Real Power Phase B (kW):", sensor.getRealPowerPhaseB());
          console.log("Real Power Phase C (kW):", sensor.getRealPowerPhaseC());

          console.log("Power Factor Phase A:", sensor.getPowerFactorPhaseA());
          console.log("Power Factor Phase B:", sensor.getPowerFactorPhaseB());
          console.log("Power Factor Phase C:", sensor.getPowerFactorPhaseC());

          console.log("Volts Phase A to B:", sensor.getVoltsPhaseAToB());
          console.log("Volts Phase B to C:", sensor.getVoltsPhaseBToC());
          console.log("Volts Phase A to C:", sensor.getVoltsPhaseAToC());
          console.log("Volts Phase A to Neutral: ",
                      sensor.getVoltsPhaseAToNeutral());
          console.log("Volts Phase B to Neutral: ",
                      sensor.getVoltsPhaseBToNeutral());
          console.log("Volts Phase C to Neutral: ",
                      sensor.getVoltsPhaseCToNeutral());

          console.log("Current Phase A:", sensor.getCurrentPhaseA());
          console.log("Current Phase B:", sensor.getCurrentPhaseB());
          console.log("Current Phase C:", sensor.getCurrentPhaseC());

          console.log("Avg Real Power (kW):", sensor.getAvgRealPower());
          console.log("Min Real Power (kW):", sensor.getMinRealPower());
          console.log("Max Real Power (kW):", sensor.getMaxRealPower());
    }

    console.log("");

}, 2000);


process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log("Exiting...");
    process.exit(0);
});
