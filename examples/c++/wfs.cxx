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
#include <signal.h>

#include "upm_utilities.h"
#include "wfs.hpp"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate a Water Flow Sensor on digital pin D2.  This must
    // be an interrupt capable pin.
    upm::WFS flow(2);

    // set the flow counter to 0 and start counting
    flow.clearFlowCounter();
    flow.startFlowCounter();

    while (shouldRun) {
        // we grab these (millis and flowCount) just for display
        // purposes in this example
        uint32_t millis = flow.getMillis();
        uint32_t flowCount = flow.flowCounter();

        float fr = flow.flowRate();

        // output milliseconds passed, flow count, and computed flow rate
        cout << "Millis: " << millis << " Flow Count: " << flowCount;
        cout << " Flow Rate: " << fr << " LPM" << endl;

        // best to gather data for at least one second for reasonable
        // results.
        upm_delay(2);
    }

    flow.stopFlowCounter();
    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
