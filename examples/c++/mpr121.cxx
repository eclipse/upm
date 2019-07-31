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

#include "mpr121.hpp"
#include "upm_utilities.h"

using namespace std;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

void
printButtons(upm::MPR121& touch)
{
    bool buttonPressed = false;

    cout << "Buttons Pressed: ";
    for (int i = 0; i < 12; i++) {
        if (touch.m_buttonStates & (1 << i)) {
            cout << i << " ";
            buttonPressed = true;
        }
    }

    if (!buttonPressed)
        cout << "None";

    if (touch.m_overCurrentFault)
        cout << "Over Current Fault detected!" << endl;

    cout << endl;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate an MPR121 on I2C bus 0

    upm::MPR121 touch(MPR121_I2C_BUS, MPR121_DEFAULT_I2C_ADDR);

    // init according to AN3944 defaults
    touch.configAN3944();

    while (shouldRun) {
        touch.readButtons();
        printButtons(touch);
        upm_delay(1);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
