/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string>

#include "tp401.hpp"
#include "upm_utilities.h"

using namespace std;

//! [Interesting]
// Give a qualitative meaning to the value from the sensor
std::string
airQuality(uint16_t value)
{
    if (value < 50)
        return "Fresh Air";
    if (value < 200)
        return "Normal Indoor Air";
    if (value < 400)
        return "Low Pollution";
    if (value < 600)
        return "High Pollution - Action Recommended";
    return "Very High Pollution - Take Action Immediately";
}

int
main()
{
    upm::TP401 airSensor(0);

    cout << airSensor.name() << endl;

    fprintf(stdout, "Heating sensor for 3 minutes...\n");
    // wait 3 minutes for sensor to warm up
    for (int i = 0; i < 3; i++) {
        if (i) {
            fprintf(stdout, "Please wait, %d minute(s) passed..\n", i);
        }
        upm_delay(60);
    }
    fprintf(stdout, "Sensor ready!\n");

    while (true) {
        uint16_t value = airSensor.getSample(); // Read raw value
        float ppm = airSensor.getPPM(); // Read CO ppm (can vary slightly from previous read)
        fprintf(stdout, "raw: %4d ppm: %5.2f   %s\n", value, ppm, airQuality(value).c_str());
        upm_delay_us(2500000); // Sleep for 2.5s
    }

    return 0;
}
//! [Interesting]
