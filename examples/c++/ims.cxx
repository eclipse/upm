/*
 * Author: Noel Eck <noel.eck@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <signal.h>

#include "ims.hpp"
#include "ims_defs.h"
#include "upm_utilities.h"

using namespace upm;

int shouldRun = true;

void
sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int
main(int argc, char** argv)
{
    signal(SIGINT, sig_handler);
    //! [Interesting]

    // Instantiate a IMS instance using i2c bus 0 and default address
    upm::IMS sensor(0);

    int i2c_addr_cur = IMS_ADDRESS_DEFAULT + 1;
    while (shouldRun) {
        std::cout << std::hex << "Version: 0x" << sensor.get_version() << " light: 0x"
                  << sensor.get_light() << " moisture: 0x" << sensor.get_moisture()
                  << " temp: " << sensor.get_temperature() << " C" << std::endl;

        // Change the address and continue
        if (i2c_addr_cur >= 128)
            i2c_addr_cur = 1;
        std::cout << "Changing device address to 0x" << i2c_addr_cur << std::endl;
        sensor.reset_i2c_address(i2c_addr_cur++);
        upm_delay(1);
    }
    //! [Interesting]

    std::cout << "Exiting..." << std::endl;

    return 0;
}
