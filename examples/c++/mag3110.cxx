/* Author: Norbert Wesp <nwesp@phytec.de>
 * Copyright (c) 2017 Phytec Messtechnik GmbH.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "mag3110.hpp"
#include "upm_utilities.h"

using namespace upm;

bool run = true;

void
sig_handler(int sig)
{
    if (sig == SIGINT)
        run = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);

    //! [Interesting]
    upm::mag3110_data_t data;

    std::cout << "Initializing test-application..." << std::endl;

    // Instantiate an MAG3110 instance on bus 1
    upm::MAG3110 mySensor(1);

    // activate periodic measurements
    mySensor.setActive();

    // update and print available values every second
    while (run) {
        mySensor.getData(&data, true);
        std::cout << "x: " << data.x << std::endl
                  << "y: " << data.y << std::endl
                  << "z: " << data.z << std::endl
                  << "Status: " << data.status << std::endl
                  << "Die temperature: " << data.dtemp << std::endl;

        std::cout << std::endl;

        upm_delay(1);
    }

    std::cout << "Exiting test-application..." << std::endl;

    //! [Interesting]

    return 0;
}
