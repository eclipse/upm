/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */


//Load grove module.
var groveSensor = require('jsupm_grove'); 
//Initialize temperature sensor class on Aio #0
var temp = new groveSensor.GroveTemp(0);

function loop()
{
    console.log(temp.value());
}

setInterval(loop, 1000);
