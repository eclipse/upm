/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2016 Intel Corporation.
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

/* This example demonstrates how to use one the ADS1015 ADC on the Grove Joule
 * Shield or the Sparkfun ADC Block for Edison with devices that output a small
 * differential voltage (e.g. geophones, piezoelectric bands or pads,
 * thermocouples).
 */

#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "ads1015.hpp"
#include "ads1x15.hpp"
#include "upm_utilities.h"

using namespace std;
using namespace upm;

bool running = true; // Controls main read/write loop

// Thread function
void
stop()
{
    upm_delay(10);
    running = false;
}

int
main()
{
    //! [Interesting]
    long id = 0;                        // Sample number
    string fileName = "./ads1015.data"; // Output filename
    ofstream f;

    // Initialize and configure the ADS1015
    ADS1015 ads1015(0, 0x48);

    // Put the ADC into differential mode for pins A0 and A1
    ads1015.getSample(ADS1X15::DIFF_0_1);

    // Set the gain based on expected VIN range to -/+ 2.048 V
    // Can be adjusted based on application to as low as -/+ 0.256 V, see API
    // documentation for details
    ads1015.setGain(ADS1X15::GAIN_TWO);

    // Set the sample rate to 3300 samples per second (max) and turn on continuous
    // sampling
    ads1015.setSPS(ADS1015::SPS_3300);
    ads1015.setContinuous(true);

    // Enable exceptions from the output stream
    f.exceptions(ofstream::failbit | ofstream::badbit);
    // Open the file
    try {
        f.open(fileName);

        // Output formatting
        f.setf(ios_base::fixed);
        f.precision(7);

        // Start the thread that will stop logging after 10 seconds
        thread timer(stop);

        // Read sensor data and write it to the output file every ms
        while (running) {
            f << id++ << " " << ads1015.getLastSample() << endl;
            upm_delay_us(1000);
        }

        // Clean-up and exit
        timer.join();
        f.close();
    } catch (ios_base::failure& e) {
        cout << "Failed to write to file: " << e.what() << endl;
        return 1;
    }
    cout << "Wrote " << id << " samples to file: " << fileName << endl;
    //! [Interesting]
    return 0;
}
