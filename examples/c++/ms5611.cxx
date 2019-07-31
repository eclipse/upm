/*
 * Author: Henry Bruce <henry.bruce@intel.com>
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

#include "ms5611.hpp"
#include "upm_utilities.h"

static int is_running = 0;

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
    upm::MS5611 sensor(0);
    signal(SIGINT, sig_handler);

    while (!is_running) {
        int value = sensor.getTemperatureCelsius();
        std::cout << "Temperature = " << value << "C" << std::endl;
        value = sensor.getPressurePa();
        std::cout << "Pressure = " << value << "Pa" << std::endl;
        upm_delay(1);
    }

    std::cout << "exiting application" << std::endl;

    return 0;
}
//! [Interesting]
