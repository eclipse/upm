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
#include <stdio.h>

#include "mma7361.hpp"
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

    // Instantiate a MMA7361 sensor on analog pins A0 (X), A1 (Y) A2
    // (Z), selftest pin on D2, upm_delay pin on D3 nd an analog reference
    // value of 5.0.  The freefall pin and the range pin are unused
    // (-1).
    upm::MMA7361 sensor(0, 1, 2, 2, 3, -1, -1, 5.0);

    // 1.5g (true = 6g)
    sensor.setRange(false);

    // Every 10th of a second, update and print values

    while (shouldRun) {
        sensor.update();

        float x, y, z;

        sensor.getAcceleration(&x, &y, &z);
        cout << "Acceleration x = " << x << " y = " << y << "z = " << z << endl;

        sensor.getVolts(&x, &y, &z);
        cout << "Volts x = " << x << " y = " << y << "z = " << z << endl;

        cout << endl;

        upm_delay_us(100000);
    }

    //! [Interesting]

    printf("Exiting...\n");

    return 0;
}
