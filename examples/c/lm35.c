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
#include "lm35.h"
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

  // Instantiate a LM35 on analog pin A0, with a default analog
  // reference voltage of 5.0
  lm35_context sensor = lm35_init(0, 5.0);

  // Every half second, sample the sensor and output the temperature

  while (shouldRun)
    {
        float temp;
        if (lm35_get_temperature(sensor, &temp) != UPM_SUCCESS)
        {
            printf("lm35_get_temperature failed\n");
            return 1;
        }

        printf("Temperature: %3.2f C\n", temp);

        upm_delay_ms(500);
    }

//! [Interesting]

  printf("Exiting\n");

  lm35_close(sensor);

  return 0;
}
