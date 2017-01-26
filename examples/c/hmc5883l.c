/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Ported based on original C++ code by:
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
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
#include <stdio.h>
#include <signal.h>

#include <upm_utilities.h>

#include "hmc5883l.h"

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    // Instantiate an HMC8883L on I2C bus 0
    hmc5883l_context sensor = hmc5883l_init(0);

    if (!sensor)
    {
        printf("%s: hmc5883l_init() failed\n", __FUNCTION__);
        return 1;
    }

    hmc5883l_set_declination(sensor, 0.2749); // Set your declination
                                              // from true north in
                                              // radians

    // Print out the coordinates, heading, and direction every second
    while (shouldRun)
    {
        hmc5883l_update(sensor); // Update the coordinates
        const int16_t *pos = hmc5883l_coordinates(sensor);

        printf("coor: %5d %5d %5d ", pos[0], pos[1], pos[2]);
        printf("heading: %5.2f direction: %3.2f\n",
               hmc5883l_heading(sensor), hmc5883l_direction(sensor));

        upm_delay(1);
    }

    hmc5883l_close(sensor);

//! [Interesting]

    return 0;
}
