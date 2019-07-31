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

#include "itg3200.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    //! [Interesting]
    int16_t* rot;
    float* ang;

    // Note: Sensor not supported on Intel Edison with Arduino breakout
    upm::Itg3200 gyro(0);

    while (true) {
        gyro.update();             // Update the data
        rot = gyro.getRawValues(); // Read raw sensor data
        ang = gyro.getRotation();  // Read rotational speed (deg/sec)
        fprintf(stdout, "Raw: %6d %6d %6d\n", rot[0], rot[1], rot[2]);
        fprintf(stdout, "AngX: %5.2f\n", ang[0]);
        fprintf(stdout, "AngY: %5.2f\n", ang[1]);
        fprintf(stdout, "AngZ: %5.2f\n", ang[2]);
        fprintf(stdout, "Temp: %5.2f Raw: %6d\n", gyro.getTemperature(), gyro.getRawTemp());
        upm_delay(1);
    }
    //! [Interesting]
    return 0;
}
