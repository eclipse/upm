/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <signal.h>
#include <stdio.h>
#include <time.h>

#include "tm1637.hpp"
#include "upm_utilities.h"

using namespace std;
using namespace upm;

bool run = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        run = false;
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    bool point = true;
    int timezone = -7; // Your UTC offset
    time_t rawtime;
    struct tm* gmt;
    char myTime[15];

    fprintf(stdout, "TM1637 Display Example\n");
    signal(SIGINT, sig_handler);

    TM1637 myDisplay = TM1637(0, 1);   // TM1637 on pins 0 (clk) and 1 (dio)
    myDisplay.write(0x39, 0x09, 0x09); // Start a box using 7-segment encoding
    myDisplay.writeAt(3, ']');         // Finish box using writeAt function
    upm_delay(3);                      // Wait 3 seconds

    while (run) {
        time(&rawtime);         // Update raw time
        gmt = gmtime(&rawtime); // Get current time

        int hour = (gmt) ? gmt->tm_hour : 0;
        int min = (gmt) ? gmt->tm_min : 0;
        // Format and store the time in 24 hour format
        snprintf(myTime, 15, "%2d%02d", (hour + timezone + 24) % 24, min);

        myDisplay.write(myTime);           // Write to display as string
        myDisplay.setColon(point ^= true); // Toggle the dots on the display
        upm_delay(1);                      // Only update once every second
    }

    //! [Interesting]
    return 0;
}
