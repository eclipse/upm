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

#include "hcsr04.hpp"
#include "upm_utilities.h"

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
        shouldRun = false;
    }
}

//! [Interesting]
int
main(int argc, char** argv)
{
    upm::HCSR04 sonar(2, 4);
    signal(SIGINT, sig_handler);

    upm_delay(1);

    while (shouldRun) {
        std::cout << "get distance" << std::endl;
        double distance = sonar.getDistance(HCSR04_CM);
        std::cout << "distance " << distance << std::endl;
        upm_delay(2);
    }
    std::cout << "Exiting... " << std::endl;

    return 0;
}
//! [Interesting]
