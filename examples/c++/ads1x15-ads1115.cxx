/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

/* This example demonstrates how to use one of the ADS1115 ADCs on the
 * DFRobot Joule Shield with devices that output a small differential
 * voltage (e.g. geophones, piezoelectric bands or pads, thermocouples).
 */

#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "ads1115.hpp"
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
    string fileName = "./ads1115.data"; // Output filename
    ofstream f;

    // Initialize and configure the ADS1115 for the SM-24 Geophone
    // There are two ADS1115 chips on the DFRobot Joule Shield on the same I2C bus
    //     - 0x48 gives access to pins A0 - A3
    //     - 0x49 gives access to pins A4 - A7
    ADS1115 ads1115(0, 0x48);

    // Put the ADC into differential mode for pins A0 and A1,
    // the SM-24 Geophone is connected to these pins
    ads1115.getSample(ADS1X15::DIFF_0_1);

    // Set the gain based on expected VIN range to -/+ 2.048 V
    // Can be adjusted based on application to as low as -/+ 0.256 V, see API
    // documentation for details
    ads1115.setGain(ADS1X15::GAIN_TWO);

    // Set the sample rate to 860 samples per second (max) and turn on continuous
    // sampling
    ads1115.setSPS(ADS1115::SPS_860);
    ads1115.setContinuous(true);

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
            f << id++ << " " << ads1115.getLastSample() << endl;
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
