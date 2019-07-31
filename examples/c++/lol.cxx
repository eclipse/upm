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

#include "lol.hpp"
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
    upm::LoL sensor;
    signal(SIGINT, sig_handler);

    // buffer = sensor.getFramebuffer();
    int x = 0, y = 0;
    while (!is_running) {
        sensor.setPixel(x, y, !(sensor.getPixel(x, y)));
        if (++x == 13) {
            x = 0;
            y++;
        }
        if (y == 9)
            y = 0;
        upm_delay_us(10000);
    }

    std::cout << "exiting application" << std::endl;

    return 0;
}
//! [Interesting]
