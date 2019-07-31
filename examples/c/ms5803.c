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
#include <signal.h>

#include "ms5803.h"
#include "upm_utilities.h"

bool shouldRun = true;

void sig_handler(int signo)
{
  if (signo == SIGINT)
    shouldRun = false;
}

int main()
{
  signal(SIGINT, sig_handler);

//! [Interesting]

  // Instantiate a MS5803 on analog I2C bus 0, at the default address
  ms5803_context sensor = ms5803_init(0, MS5803_DEFAULT_I2C_ADDR, -1);

  if (!sensor)
  {
      printf("ms5803_init() failed\n");
      return 1;
  }

  // Every second, sample the sensor and output the pressure and
  // temperature

  while (shouldRun)
    {
        if (ms5803_update(sensor))
        {
            printf("ms5803_update() failed\n");
        }

        printf("Temperature: %f C, Pressure = %f mbar\n",
               ms5803_get_temperature(sensor),
               ms5803_get_pressure(sensor));

        upm_delay(1);
    }

//! [Interesting]

  printf("Exiting\n");

  ms5803_close(sensor);

  return 0;
}
