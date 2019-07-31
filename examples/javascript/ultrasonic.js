/*
 * Author: Jun Kato <i@junkato.jp>
 * Copyright (c) 2015 Jun Kato.
 *
 * Thanks to Seeed Studio for a working arduino sketch
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

var ultrasonic = require("jsupm_ultrasonic");
var sensor = new ultrasonic.UltraSonic(2);

var myInterval = setInterval(function()
{
  var travelTime = sensor.getDistance();
  if (travelTime > 0) {
    var distance = (travelTime / 29 / 2).toFixed(3);
    console.log("distance: " + distance + " [cm]");
  }
}, 200);

// When exiting: clear interval and print message
process.on('SIGINT', function()
{
  clearInterval(myInterval);
  console.log("Exiting...");
  process.exit(0);
});
