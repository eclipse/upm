/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-02017 Intel Corporation.
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
#include <stdio.h>
#include <signal.h>

#include <upm_utilities.h>
#include "wfs.h"

int shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}


int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    // Instantiate a Water Flow Sensor on digital pin D2.  This must
    // be an interrupt capable pin.

    wfs_context sensor = wfs_init(2);
    if (!sensor)
    {
        printf("%s: wfs_init() failed\n", __FUNCTION__);
        return 1;
    }

    // set the flow counter to 0 and start counting
    wfs_clear_flow_counter(sensor);
    if (wfs_start_flow_counter(sensor))
    {
        printf("%s: wfs_start_flow_counter() failed\n", __FUNCTION__);
        wfs_close(sensor);
        return 1;
    }

    while (shouldRun)
    {
        // we grab these (millis and flowCount) just for display
        // purposes in this example
        uint32_t millis = wfs_get_millis(sensor);
        uint32_t flowCount = wfs_flow_counter(sensor);

        float fr = wfs_flow_rate(sensor);

        // output milliseconds passed, flow count, and computed flow rate
        printf("Millis: %8d Flow Count: %5d Flow Rate: %f LPM\n",
               millis, flowCount, fr);

        // best to gather data for at least one second for reasonable
        // results.
        upm_delay(2);
    }

    wfs_stop_flow_counter(sensor);

    printf("Exiting...\n");

    wfs_close(sensor);

//! [Interesting]
    return 0;
}
