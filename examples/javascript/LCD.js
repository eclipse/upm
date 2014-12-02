/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*global */

var mraa = require('mraa'); //require mraa

//Load i2clcd module
var LCD = require('jsupm_i2clcd');
//Initialize Jhd1313m1 at 0x62 (RGB_ADDRESS) and 0x3E (LCD_ADDRESS) 
var myLcd;
if (mraa.getPlatformType() == 1) // Galileo
    myLcd = new LCD.Jhd1313m1 (0, 0x3E, 0x62);
else if (mraa.getPlatformType() == 2) // Edison
    myLcd = new LCD.Jhd1313m1 (6, 0x3E, 0x62);
else // default
    myLcd = new LCD.Jhd1313m1 (0, 0x3E, 0x62);
myLcd.setCursor(0,0);
// RGB Blue
//myLcd.setColor(53, 39, 249);
// RGB Red
myLcd.setColor(255, 0, 0);
myLcd.write('Hello World now');  
myLcd.close();
