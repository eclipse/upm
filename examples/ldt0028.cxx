/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "ldt0028.h"

int
main(int argc, char **argv)
{
//! [Interesting]
    const int NUMBER_OF_SECONDS = 10;
    const int MICROSECONDS_PER_SECOND = 1000000;
    const int SAMPLES_PER_SECOND = 50;
    const int THRESHOLD = 100;

    // Create the LDT0-028 Piezo Vibration Sensor object using AIO pin 0
    upm::LDT0028* sensor = new upm::LDT0028(0);

    // Read the signal every 20 milliseconds for 10 seconds
    std::cout << "For the next " << NUMBER_OF_SECONDS << " seconds, " 
              << SAMPLES_PER_SECOND << " samples will be taken every second." 
              << std::endl << std::endl;
    uint16_t* buffer = new uint16_t[NUMBER_OF_SECONDS * SAMPLES_PER_SECOND];
    for (int i=0; i < NUMBER_OF_SECONDS * SAMPLES_PER_SECOND; i++) {
        buffer[i] = (uint16_t) sensor->getSample();
        usleep(MICROSECONDS_PER_SECOND / SAMPLES_PER_SECOND);
    }

    // Print the number of times the reading was greater than the threshold
    int count = 0;
    for (int i=0; i < NUMBER_OF_SECONDS * SAMPLES_PER_SECOND; i++) {
        if (buffer[i] > THRESHOLD) {
            count++;
        }
    }
    std::cout << sensor->name() << " exceeded the threshold value of " <<
        THRESHOLD << " a total of " << count << " times," << std::endl
        << "out of a total of " << NUMBER_OF_SECONDS*SAMPLES_PER_SECOND 
        << " readings." << std::endl << std::endl;

    // Print a graphical representation of the average value sampled
    // each second for the past 10 seconds, using a scale factor of 15
    std::cout << "Now printing a graphical representation of the average reading "
        << std::endl << "each second for the last " 
        << NUMBER_OF_SECONDS << " seconds." << std::endl;
    const int SCALE_FACTOR = 15;
    for (int i=0; i < NUMBER_OF_SECONDS; i++) {
        long sum = 0;
        for (int j=0; j < SAMPLES_PER_SECOND; j++) {
            sum += buffer[i*SAMPLES_PER_SECOND + j];
        }
        double average = (double) sum / (double) SAMPLES_PER_SECOND;
        int stars_to_print = (int) round(average / SCALE_FACTOR);
        std::cout << "(" << std::setw(4) << (int) round(average) << ") | ";
        for (int j=0; j<stars_to_print; j++) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    // Delete the sensor object
    delete sensor;
//! [Interesting]

    return 0;
}
