/*
 * Author: Marc Graham <marc@m2ag.net>
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

#include "micsv89.hpp"
#include "upm_utilities.h"

/*
 * An example for using the MICSV89 sensor library.
 * The MICSV89 comes in 4 variants, PWM and I2c
 * in 3.3 volts and 5 volts. This library only implements
 * the I2c version of the device.
 *
 * Device output is not valid until a warm up of 15 minutes
 * of operation.
 *
 * Additional linker flags: -lupm-micsv89
 */

using namespace std;

volatile int running = 1;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
        cout << "Exiting program." << endl;
        running = 0;
    }
}

int
main()
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    upm::MICSV89 sensor(6);

    while (running) {
        sensor.update();
        while (!sensor.valid())
            ;
        cout << "co2: " << sensor.co2equ() << endl;
        cout << "short: " << sensor.vocshort() << endl;
        cout << "tvoc: " << sensor.tvoc() << endl;
        cout << "resistor: " << sensor.resistor() << endl;
        cout << "****************************" << endl;
        upm_delay(5);
    }

    //! [Interesting]

    return 0;
}
