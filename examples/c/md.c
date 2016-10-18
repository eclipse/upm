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
