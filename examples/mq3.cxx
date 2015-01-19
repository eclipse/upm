/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <unistd.h>
#include <iostream>
#include "mq3.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

int is_running = 0;
uint16_t buffer [128];
upm::MQ3 *sensor = NULL;

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
main(int argc, char **argv)
{
    // Attach gas sensor to A0
    sensor = new upm::MQ3(0);
    signal(SIGINT, sig_handler);

    thresholdContext ctx;
    ctx.averageReading = 0;
    ctx.runningAverage = 0;
    ctx.averagedOver   = 2;

    // Infinite loop, ends when script is cancelled
    // Repeatedly, take a sample every 2 milliseconds;
    // find the average of 128 samples; and
    // print a running graph of the averages using a resolution of 5
    while (!is_running) {
        int len = sensor->getSampledWindow (2, 128, buffer);
        if (len) {
            int thresh = sensor->findThreshold (&ctx, 30, buffer, len);
            sensor->printGraph(&ctx, 5);
            if (thresh) {
                // do something ....
            }
        }
    }

    std::cout << "exiting application" << std::endl;

    delete sensor;

    return 0;
}
//! [Interesting]
