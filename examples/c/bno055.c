/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2017 Intel Corporation.
 *
 * The MIT License
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <upm_utilities.h>

#include <signal.h>
#include "bno055.h"

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

  // Initialize a BNO055 using default parameters (bus 0, addr
  // 0x28).  The default running mode is NDOF absolute orientation
  // mode.
  bno055_context sensor = bno055_init(0, BNO055_DEFAULT_ADDR, NULL);

  if (!sensor)
  {
      printf("bno055_init() failed.\n");
      return 1;
  }

  // First we need to calibrate....
  printf("First we need to calibrate.  4 numbers will be output every \n"
         "second for each sensor.  0 means uncalibrated, and 3 means \n"
         "fully calibrated.\n"
         "See the UPM documentation on this sensor for instructions on \n"
         "what actions are required to calibrate.\n");

  // do the calibration...
  while (shouldRun && !bno055_is_fully_calibrated(sensor))
    {
      int mag, acc, gyr, sys;
      bno055_get_calibration_status(sensor, &mag, &acc, &gyr, &sys);

      printf("Magnetometer: %d Accelerometer: %d Gyroscope: %d System: %d\n",
             mag, acc, gyr, sys);

      upm_delay(1);
    }

  printf("\nCalibration complete.\n\n");

  // now output various fusion data every 250 milliseconds
  while (shouldRun)
    {
      float w, x, y, z;

      if (bno055_update(sensor))
      {
          printf("bno055_update() failed.\n");
          return 1;
      }

      bno055_get_euler_angles(sensor, &x, &y, &z);
      printf("Euler: Heading: %f Roll: %f Pitch: %f degrees\n",
             x, y, z);

      bno055_get_quaternions(sensor, &w, &x, &y, &z);
      printf("Quaternion: W: %f X: %f Y: %f Z: %f\n",
             w, x, y, z);

      bno055_get_linear_acceleration(sensor, &x, &y, &z);
      printf("Linear Acceleration: X: %f Y: %f Z: %f m/s^2\n",
             x, y, z);

      bno055_get_gravity_vectors(sensor, &x, &y, &z);
      printf("Gravity Vector: X: %f Y: %f Z: %f m/s^2\n",
             x, y, z);

      printf("\n");
      upm_delay_ms(250);
    }

//! [Interesting]

  printf("Exiting...\n");

  bno055_close(sensor);

  return 0;
}
