/*
 * Author: Stan Gifford <stan@gifford.id.au>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

/**
 * Description
 * Demo program for Adafruit 16 channel servo shield/controller
 * Physical setup for tests is a single servo attached to one channel.
 * Note - when 3 or more GWS servos attached results unpredictable.
 * Adafruit do recommend a Cap be installed on the board which should alleviate
 * the issue.
 * I (and Adafruit) are unable to give any Capacitor sizing data.
 */

#include <iostream>

#include "adafruitss.hpp"
#include "upm_utilities.h"

using namespace std;

int
main()
{
    int n;

    //! [Interesting]
    upm::adafruitss servos(6, 0x40);

    for (;;) {
        cout << "Setting all to 0" << endl;
        for (n = 0; n < 16; n++)
            servos.servo(n, 1, 0); // GWS Mini Servo = Type 1.
        upm_delay_us(1000000);     // Wait 1 second

        cout << "Setting all to 45" << endl;
        for (n = 0; n < 16; n++)
            servos.servo(n, 1, 45);
        upm_delay_us(1000000); // Wait 1 second

        cout << "Setting all to 90" << endl;
        for (n = 0; n < 16; n++)
            servos.servo(n, 1, 90);
        upm_delay_us(1000000); // Wait 1 second

        cout << "Setting all to 135" << endl;
        for (n = 0; n < 16; n++)
            servos.servo(n, 1, 135);
        upm_delay_us(1000000); // Wait 1 second

        cout << "Setting all to 180" << endl;
        for (n = 0; n < 16; n++)
            servos.servo(n, 1, 160);
        upm_delay_us(2000000); // Wait 1 second
    }
    //! [Interesting]
    return 0;
}
