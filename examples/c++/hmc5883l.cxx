/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Contributions: Mihai Tudor Panu <mihai.tudor.panu@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>

#include "hmc5883l.hpp"
#include "upm_utilities.h"

int
main(int argc, char** argv)
{
    //! [Interesting]
    // Instantiate on I2C
    upm::Hmc5883l compass(0);

    compass.set_declination(0.2749); // Set your declination from
    // true north in radians

    // Print out the coordinates, heading, and direction every second
    while (true) {
        compass.update(); // Update the coordinates
        const int16_t* pos = compass.coordinates();
        fprintf(stdout, "coor: %5d %5d %5d ", pos[0], pos[1], pos[2]);
        fprintf(stdout, "heading: %5.2f direction: %3.2f\n", compass.heading(), compass.direction());
        upm_delay(1);
    }
    //! [Interesting]

    return 0;
}
