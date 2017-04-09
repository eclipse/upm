/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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

var sensorObj = require('jsupm_max30100');

// Instantiate a MAX30100 instance using bus 0
var sensor = new sensorObj.MAX30100(0);

console.log('Oximeter sensor example...');

// Read the temperature and version
console.log ('Temperature: %d C', sensor.temperature());
console.log ('Version: 0x%s', sensor.version().toString(16));

// Set high-res (50 Hz, 16-bit)
sensor.high_res_enable(true);

// Set to sample SpO2
sensor.mode(sensorObj.MAX30100_MODE_SPO2_EN);

// Read individual samples
for (var i = 0; i < 10; i++)
{
    var val = sensor.sample();
    console.log('Single value IR: %d R: %d ', val.IR, val.R);
}

// exit on ^C
process.on('SIGINT', function()
{
    sensor = null;
    sensorObj.cleanUp();
    sensorObj = null;
    console.log('Exiting.');
    process.exit(0);
});
