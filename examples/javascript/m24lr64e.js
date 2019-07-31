/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */


var nfcTagObj = require('jsupm_m24lr64e');

// Instantiate a M24LR64E Grove NFC Tag Module on UART 0
var nfcTag = new nfcTagObj.M24LR64E(nfcTagObj.M24LR64E_I2C_BUS);

// This example accesses the device in the 'user' (default) mode,
// reads the last byte of data in the EEPROM, inverts it, writes
// it back, and then re-reads it.

// Read the last byte of the EEPROM area
var addr = (nfcTagObj.M24LR64E.EEPROM_I2C_LENGTH - 1);
console.log("Address: " + addr);
var byte = nfcTag.readByte(addr);

console.log("Read byte: " + byte.toString(16));

// Now change it to it's opposite and write it
byte = (~byte & 0xff);
nfcTag.writeByte(addr, byte);
console.log("Wrote inverted byte: " + byte.toString(16));

// Now read it back.
byte = nfcTag.readByte(addr);
console.log("Read byte: " + byte.toString(16));

// clean up
nfcTag = null;
nfcTagObj.cleanUp();
nfcTagObj = null;
console.log("Exiting...");
process.exit(0);
