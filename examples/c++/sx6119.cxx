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

#include "sx6119.hpp"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a SX6119 on digital pins 2 (power) and 3 (seek)
    // This example was tested on the Grove FM Receiver.

    upm::SX6119 radio(2, 3);

    // if an argument was specified (any argument), seek to the next
    // station, else just toggle the power.

    cout << "Supply any argument to the command line to seek to the" << endl;
    cout << "next station." << endl;
    cout << "Running the example without an argument will toggle the" << endl;
    cout << "power on or off." << endl;

    cout << endl;

    bool doSeek = false;

    if (argc > 1)
        doSeek = true;

    // depending on what was selected, do it

    if (doSeek)
        radio.seek();
    else
        radio.togglePower();

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
