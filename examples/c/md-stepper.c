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

#include <md.h>
#include <upm_utilities.h>

int main(int argc, char **argv)
{
  //! [Interesting]
  // Instantiate an I2C Motor Driver on I2C bus 0

  md_context stepper = md_init(MD_I2C_BUS, MD_DEFAULT_I2C_ADDR);

  if (!stepper)
  {
      printf("md_init() failed\n");
      return 1;
  }

  // This example demonstrates using the MD to drive a stepper motor

  // configure it, for this example, we'll assume 200 steps per rev
  md_config_stepper(stepper, 200, MD_STEP_MODE1);

  // set for half a rotation
  md_set_stepper_steps(stepper, 100);

  // let it go - clockwise rotation, 10 RPM speed
  md_enable_stepper(stepper, MD_STEP_DIR_CW, 10);

  upm_delay(3);

  // Now do it backwards...
  md_set_stepper_steps(stepper, 100);
  md_enable_stepper(stepper, MD_STEP_DIR_CCW, 10);

  // now disable
  md_disable_stepper(stepper);

  printf("Exiting...\n");

  md_close(stepper);
  //! [Interesting]
  return 0;
}
