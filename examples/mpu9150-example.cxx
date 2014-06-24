/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
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
#include "mpu9150.h"

int
main(int argc, char **argv)
{
    //! [Interesting]
    upm::Vector3D data;
    upm::MPU9150 *sensor = new upm::MPU9150(0, ADDR);
    sensor->getData ();
    sensor->getAcceleromter (&data);
    std::cout << "*************************************************" << std::endl;
    std::cout << "DEVICE ID (" << (int) sensor->getDeviceID () << ")" << std::endl;
    std::cout << "*************************************************" << std::endl;
    std::cout << "ACCELEROMETER :: X (" << data.axisX << ")" << " Y (" << data.axisY << ")"
                << " Z (" << data.axisZ << ")" << std::endl;

    sensor->getGyro (&data);
    std::cout << "GYRO :: X (" << data.axisX << ")" << " Y (" << data.axisY << ")"
                << " Z (" << data.axisZ << ")" << std::endl;

    sensor->getMagnometer (&data);
    std::cout << "MAGNOMETER :: X (" << data.axisX << ")" << " Y (" << data.axisY << ")"
                << " Z (" << data.axisZ << ")" << std::endl;
    std::cout << "TEMPERATURE (" << sensor->getTemperature () << ")" << std::endl;
    std::cout << "*************************************************" << std::endl;
    //! [Interesting]

    std::cout << "exiting application" << std::endl;

    delete sensor;

    return 0;
}
