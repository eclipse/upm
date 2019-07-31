/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* This program and the accompanying materials are made available under the
* terms of the The MIT License which is available at
* https://opensource.org/licenses/MIT.
*
* SPDX-License-Identifier: MIT
*/

#include <iostream>
#include <signal.h>
#include  <sys/time.h>
#include "bh1792.hpp"
#include "upm_utilities.h"

bool isStopped;
upm::BH1792 dev;

void signal_int_handler(int signo)
{
    if (signo == SIGINT)
        isStopped = true;
}

//! [Interesting]
void PrintData(void *args)
{
    std::vector<std::vector<int>> fifo;
    struct timespec now;

    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    try {
        fifo = dev.GetFifoData();

        std::cout << "\nHeart beat sensor FIFO data:" << std::endl;
        for(int i = 0; i < 32; i++)
            std::cout << i << ": off: " << fifo.at(i).at(0) << ", on: " <<
                        fifo.at(i).at(1) << std::endl;

        std::cout << "timestamp " << now.tv_sec << " sec, " << now.tv_nsec <<
                    " nsec" << std::endl;
    } catch (std::exception& e) {
        std::cout << "Failed to read FIFO data" << std::endl;
        throw;
    }
}

int main(int argc, char **argv)
{
    float measTime;

    signal(SIGINT, signal_int_handler);
    dev.SoftReset();
    dev.EnableSyncMode(256, 32);
    std::cout << "Installing ISR" << std::endl;
    dev.InstallISR(MRAA_GPIO_EDGE_FALLING, 33, &PrintData, NULL);
    measTime = dev.GetMeasurementTimeMS();
    dev.StartMeasurement();

    while(!isStopped) {
        usleep(measTime * 1000);
    }

    return 0;
}
//! [Interesting]
