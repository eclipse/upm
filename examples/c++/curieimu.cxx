/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Author: Ron Evans (@deadprogram)
 * Author: Justin Zemlyansky (@JustInDevelopment)
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include "curieimu.hpp"
#include "upm_utilities.h"
#include "mraa.h"
#include "mraa/firmata.h"
#include <iostream>
#include <math.h>
#include <unistd.h>

int
main(int argc, char** argv)
{
    //! [Interesting]
    mraa_init();
    mraa_add_subplatform(MRAA_GENERIC_FIRMATA, "/dev/ttyACM0");

    upm::CurieImu sensor;

    std::cout << "temperature is: " << (sensor.getTemperature() * pow(0.5, 9) + 23) << std::endl;

    sensor.updateAccel();
    int x = sensor.getAccelX(), y = sensor.getAccelY(), z = sensor.getAccelZ();
    printf("accelerometer is: %d, %d, %d\n", x, y, z);

    sensor.updateGyro();
    int a = sensor.getGyroX(), b = sensor.getGyroY(), c = sensor.getGyroZ();
    printf("gyroscope is: %d, %d, %d\n", a, b, c);

    int axis, direction;
    sensor.enableShockDetection(true);
    for (int i = 0; i < 300; i++) {
        if (sensor.isShockDetected()) {
            sensor.getNextShock();
            axis = sensor.getAxis();
            direction = sensor.getDirection();
            printf("shock data is: %d, %d\n", axis, direction);
        }
        upm_delay_us(10000);
    }

    sensor.updateMotion();
    int m = sensor.getAccelX(), n = sensor.getAccelY(), o = sensor.getAccelZ(),
        p = sensor.getGyroX(), q = sensor.getGyroY(), r = sensor.getGyroZ();
    printf("motion is: %d, %d, %d, %d, %d, %d\n", m, n, o, p, q, r);

    //! [Interesting]

    return 0;
}
