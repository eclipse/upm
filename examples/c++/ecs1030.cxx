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

#include "ecs1030.hpp"

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
    upm::ECS1030 sensor(0);
    signal(SIGINT, sig_handler);

    while (!is_running) {
        std::cout << "I = " << sensor.getCurrency_A() << ", Power = " << sensor.getPower_A()
                  << std::endl;
        std::cout << "I = " << sensor.getCurrency_B() << ", Power = " << sensor.getPower_B()
                  << std::endl;
    }

    std::cout << "exiting application" << std::endl;

    return 0;
}
//! [Interesting]
