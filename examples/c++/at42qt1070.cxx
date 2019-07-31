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

#include "at42qt1070.hpp"
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
printButtons(upm::AT42QT1070& touch)
{
    bool buttonPressed = false;
    uint8_t buttons = touch.getButtons();

    cout << "Buttons Pressed: ";
    for (int i = 0; i < 7; i++) {
        if (buttons & (1 << i)) {
            cout << i << " ";
            buttonPressed = true;
        }
    }

    if (!buttonPressed)
        cout << "None";

    cout << endl;

    if (touch.isCalibrating())
        cout << "Calibration is occurring." << endl;

    if (touch.isOverflowed())
        cout << "Overflow was detected." << endl;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate an AT42QT1070 on I2C bus 0

    upm::AT42QT1070 touch(AT42QT1070_I2C_BUS, AT42QT1070_DEFAULT_I2C_ADDR);

    while (shouldRun) {
        touch.updateState();
        printButtons(touch);
        upm_delay_us(100000);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
