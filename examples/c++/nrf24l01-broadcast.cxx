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
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "nrf24l01.hpp"
#include "upm_utilities.h"

int running = 0;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        running = 1;
    }
}

void
nrf_handler()
{
}

int
main(int argc, char** argv)
{
    //! [Interesting]

    upm::NRF24L01 sensor(7, 8);
    sensor.setBeaconingMode();

    std::vector<std::string> msgs;

    msgs.push_back("Hello World 1!!!");
    msgs.push_back("Hello World 2!!!");
    msgs.push_back("Hello World 3!!!");
    msgs.push_back("Hello World 4!!!");
    msgs.push_back("Hello World 5!!!");

    signal(SIGINT, sig_handler);

    while (!running) {
        for (std::vector<std::string>::iterator item = msgs.begin(); item != msgs.end(); ++item) {
            std::cout << "BROADCASTING " << (*item).c_str() << std::endl;

            for (int i = 0; i < 3; i++) {
                sensor.sendBeaconingMsg((uint8_t*) (*item).c_str());
                upm_delay_us(1000000);
            }
        }
    }

    std::cout << "exiting application" << std::endl;

    msgs.clear();
    //! [Interesting]

    return 0;
}
