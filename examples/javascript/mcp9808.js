/*
 * The MIT License 
 *
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation.
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

var mcp = require('jsupm_mcp9808');

var temp = new mcp.MCP9808(6); 

console.log(temp.getTemp());

//Sleep mode: 
temp.shutDown();
//wake up 
temp.shutDown(false); 
//set mode to report fahrenheit 
temp.setMode(false); 
//check reporting mode
console.log(temp.isCelsius())  //False = fahrenheit
//set mode to celsius
temp.setMode(); 
//read temp
temp.getTemp();
//check Tcrit, Tupper or Tlower status bits
temp.isTcrit(); // true if over boundry. 
temp.isTupper(); 
temp.isTlower(); 
//set the values of monitior registers
temp.setMonitorReg(mcp.MCP9808.CRIT_TEMP, 23); 
temp.setMonitorReg(mcp.MCP9808.LOWER_TEMP, 20); 
temp.setMonitorReg(mcp.MCP9808.UPPER_TEMP, 22); 
//read MonitorReg
console.log("Tcrit = " + temp.getMonitorReg(mcp.MCP9808.CRIT_TEMP)); 
console.log("Tcrit = " + temp.getMonitorReg(mcp.MCP9808.LOWER_TEMP)); 
console.log("Tcrit = " + temp.getMonitorReg(mcp.MCP9808.UPPER_TEMP)); 

    
process.exit(0); 






                      