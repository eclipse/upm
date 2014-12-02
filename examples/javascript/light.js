/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */

//Load grove module.
var groveSensor = require('jsupm_grove'); 
//Initialize GroveLight on Analog Pin #1
var light = new groveSensor.GroveLight(1);
//read light value.
setInterval(function()
{
    console.log(light.value());
}, 1000);
