/*
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
#include "hmc5883l.h"

int
main(int argc, char **argv)
{
//! [Interesting]
    // Instantiate on I2C
    upm::Hmc5883l* compass = new upm::Hmc5883l(0);
    int16_t *pos;

    compass->set_declination(0.2749); // Set your declination from true north in radians

    // Print out the coordinates, heading, and direction every second
    while(true){
        compass->update(); // Update the coordinates
        pos = compass->coordinates();
        fprintf(stdout, "coor: %5d %5d %5d ", pos[0], pos[1], pos[2]);
        fprintf(stdout, "heading: %5.2f direction: %3.2f\n", compass->heading(), compass->direction());
        sleep(1);
    }
//! [Interesting]

    return 0;
}
