/*jslint node:true, vars:true, bitwise:true, unparam:true */
/*jshint unused:true */
/*
* Author: Zion Orent <zorent@ics.com>
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

var collision_lib = require("jsupm_grovecollision");

// The was tested with the Grove Collision Sensor
// Instantiate a Grove Collision on digital pin D2
var collision_obj = new collision_lib.GroveCollision(2);

var collisionState = false;
console.log("No collision");

// Having an infinate loop prevents nodeJS from catching Cntl-C
// We need to catch Cntl-C to clean up memory
// Instead, we check the collision sensor every millisecond
var myInterval = setInterval(function()
{
	if (collision_obj.isColliding() && !collisionState)
	{
		console.log("Collision!");
		collisionState = true;
	}
	else if (!collision_obj.isColliding() && collisionState)
	{
		console.log("No collision");
		collisionState = false;
	}
}, 1);

// When exiting: clear interval, clean up memory, and print message
process.on('SIGINT', function()
{
	clearInterval(myInterval);
	collision_obj = null;
	collision_lib.cleanUp();
	collision_lib = null;
	console.log("Exiting...");
	process.exit(0);
});
