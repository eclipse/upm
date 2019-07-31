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
