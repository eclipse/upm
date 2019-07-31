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

#include "am2315.hpp"
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
    float humidity = 0.0;
    float temperature = 0.0;

    upm::AM2315 sensor(0, AM2315_I2C_ADDRESS);

    sensor.testSensor();

    while (!doWork) {
        humidity = sensor.getHumidity();
        temperature = sensor.getTemperature();

        std::cout << "humidity value = " << humidity << ", temperature value = " << temperature
                  << std::endl;
        upm_delay_us(500000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
