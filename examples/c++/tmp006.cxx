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

#include "tmp006.hpp"
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

    // please make sure that you have the right i2c address for your device
    // the correct range of addresses is 0x40 - 0x47
    // Instantiate an TMP006 instance on bus 1
    upm::TMP006 mySensor(1, TMP006_CONFIG_CR_DEF, TMP006_I2C_ADDRESS);

    // you can also get basic tmp007 functionality by initializing it as follows
    // upm::TMP006 mySensor(1, TMP006_CONFIG_CR_DEF, TMP007_I2C_ADDRESS);

    // activate periodic measurements
    mySensor.setActive();

    // update and print available values every second
    while (run) {
        // Print out temperature value in *C
        std::cout << "Temperature: " << mySensor.getTemperature(true) << " *C" << std::endl;

        upm_delay(1);
    }

    std::cout << "Exiting test-application..." << std::endl;

    //! [Interesting]

    return 0;
}
