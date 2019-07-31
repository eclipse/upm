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

  md_context motors = md_init(MD_I2C_BUS, MD_DEFAULT_I2C_ADDR);

  if (!motors)
  {
      printf("md_init() failed\n");
      return 1;
  }

  // set direction to CW and set speed to 50%
  printf("Spin M1 and M2 at half speed for 3 seconds\n");
  md_set_motor_directions(motors, MD_DIR_CW, MD_DIR_CW);
  md_set_motor_speeds(motors, 127, 127);

  upm_delay(3);
  // counter clockwise
  printf("Reversing M1 and M2 for 3 seconds\n");
  md_set_motor_directions(motors, MD_DIR_CCW, MD_DIR_CCW);
  upm_delay(3);

  printf("Stopping motors\n");
  md_set_motor_speeds(motors, 0, 0);

  printf("Exiting...\n");

  md_close(motors);
//! [Interesting]
  return 0;
}
