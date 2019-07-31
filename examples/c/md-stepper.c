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
