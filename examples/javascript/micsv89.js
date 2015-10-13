// Drive the Grive RGB LCD (a JHD1313m1)
//
// The way to drive the LCD directly from
// Javascript code using the i2c interface directly
// This approach is useful for learning about using
// the i2c bus. The i2c file is an implementation
// in Javascript for some of the common LCD functions

// configure jshint
/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */

var upmMICSV89 = require("jsupm_micsv89"); 

var mics = new upmMICSV89.MICSV89(6); 

while(1)
{
    mics.start(); 
    while(!mics.valid());
    console.log("co2: " + mics.co2equ()); 
    console.log("short: " + mics.vocshort()); 
    console.log("tvoc: " + mics.tvoc());   
    console.log("resistor: " + mics.resistor()); 
    console.log("***********************");
}
