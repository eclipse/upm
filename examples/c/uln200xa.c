/*
 * Author: Jon Trulson <jtrulson@ics.com>
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
