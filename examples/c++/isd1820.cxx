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

#include "isd1820.hpp"
#include "upm_utilities.h"

using namespace std;

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a ISD1820 on digital pins 2 (play) and 3 (record)
    // This example was tested on the Grove Recorder.

    upm::ISD1820 recorder(2, 3);

    // if an argument was specified (any argument), go into record mode,
    // else playback a previously recorded sample

    cout << "Supply any argument to the command line to record." << endl;
    cout << "Running this example without arguments will play back any " << endl;
    cout << "previously recorded sound." << endl;
    cout << "There is approximately 10 seconds of recording time." << endl;

    cout << endl;

    bool doRecord = false;

    if (argc > 1)
        doRecord = true;

    // depending on what was selected, do it, and upm_delay for 15 seconds

    if (doRecord)
        recorder.record(true);
    else
        recorder.play(true);

    // There are about 10 seconds of recording/playback time, so we will
    // upm_delay for a little extra time.
    cout << "Sleeping for 15 seconds..." << endl;
    upm_delay(15);

    // now, turn off whatever we were doing.

    if (doRecord)
        recorder.record(false);
    else
        recorder.play(false);

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
