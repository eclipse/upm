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

#include "tcs37727.hpp"
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
    upm::tcs37727_data_t data;

    std::cout << "Initializing test-application..." << std::endl;

    // Instantiate an TCS37727 instance on bus 1
    upm::TCS37727 mySensor(1);

    // activate periodic measurements
    mySensor.setActive();

    // update and print available values every second
    while (run) {
        mySensor.getData(&data, true);
        std::cout << "Red: " << (int) data.red << std::endl
                  << "Green: " << (int) data.green << std::endl
                  << "Blue: " << (int) data.blue << std::endl
                  << "Clear: " << (int) data.clear << std::endl
                  << "Lux: " << (int) data.lux << std::endl
                  << "Color temperature: " << (int) data.ct << std::endl;

        std::cout << std::endl;

        upm_delay(1);
    }

    std::cout << "Exiting test-application..." << std::endl;

    //! [Interesting]

    return 0;
}
