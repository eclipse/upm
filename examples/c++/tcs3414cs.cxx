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

#include "tcs3414cs.hpp"
#include "upm_utilities.h"

int doWork = 0;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        doWork = 1;
    }
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    upm::tcs3414sc_rgb_t rgb;

    // Instantiate the color sensor on I2C
    upm::TCS3414CS sensor;

    // Print out the r, g, b, and clr value every 0.5 seconds
    while (!doWork) {
        sensor.readRGB(&rgb);
        std::cout << (int) rgb.r << ", " << (int) rgb.g << ", " << (int) rgb.b << ", " << rgb.clr
                  << std::endl;
        upm_delay_us(500000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
