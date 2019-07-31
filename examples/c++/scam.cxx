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

#include "scam.hpp"

using namespace std;
using namespace upm;

int
main(int argc, char** argv)
{
    //! [Interesting]

    // Instantiate a Serial Camera on UART 0
    upm::SCAM camera(0);

    // make sure port is initialized properly.  115200 baud is the default.
    if (!camera.setupTty()) {
        cerr << "Failed to setup tty port parameters" << endl;
        return 1;
    }

    if (camera.init())
        cout << "Initialized..." << endl;
    else
        cout << "init() failed" << endl;

    if (camera.preCapture())
        cout << "preCapture succeeded..." << endl;
    else
        cout << "preCapture failed." << endl;

    if (camera.doCapture())
        cout << "doCapture succeeded..." << endl;
    else
        cout << "doCapture failed." << endl;

    cout << "Image size is " << camera.getImageSize() << " bytes" << endl;

    if (camera.getImageSize() > 0) {
        cout << "Storing image.jpg..." << endl;
        if (camera.storeImage("image.jpg"))
            cout << "storeImage succeeded..." << endl;
        else
            cout << "storeImage failed." << endl;
    }
    //! [Interesting]

    return 0;
}
