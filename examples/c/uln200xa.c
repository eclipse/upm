/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <stdio.h>

#include <upm_utilities.h>
#include "uln200xa.h"

int main ()
{
//! [Interesting]

    // Instantiate a Stepper motor on a ULN200XA Darlington controller.

    // This was tested with the Grove Gear Stepper Motor with Driver

    // Wire the pins so that I1 is pin D8, I2 is pin D9, I3 is pin D10 and
    // I4 is pin D11
    uln200xa_context motor = uln200xa_init(4096, 8, 9, 10, 11);

    if (!motor)
    {
        printf("uln200xa_init() failed\n");
        return 1;
    }

    uln200xa_set_speed(motor, 5);
    uln200xa_set_direction(motor, ULN200XA_DIR_CW);
    printf("Rotating 1 revolution clockwise.\n");
    uln200xa_stepper_steps(motor, 4096);

    printf("Sleeping for 2 seconds...\n");
    upm_delay(2);

    printf("Rotating 1/2 revolution counter clockwise.\n");
    uln200xa_set_direction(motor, ULN200XA_DIR_CCW);
    uln200xa_stepper_steps(motor, 2048);

    // turn off the power
    uln200xa_release(motor);

    printf("Exiting...\n");

    uln200xa_close(motor);
//! [Interesting]
  return 0;
}
