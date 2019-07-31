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

#include "zfm20.hpp"

using namespace std;
using namespace upm;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a ZFM20 Fingerprint reader on UART 0

    upm::ZFM20 fp(0);

    // make sure port is initialized properly.  57600 baud is the default.
    if (!fp.setupTty(57600)) {
        cerr << "Failed to setup tty port parameters" << endl;
        return 1;
    }

    // first, set the default password and address
    fp.setPassword(ZFM20_DEFAULT_PASSWORD);
    fp.setAddress(ZFM20_DEFAULT_ADDRESS);

    // now verify the password.  If this fails, any other commands
    // will be ignored, so we just bail.
    if (fp.verifyPassword()) {
        cout << "Password verified." << endl;
    } else {
        cerr << "Password verification failed." << endl;
        return 1;
    }

    // how many valid stored templates (fingerprints) do we have?
    cout << "Total stored templates: " << fp.getNumTemplates() << endl;
    cout << endl;

    // now spin waiting for a fingerprint to successfully image
    cout << "Waiting for finger print..." << endl;

    while (fp.generateImage() == ZFM20::ERR_NO_FINGER)
        ;

    // in theory, we have an image
    cout << "Image captured, converting..." << endl;

    uint8_t rv;
    if ((rv = fp.image2Tz(1)) != ZFM20::ERR_OK) {
        cerr << "Image conversion failed with error code " << int(rv) << endl;
        return 1;
    }

    cout << "Image conversion succeeded." << endl;
    cout << "Searching database..." << endl;

    uint16_t id = 0;
    uint16_t score = 0;

    // we search for a print matching slot 1, where we shored our last
    // converted fingerprint
    if ((rv = fp.search(1, id, score)) != ZFM20::ERR_OK) {
        if (rv == ZFM20::ERR_FP_NOTFOUND) {
            cout << "Finger Print not found" << endl;
            return 0;
        } else {
            cerr << "Search failed with error code " << int(rv) << endl;
            return 1;
        }
    }

    cout << "Fingerprint found!" << endl;
    cout << "ID: " << int(id) << ", Score: " << int(score) << endl;

    //! [Interesting]

    return 0;
}
