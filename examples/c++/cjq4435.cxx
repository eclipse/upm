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

#include "cjq4435.hpp"
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
main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    // Instantiate a CJQ4435 MOSFET on a PWM capable digital pin D3
    upm::CJQ4435 mosfet(3);

    mosfet.setPeriodMS(10);
    mosfet.enable(true);

    while (shouldRun) {
        // start with a duty cycle of 0.0 (off) and increment to 1.0 (on)
        for (float i = 0.0; i <= 1.0; i += 0.1) {
            mosfet.setDutyCycle(i);
            upm_delay_us(100000);
        }
        upm_delay(1);
        // Now take it back down
        // start with a duty cycle of 1.0 (on) and decrement to 0.0 (off)
        for (float i = 1.0; i >= 0.0; i -= 0.1) {
            mosfet.setDutyCycle(i);
            upm_delay_us(100000);
        }
        upm_delay(1);
    }

    //! [Interesting]

    cout << "Exiting..." << endl;

    return 0;
}
