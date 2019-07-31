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

#include <iostream>
#include <stdio.h>

#include "m24lr64e.hpp"

using namespace std;
using namespace upm;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate an M24LR64E on I2C bus 0

    upm::M24LR64E nfcTag(M24LR64E_I2C_BUS);

    // This example accesses the device in the 'user' (default) mode,
    // reads the last byte of data in the EEPROM, inverts it, writes
    // it back, and then re-reads it.

    // Read the last byte of the EEPROM area

    int addr = M24LR64E::EEPROM_I2C_LENGTH - 1;
    printf("Address: %d\n", addr);
    uint8_t byte = nfcTag.readByte(addr);
    printf("Read byte: %02x\n", byte);

    // Now change it to it's opposite and write it
    byte = ~byte;
    nfcTag.writeByte(addr, byte);
    printf("Wrote inverted byte: %02x\n", byte);

    // Now read it back.
    byte = nfcTag.readByte(addr);
    printf("Read byte: %02x\n", byte);

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
