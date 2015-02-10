/*
 * Author: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include "itg3200.h"

int
main(int argc, char **argv)
{
//! [Interesting]
    int16_t *rot;
    float *ang;

    // Note: Sensor not supported on Intel Edison with Arduino breakout
    upm::Itg3200* gyro = new upm::Itg3200(0);

    while(true){
        gyro->update(); // Update the data
        rot = gyro->getRawValues(); // Read raw sensor data
        ang = gyro->getRotation(); // Read rotational speed (deg/sec)
        fprintf(stdout, "Raw: %6d %6d %6d\n", rot[0], rot[1], rot[2]);
        fprintf(stdout, "AngX: %5.2f\n", ang[0]);
        fprintf(stdout, "AngY: %5.2f\n", ang[1]);
        fprintf(stdout, "AngZ: %5.2f\n", ang[2]);
        fprintf(stdout, "Temp: %5.2f Raw: %6d\n", gyro->getTemperature(), gyro->getRawTemp());
        sleep(1);
    }
//! [Interesting]
    return 0;
}
