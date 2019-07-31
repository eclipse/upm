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

#include "th02.hpp"
#include "upm_utilities.h"

int doWork = 0;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        doWork = 1;
    }
}

int
main(int argc, char** argv)
{
    //! [Interesting]
    float temperature = 0.0;
    float humidity = 0.0;
    upm::TH02 sensor;

    while (!doWork) {
        temperature = sensor.getTemperature();
        humidity = sensor.getHumidity();
        std::cout << "Temperature = " << temperature << ", Humidity = " << humidity << std::endl;
        upm_delay_us(500000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
