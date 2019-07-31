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

#include "hdc1000.hpp"
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
    std::cout << "Initializing test-application..." << std::endl;

    // Instantiate an HDC1000 instance on bus 1
    upm::HDC1000 mySensor(1);

    // update and print available values every second
    while (run) {
        std::cout << "Humidity: " << mySensor.getHumidity(true) << std::endl
                  << "Temperature: " << mySensor.getTemperature(true) << std::endl;

        std::cout << std::endl;

        upm_delay(1);
    }

    std::cout << "Exiting test-application..." << std::endl;

    //! [Interesting]

    return 0;
}
