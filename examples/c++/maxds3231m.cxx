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

#include "maxds3231m.hpp"
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
    upm::Time3231 t;
    upm::MAXDS3231M sensor(0, ADDR);

    t.second = 1;
    t.minute = 3;
    t.hour = 3;
    t.day = 3;
    t.month = 3;
    t.year = 3;
    t.weekDay = 3;
    sensor.setDate(t); // Note, second should be set to 1.

    upm_delay_us(500000);

    while (!doWork) {
        if (sensor.getDate(t)) {
            std::cout << (int) t.hour << ":" << (int) t.minute << ":" << (int) t.second << std::endl;
        }
        std::cout << "Temperature " << sensor.getTemperature() << std::endl;
        upm_delay_us(1000000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
