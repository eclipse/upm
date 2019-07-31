/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


var sensorObj = require('jsupm_ozw');


/************** Main code **************/
// Instantiate an OZW instance
var sensor = new sensorObj.OZWDUMP();

var defaultDev = "/dev/ttyACM0";

// if an argument was specified, use it as the device instead
if (process.argv.length > 2)
{
    defaultDev = process.argv[2];
}

// The first thing to do is create options, then lock them when done.
sensor.optionsCreate();
sensor.optionsLock();

// Next, initialize it.
console.log("Initializing, this may take awhile depending on your ZWave network");

sensor.init(defaultDev);
console.log("Initialization complete");

console.log("Dumping nodes...");

sensor.dumpNodes(true);

// The following is example output of dumpNodes:
//
// Dumping nodes...
// Node 1: Z-Stick Gen5
// Node 2: Smart Switch 6
//    Index:  0, Type: bool, Label: Switch, Value: False
//    Index:  2, Type: float, Label: Energy, Value: 1.190 kWh
//    Index:  3, Type: float, Label: Previous Reading, Value: 1.190 kWh
//    Index:  4, Type: int32, Label: Interval, Value: 1521 seconds
//    Index:  5, Type: float, Label: Power, Value: 0.000 W
//    Index:  6, Type: float, Label: Voltage, Value: 121.256 V
//    Index:  7, Type: float, Label: Current, Value: 0.000 A
//    Index:  8, Type: bool, Label: Exporting, Value: False
//    Index: 45, Type: list, Label: Day, Value: Friday
//    Index: 46, Type: byte, Label: Hour, Value: 5
//    Index: 47, Type: byte, Label: Minute, Value: 53
// Node 3: Multi Sensor
//    Index:  0, Type: bool, Label: Sensor, Value: True
//    Index:  1, Type: float, Label: Temperature, Value: 72.8 F
//    Index:  2, Type: float, Label: Luminance, Value: 4 lux
//    Index:  3, Type: float, Label: Relative Humidity, Value: 22 %
//    Index: 17, Type: byte, Label: Battery Level, Value: 98 %
// Node 5: Minimote
// Node 6: Smart Energy Switch
//    Index:  0, Type: bool, Label: Switch, Value: False
//    Index:  2, Type: float, Label: Power, Value: 0.000 W
//    Index:  3, Type: float, Label: Energy, Value: 1.609 kWh
//    Index:  4, Type: float, Label: Previous Reading, Value: 1.609 kWh
//    Index:  5, Type: int32, Label: Interval, Value: 1521 seconds
//    Index:  6, Type: float, Label: Power, Value: 0.000 W
//    Index:  7, Type: float, Label: Previous Reading, Value: 1.609 W
//    Index:  8, Type: int32, Label: Interval, Value: 1521 seconds
//    Index:  9, Type: bool, Label: Exporting, Value: False
// Node 7: Smart Energy Switch
//    Index:  0, Type: bool, Label: Switch, Value: False
//    Index:  2, Type: float, Label: Power, Value: 0.000 W
//    Index:  3, Type: float, Label: Energy, Value: 0.000 kWh
//    Index:  4, Type: float, Label: Previous Reading, Value: 0.000 kWh
//    Index:  5, Type: int32, Label: Interval, Value: 1521 seconds
//    Index:  6, Type: float, Label: Power, Value: 0.000 W
//    Index:  7, Type: float, Label: Previous Reading, Value: 0.000 W
//    Index:  8, Type: int32, Label: Interval, Value: 1521 seconds
//    Index:  9, Type: bool, Label: Exporting, Value: False
//

sensor = null;
sensorObj.cleanUp();
sensorObj = null;
console.log("Exiting...");
process.exit(0);
