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
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "pn532.hpp"

using namespace std;

// the URL we want to add as an NDEF record
// NOTE: this cannot exceed 34 characters.
static char url[] = "iotdk.intel.com";

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate an PN532 on I2C bus 0 (default) using gpio 3 for the
    // IRQ, and gpio 2 for the reset pin.

    upm::PN532 nfc(3, 2);

    if (!nfc.init())
        cerr << "init() failed" << endl;

    uint32_t vers = nfc.getFirmwareVersion();

    if (vers)
        printf("Got firmware version: 0x%08x\n", vers);
    else {
        printf("Could not identify PN532\n");
        return 1;
    }

    // Now scan and identify any cards that come in range (1 for now)

    // Retry forever
    nfc.setPassiveActivationRetries(0xff);

    nfc.SAMConfig();

    uint8_t uidSize;
    uint8_t uid[7];

    bool foundCard = false;
    while (!foundCard) {
        memset(uid, 0, 7);
        if (nfc.readPassiveTargetID(nfc.BAUD_MIFARE_ISO14443A, uid, &uidSize, 2000)) {
            // found a card
            printf("Found a card: UID len %d\n", uidSize);
            printf("UID: ");
            for (int i = 0; i < uidSize; i++)
                printf("%02x ", uid[i]);
            printf("\n");
            printf("SAK: 0x%02x\n", nfc.getSAK());
            printf("ATQA: 0x%04x\n\n", nfc.getATQA());
            foundCard = true;
        } else {
            printf("Waiting for a card...\n");
        }
    }

    if (uidSize != 7) {
        printf("This example will only write an NDEF URI to preformatted\n");
        printf("Mifare Ultralight or NTAG2XX tags\n");

        return 1;
    }

    // 48 bytes is maximum data area on ultralight cards, so we use that
    // as the maximum datasize here.  Obviously if you have a bigger
    // card, you can write more data.
    if (!nfc.ntag2xx_WriteNDEFURI(nfc.NDEF_URIPREFIX_HTTP, url, 48)) {
        // failure
        printf("Failed to write NDEF record tag.\n");
        return 1;
    }

    printf("Success, URL record written to tag.\n");

    //! [Interesting]

    return 0;
}
