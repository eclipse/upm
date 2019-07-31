/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-02017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
