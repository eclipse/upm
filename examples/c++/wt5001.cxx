/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <stdlib.h>
#include <string>

#include "wt5001.hpp"

using namespace std;

void
printUsage(char* progname)
{
    cout << "Usage:" << progname << " <command>" << endl;
    cout << "Commands:" << endl;
    cout << "0  - stop playing" << endl;
    cout << "1  - start playing track 1" << endl;
    cout << "2  - pause/un-pause playback" << endl;
    cout << "3  - next track" << endl;
    cout << "4  - previous track" << endl;
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate a WT5001 serial MP3 player on uart 0.
    // This example was tested on the Grove Serial MP3 module.

    upm::WT5001 mp3(0);

    int cmd = -1;
    if (argc > 1)
        cmd = atoi(argv[1]);

    // make sure port is initialized properly.  9600 baud is the default.
    if (!mp3.setupTty(B9600)) {
        cerr << "Failed to setup tty port parameters" << endl;
        return 1;
    }

    switch (cmd) {
        case 0:
            mp3.stop();
            break;

        case 1:
            mp3.play(upm::WT5001::SD, 1);
            break;

        case 2:
            mp3.pause();
            break;

        case 3:
            mp3.next();
            break;

        case 4:
            mp3.previous();
            break;

        default:
            // nothing, just output usage, and info below
            printUsage(argv[0]);
            break;
    }

    // Example: set the date
    // mp3.setDate(2015, 1, 1);

    // Example: set the time
    // mp3.setTime(12, 30, 30);

    // print out some information
    uint8_t vol = 0;
    if (mp3.getVolume(&vol))
        cout << "The current volume is: " << int(vol) << endl;

    uint8_t ps = 0;
    if (mp3.getPlayState(&ps))
        cout << "The current play state is: " << int(ps) << endl;

    uint16_t numf = 0;
    if (mp3.getNumFiles(upm::WT5001::SD, &numf))
        cout << "The number of files on the SD card is: " << int(numf) << endl;

    uint16_t curf = 0;
    if (mp3.getCurrentFile(&curf))
        cout << "The current file is: " << int(curf) << endl;

    uint16_t year = 0;
    uint8_t month = 0, day = 0;
    if (mp3.getDate(&year, &month, &day))
        cout << "The device date is: " << int(month) << "/" << int(day) << "/" << int(year) << endl;

    uint8_t hour = 0, minute = 0, second = 0;
    if (mp3.getTime(&hour, &minute, &second))
        cout << "The device time is: " << int(hour) << ":" << int(minute) << ":" << int(second)
             << endl;

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
