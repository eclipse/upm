/*
 * Author: William Penner <william.penner@intel.com>
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

#include "mpl3115a2.hpp"
#include "upm_utilities.h"

volatile int doWork = 0;

void
sig_handler(int signo)
{
    if (signo == SIGINT) {
        printf("\nCtrl-C received.\n");
        doWork = 1;
    }
}

int
main(int argc, char** argv)
{
    // Register signal handler
    signal(SIGINT, sig_handler);

    //! [Interesting]
    float pressure = 0.0;
    float temperature = 0.0;
    float altitude = 0.0;
    float sealevel = 0.0;

    upm::MPL3115A2 sensor(0, MPL3115A2_I2C_ADDRESS);

    sensor.testSensor();

    while (!doWork) {
        temperature = sensor.getTemperature(true);
        pressure = sensor.getPressure(false);
        altitude = sensor.getAltitude();
        sealevel = sensor.getSealevelPressure();

        std::cout << "pressure value = " << pressure << ", altitude value = " << altitude
                  << ", sealevel value = " << sealevel << ", temperature = " << temperature
                  << std::endl;
        upm_delay_us(500000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
