/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdint.h>

#include "ldt0028.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    //! [Interesting]
    const int NUMBER_OF_SECONDS = 10;
    const int MICROSECONDS_PER_SECOND = 1000000;
    const int SAMPLES_PER_SECOND = 50;
    const int THRESHOLD = 100;

    // Create the LDT0-028 Piezo Vibration Sensor object using AIO pin 0
    upm::LDT0028 sensor(0);

    // Read the signal every 20 milliseconds for 10 seconds
    std::cout << "For the next " << NUMBER_OF_SECONDS << " seconds, " << SAMPLES_PER_SECOND
              << " samples will be taken every second." << std::endl
              << std::endl;
    uint16_t buffer[NUMBER_OF_SECONDS * SAMPLES_PER_SECOND];
    for (int i = 0; i < NUMBER_OF_SECONDS * SAMPLES_PER_SECOND; i++) {
        buffer[i] = (uint16_t) sensor.getSample();
        upm_delay_us(MICROSECONDS_PER_SECOND / SAMPLES_PER_SECOND);
    }

    // Print the number of times the reading was greater than the threshold
    int count = 0;
    for (int i = 0; i < NUMBER_OF_SECONDS * SAMPLES_PER_SECOND; i++) {
        if (buffer[i] > THRESHOLD) {
            count++;
        }
    }
    std::cout << sensor.name() << " exceeded the threshold value of " << THRESHOLD << " a total of "
              << count << " times," << std::endl
              << "out of a total of " << NUMBER_OF_SECONDS * SAMPLES_PER_SECOND << " readings."
              << std::endl
              << std::endl;

    // Print a graphical representation of the average value sampled
    // each second for the past 10 seconds, using a scale factor of 15
    std::cout << "Now printing a graphical representation of the average reading " << std::endl
              << "each second for the last " << NUMBER_OF_SECONDS << " seconds." << std::endl;
    const int SCALE_FACTOR = 15;
    for (int i = 0; i < NUMBER_OF_SECONDS; i++) {
        long sum = 0;
        for (int j = 0; j < SAMPLES_PER_SECOND; j++) {
            sum += buffer[i * SAMPLES_PER_SECOND + j];
        }
        double average = (double) sum / (double) SAMPLES_PER_SECOND;
        int stars_to_print = (int) round(average / SCALE_FACTOR);
        std::cout << "(" << std::setw(4) << (int) round(average) << ") | ";
        for (int j = 0; j < stars_to_print; j++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    // Delete the sensor object
    //! [Interesting]

    return 0;
}
