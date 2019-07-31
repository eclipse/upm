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
#include <iomanip>
#include <unistd.h>
#include <signal.h>
#include "kxtj3.hpp"
#include "upm_utilities.h"

#define SENSOR_ADDR 0x0f
#define I2C_BUS 0
#define SAMPLE_COUNT 10

bool isStopped = false;

void signal_int_handler(int signo)
{
    if (signo == SIGINT)
        isStopped = true;
}

void print_acceleration_data(upm::KXTJ3 &dev)
{
    float wait_time = dev.GetAccelerationSamplePeriod() * SECOND_IN_MICRO_S;
    int sample_counter = SAMPLE_COUNT;
    std::vector<float> xyz;
    while ((sample_counter-- > 0) && !isStopped)
    {
        xyz = dev.GetAccelerationVector();
        std::cout << std::fixed << std::setprecision(3)
                  << xyz[0] << " | " << xyz[1] << " | " << xyz[2] << std::endl;
        upm_delay_us(wait_time);
    }
}

int main(int argc, char **argv)
{
    signal(SIGINT, signal_int_handler);

    std::cout << "Sensor init" << std::endl;
    upm::KXTJ3 dev(I2C_BUS, SENSOR_ADDR);

    std::cout << "Setting settings:\nODR: 25 Hz\nResolution: "
              << "High\nAcceleration range: 16g with 14bits" << std::endl;
    dev.SensorInit(KXTJ3_ODR_25, HIGH_RES, KXTJ3_RANGE_16G_14);
    std::cout << "Showing acceleration data:" << std::endl;
    print_acceleration_data(dev);

    std::cout << "Closing sensor" << std::endl;
    return 0;
}
