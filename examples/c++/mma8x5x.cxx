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

#include "mma8x5x.hpp"
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
    upm::mma8x5x_data_t data;

    // U can set your own parameters for an MMA8X5X instance with
    /*
    upm::mma8x5x_params_t params;
    params.type   = <device-id>;      device-id of your sensor
    params.rate   = <sampling-rate>;    between 0<<3 and 7<<3
    params.range  = <range>;        between 0 and 2
    params.offsetX  = <x-axis offset>;    between 0 and 255
    params.offsetY  = <y-axis offset>;    between 0 and 255
    params.offsetZ  = <z-axis offset>;    between 0 and 255
  */

    std::cout << "Initializing test-application..." << std::endl;

    // Instantiate an MMA8X5X instance on bus 1 with default parameters
    // The sensor-type will be detected by reading out the device-id
    upm::MMA8X5X mySensor(1);

    // If u have set own parameters use this one
    /*
        upm::MMA8X5X mySensor(1, &params);
  */

    // activate periodic measurements
    mySensor.setActive();

    // update and print available values every second
    while (run) {
        mySensor.getData(&data, true);
        std::cout << "x: " << (int) data.x << std::endl
                  << "y: " << (int) data.y << std::endl
                  << "z: " << (int) data.z << std::endl;

        std::cout << std::endl;

        upm_delay(1);
    }

    std::cout << "Exiting test-application..." << std::endl;

    //! [Interesting]

    return 0;
}
