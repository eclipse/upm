/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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

#include <iostream>

//! [Interesting]
#include "lsm303.h"

int
main(int argc, char **argv)
{
	// Instantiate LSM303 compass on I2C
    upm::LSM303 *sensor = new upm::LSM303(0);

    // Get the coordinate data
    sensor->getCoordinates();
    int16_t* coor = sensor->getRawCoorData(); // in XYZ order. 
    // The sensor returns XZY, but the driver compensates and makes it XYZ
    
    // Print out the X, Y, and Z coordinate data using two different methods
    std::cout << "coor: rX " << (int)coor[0]
              << " - rY " << (int)coor[1]
              << " - rZ " << (int)coor[2]
              << std::endl;
    std::cout << "coor: gX " << sensor->getCoorX()
              << " - gY " << sensor->getCoorY()
              << " - gZ " << sensor->getCoorZ()
              << std::endl;

    // Get and print out the heading
    std::cout << "heading: "
              << sensor->getHeading()
              << std::endl;

    // Get the acceleration
    sensor->getAcceleration();
    int16_t* accel = sensor->getRawAccelData();
    // Print out the X, Y, and Z acceleration data using two different methods
    std::cout << "acc: rX " << (int)accel[0]
              << " - rY " << (int)accel[1]
              << " - Z " << (int)accel[2]
              << std::endl;
    std::cout << "acc: gX " << sensor->getAccelX()
              << " - gY " << sensor->getAccelY()
              << " - gZ " << sensor->getAccelZ()
              << std::endl;

    return 0;
}
//! [Interesting]
