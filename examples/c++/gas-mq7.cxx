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
#include <signal.h>
#include <stdint.h>

#include "gas.hpp"
#include "mq7.hpp"

bool shouldRun = true;

using namespace std;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
        shouldRun = false;
    }
}

//! [Interesting]
int
main(int argc, char** argv)
{
    // Attach gas sensor to Analog A0
    upm::MQ7 sensor(0);
    signal(SIGINT, sig_handler);

    uint16_t buffer[128];

    thresholdContext ctx;
    ctx.averageReading = 0;
    ctx.runningAverage = 0;
    ctx.averagedOver = 2;

    // Infinite loop, ends when script is cancelled
    // Repeatedly, take a sample every 2 microseconds;
    // find the average of 128 samples; and
    // print a running graph of asteriskss as averages
    while (shouldRun) {
        int len = sensor.getSampledWindow(2, 128, buffer);
        if (len) {
            int thresh = sensor.findThreshold(&ctx, 30, buffer, len);
            sensor.printGraph(&ctx, 5);
            if (thresh) {
                // do something ....
            }
        }
    }

    cout << "Exiting" << endl;

    return 0;
}
//! [Interesting]
