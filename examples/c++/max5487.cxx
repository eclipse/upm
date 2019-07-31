/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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

#include "max5487.hpp"
#include "upm_utilities.h"

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
    }
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    upm::MAX5487 sensor(7);

    // Power LED UP
    for (int i = 0; i < 255; i++) {
        sensor.setWiperA(i);
        upm_delay_us(5000);
    }

    // Power LED DOWN
    for (int i = 0; i < 255; i++) {
        sensor.setWiperA(255 - i);
        upm_delay_us(5000);
    }

    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
