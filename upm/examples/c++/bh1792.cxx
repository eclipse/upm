/*
* The MIT License (MIT)
*
* Author: Assam Boudjelthia
* Copyright (c) 2018 Rohm Semiconductor.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
