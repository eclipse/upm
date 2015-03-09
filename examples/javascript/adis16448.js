//////////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Submit Date: 03/09/2015
// Author: Juan Jose Chong <juanjchong@gmail.com>
// Copyright (c) 2015 Juan Jose Chong
//
//////////////////////////////////////////////////////////////////////////////////////
// adis16448.js
//////////////////////////////////////////////////////////////////////////////////////
//
// This example code runs on an Intel Edison and uses mraa to acquire data
// from an ADIS16448. This data is then scaled and printed onto the terminal.
// 
// This software has been tested to connect to an ADIS16448 through a level shifter
// such as the TI TXB0104. The SPI lines (DIN, DOUT, SCLK, /CS) are all wired through
// the level shifter and the ADIS16448 is also being powered by the Intel Edison.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////////////////

//Call the ADIS16448 library
var adis16448 = require('jsupm_adis16448');

//Instantiate SPI and Reset
var imu = new adis16448.ADIS16448(0,3);

periodicActivity(); //Call the periodicActivity function

function periodicActivity()
{   
    //Read & Scale Gyro/Accel Data
    var xgyro = imu.gyroScale(imu.regRead(0x04));
    var ygyro = imu.gyroScale(imu.regRead(0x06));
    var zgyro = imu.gyroScale(imu.regRead(0x08));
    var xaccl = imu.accelScale(imu.regRead(0x0A)); 
    var yaccl = imu.accelScale(imu.regRead(0x0C));
    var zaccl = imu.accelScale(imu.regRead(0x0E));
    
    //Display Scaled Data on the Console Log
    console.log('XGYRO: ' + xgyro);
    console.log('YGYRO: ' + ygyro);
    console.log('ZGYRO: ' + zgyro);
    console.log('XACCL: ' + xaccl);
    console.log('YACCL: ' + yaccl);
    console.log('ZACCL: ' + zaccl);
    console.log(' ');
    setTimeout(periodicActivity,200); //call the indicated function after 0.2 seconds (200 milliseconds)
}