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

#include "mma7455.hpp"
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
    upm::MMA7455 sensor(0, ADDR);

    short x, y, z;
    while (!doWork) {
        sensor.readData(&x, &y, &z);
        std::cout << "Accelerometer X(" << x << ") Y(" << y << ") Z(" << z << ")" << std::endl;
        upm_delay_us(100000);
    }

    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
