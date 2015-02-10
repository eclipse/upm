/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

var upmTP401 = require('jsupm_gas');
//var time = require('sleep');

//give a qualitative meaning to the value from the sensor
function airQuality(value)
{
    if(value < 50) return "Fresh Air";
    if(value < 200) return "Normal Indoor Air";
    if(value < 400) return "Low Pollution";
    if(value < 600) return "High Pollution - Action Recommended";
    return "Very High Pollution - Take Action Immediately";
}

function loop()
{
    //read values (consecutive reads might vary slightly)
    var value = airSensor.getSample();
    var ppm = airSensor.getPPM();

    //write the sensor values to the console
    console.log("raw: " + value + " ppm: " + (" " + ppm.toFixed(2)).substring(-5, 5) + "   " + airQuality(value));

    //wait 2.5 s then call function again
    setTimeout(loop, 2500);
}

//setup sensor on Analog pin #0 (A0)
var airSensor = new upmTP401.TP401(0);

//warm up sensor
console.log("Sensor is warming up for 3 minutes..");
var i = 1;

//print a message every passing minute
var waiting = setInterval(function() {
        console.log(i++ + " minute(s) passed.");
        if(i == 3) clearInterval(waiting);
    }, 60000);

//start loop in 3 minutes
setTimeout(function(){
    console.log("Sensor is ready!");
    loop();
    }, 180000);
