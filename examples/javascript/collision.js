/*
* Author: Zion Orent <zorent@ics.com>
* Copyright (c) 2015 Intel Corporation.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var collision_lib = require("jsupm_collision");

// The was tested with the Collision Sensor
// Instantiate a Collision on digital pin D2
var collision_obj = new collision_lib.Collision(2);

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
