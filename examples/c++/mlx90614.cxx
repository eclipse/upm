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

#include "mlx90614.hpp"
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
    upm::MLX90614 sensor(0, DEVICE_ADDR);

    while (!doWork) {
        std::cout << "Object Temperature (" << sensor.readObjectTempC()
                  << ")  Ambient Temperature (" << sensor.readAmbientTempC() << ")" << std::endl;
        upm_delay_us(1000000);
    }
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    return 0;
}
