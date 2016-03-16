/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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

#include <unistd.h>
#include <iostream>
#include "curieimu.hpp"
#include "mraa.h"
#include "mraa/firmata.h"
#include <math.h>

int
main(int argc, char **argv)
{
    //! [Interesting]
    mraa_init();
    mraa_add_subplatform(MRAA_GENERIC_FIRMATA, "/dev/ttyACM0");

    upm::CurieImu* sensor = new upm::CurieImu();

    std::cout << "temperature is: " << (sensor->getTemperature() * pow(0.5, 9) + 23) << std::endl;

    int x, y, z;
    sensor->readAccelerometer(&x, &y, &z);
    printf("accelerometer is: %d, %d, %d\n", x, y, z);

    int a, b, c;
    sensor->readGyro(&a, &b, &c);
    printf("gyroscope is: %d, %d, %d\n", a, b, c);

    int axis, direction;
    sensor->enableShockDetection(true);
    for(int i=0; i<300; i++) {
      if (sensor->isShockDetected()) {
        sensor->getShockDetectData(&axis, &direction);
        printf("shock data is: %d, %d\n", axis, direction);
      }
      usleep(10000);
    }

    int m, n, o, p, q, r;
    sensor->readMotion(&m, &n, &o, &p, &q, &r);
    printf("motion is: %d, %d, %d, %d, %d, %d\n", m, n, o, p, q, r);

    delete sensor;

    return 0;
}
