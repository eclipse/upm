/*
 * Author: Saloni Jain <saloni.jain@tcs.com>
 * Author: Niti Rohilla <niti.rohilla@tcs.com>
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
#include <stdio.h>

#include "lidarlitev3.hpp"
#include "upm_utilities.h"

volatile int doWork = 0;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("\nCtrl-C received.\n");
        doWork = 1;
    }
}

int
main(int argc, char** argv)
{
    // Register signal handler
    signal(SIGINT, sig_handler);

    //! [Interesting]
    upm::LIDARLITEV3 sensor(0, ADDR);

    while (!doWork) {
        std::cout << "Distance = " << sensor.getDistance() << std::endl;
        upm_delay_us(50000);
    }

    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
