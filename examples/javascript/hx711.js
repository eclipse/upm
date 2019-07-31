/*
*
* Author: Rafael da Mata Neri <rafael.neri@gmail.com>
* Copyright (c) 2015 Intel Corporation.
*
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

var hx711 = require('jsupm_hx711');
// Instantiate a HX711 data on digital pin D3 and clock on digital pin D2
var scale = new hx711.HX711(3, 2);

setTimeout(function(){
	// 2837: value obtained via calibration
	scale.setScale(2837);
	scale.tare(2);
	setInterval(function(){
		console.log(scale.getUnits());
	}, 1000);
}, 1000);
