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

#include <stdio.h>

#include "adxl345.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    //! [Interesting]
    int16_t* raw;
    float* acc;

    // Note: Sensor only works at 3.3V on the Intel Edison with Arduino breakout
    upm::Adxl345 accel(0);

    while (true) {
        accel.update();                // Update the data
        raw = accel.getRawValues();    // Read raw sensor data
        acc = accel.getAcceleration(); // Read acceleration (g)
        fprintf(stdout, "Current scale: 0x%2xg\n", accel.getScale());
        fprintf(stdout, "Raw: %6d %6d %6d\n", raw[0], raw[1], raw[2]);
        fprintf(stdout, "AccX: %5.2f g\n", acc[0]);
        fprintf(stdout, "AccY: %5.2f g\n", acc[1]);
        fprintf(stdout, "AccZ: %5.2f g\n", acc[2]);
        upm_delay(1);
    }
    //! [Interesting]
    return 0;
}
