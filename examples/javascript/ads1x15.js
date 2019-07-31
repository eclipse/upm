/*
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
var mraa = require('mraa');
var version = mraa.getVersion();

if (version >= 'v0.6.1') {
    console.log('mraa version (' + version + ') ok');
}
else {
    console.log('mraa version(' + version + ') is old - this code may not work');
}

var ADS1X15 = require('jsupm_ads1x15');
var ads1115 = new ADS1X15.ADS1115(1, 0x49); 
var ads1015 = new ADS1X15.ADS1015(1, 0x48); 

setInterval(function(){
    console.log(ads1115.getSample()); 
    console.log(ads1015.getSample()); 
    console.log("*********************"); 
}, 1000); 


