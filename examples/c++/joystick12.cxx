/*
 * Author: Alexander Komarov <alexander.komarov@intel.com>
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

#include "joystick12.hpp"
#include "upm_utilities.h"

int is_running = 0;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        is_running = 1;
    }
}

//! [Interesting]
int
main(int argc, char** argv)
{
    // Instantiate a joystick on analog pins A0 and A1
    upm::Joystick12 sensor(0, 1);
    signal(SIGINT, sig_handler);

    // Print the X and Y input values every second
    while (!is_running) {
        float x = sensor.getXInput();
        float y = sensor.getYInput();
        std::cout << "Driving X:" << x << ": and Y:" << y << std::endl;
        upm_delay(1);
    }

    std::cout << "exiting application" << std::endl;

    return 0;
}
//! [Interesting]
