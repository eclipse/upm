/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "dfrorp.hpp"
#include "upm_utilities.h"

using namespace std;

bool shouldRun = true;

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

    // Instantiate a DFRobot ORP sensor on analog pin A0 with an analog
    // reference voltage of 5.0.
    upm::DFRORP sensor(0, 5.0);

    // To calibrate:
    //
    // Disconnect the sensor probe (but leave the sensor interface board
    // connected).  Then run one of the examples while holding down the
    // 'calibrate' button on the device.  Read the ORP value reported
    // (it should be fairly small).
    //
    // This value is what you should supply to setCalibrationOffset().
    // Then reconnect the probe to the interface board and you should be
    // ready to go.
    //
    // DO NOT press the calibrate button on the interface board while
    // the probe is attached or you can permanently damage the probe.
    sensor.setCalibrationOffset(0.97);

    // Every second, update and print values
    while (shouldRun) {
        sensor.update();

        cout << "ORP: " << sensor.getORP() << " mV" << endl;

        cout << endl;

        upm_delay(1);
    }

    //! [Interesting]

    cout << "Exiting" << endl;

    return 0;
}
