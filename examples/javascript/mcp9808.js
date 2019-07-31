/*
 * The MIT License 
 *
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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






                      