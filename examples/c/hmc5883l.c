/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * Ported based on original C++ code by:
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
