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
#include <unistd.h>
#include <stdexcept>
#include <signal.h>
#include <vector>
#include "bh1749.hpp"
#include "upm_utilities.h"

bool isStopped;
upm::BH1749 dev;

void signal_int_handler(int signo)
{
    if (signo == SIGINT)
        isStopped = true;
}

//! [Interesting]
void PrintData(void *args)
{
    std::vector<uint16_t> result = dev.GetMeasurements();
    std::cout << "R: " << result.at(0) <<
                ", G: " << result.at(1) <<
                ", B: " << result.at(2) <<
                ", IR: " << result.at(3) <<
                ", G2: " << result.at(4) << std::endl;
    dev.ResetInterrupt();
}

int main(int argc, char **argv)
{
    signal(SIGINT, signal_int_handler);
    dev.SoftReset();
    dev.SensorInit(INT_JUDGE_1, MEAS_240MS, RGB_GAIN_1X, IR_GAIN_1X, RED);
    dev.SetThresholdHigh(511);
    std::cout << "Installing ISR" << std::endl;
    dev.InstallISR(MRAA_GPIO_EDGE_FALLING, 33, &PrintData, NULL);
    dev.EnableInterrupt();
    dev.Enable();

    while(!isStopped) {
        upm_delay_ms(1000);
    }

    return 0;
}
//! [Interesting]
