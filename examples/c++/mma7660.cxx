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

#include "mma7660.hpp"
#include "upm_utilities.h"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate an MMA7660 on I2C bus 0

    upm::MMA7660 accel(MMA7660_DEFAULT_I2C_BUS, MMA7660_DEFAULT_I2C_ADDR);

    // place device in standby mode so we can write registers
    accel.setModeStandby();

    // enable 64 samples per second
    accel.setSampleRate(MMA7660_AUTOSLEEP_64);

    // place device into active mode
    accel.setModeActive();

    while (shouldRun) {
        float ax, ay, az;

        accel.getAcceleration(&ax, &ay, &az);
        cout << "Acceleration: x = " << ax << "g y = " << ay << "g z = " << az << "g" << endl;

        cout << endl;
        upm_delay_us(500000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
