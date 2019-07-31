/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>
#include <stdio.h>

#include "mic.hpp"

int is_running = 0;
uint16_t buffer[128];

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        is_running = 1;
    }
}

//! [Interesting]
int
main(int argc, char** argv)
{
    // Attach microphone to analog port A0
    upm::Microphone sensor(0);
    signal(SIGINT, sig_handler);

    thresholdContext ctx;
    ctx.averageReading = 0;
    ctx.runningAverage = 0;
    ctx.averagedOver = 2;

    // Infinite loop, ends when script is cancelled
    // Repeatedly, take a sample every 2 microseconds;
    // find the average of 128 samples; and
    // print a running graph of the averages
    while (!is_running) {
        int len = sensor.getSampledWindow(2, 128, buffer);
        if (len) {
            int thresh = sensor.findThreshold(&ctx, 30, buffer, len);
            sensor.printGraph(&ctx);
            if (thresh) {
                // do something ....
            }
        }
    }

    std::cout << "exiting application" << std::endl;

    return 0;
}
//! [Interesting]
