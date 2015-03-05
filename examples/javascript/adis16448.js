////////////////////////////////////////////////////////////////////////////////////////////////////////
// March 2015
// By: Juan Jose Chong
////////////////////////////////////////////////////////////////////////////////////////////////////////
// adis16448.js
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This example interfaces the ADIS16448 IMU with an Intel Edison. 
//
// This example is free software. You can redistribute it and/or modify it
// under the terms of the GNU Lesser Public License as published by the Free Software
// Foundation, either version 3 of the License, or any later version.
//
// This example is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more details.
//
// You should have received a copy of the GNU Lesser Public License along with
// this example. If not, see <http://www.gnu.org/licenses/>.
//
// This library is based on the ADIS16480 library written by Daniel Tatum.
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

//Call the ADIS16448 library
var adis16448 = require('jsupm_adis16448');

//Instantiate SPI and Reset
var imu = new adis16448.ADIS16448(0,3);

periodicActivity(); //call the periodicActivity function

function periodicActivity() //
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