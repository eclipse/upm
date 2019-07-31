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
#include <signal.h>

#include "dfrec.h"
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

    // Instantiate a DFRobot EC sensor on analog pin A0, with a ds18b20
    // temperature sensor connected to UART 0, and a device index (for
    // the ds1820b uart bus) of 0, and an analog reference voltage of
    // 5.0.
    dfrec_context sensor = dfrec_init(0, 0, 0, 5.0);

    if (!sensor)
    {
        printf("dfrec_init() failed.\n");
        return(1);
    }

    // Every 2 seconds, update and print values
    while (shouldRun)
    {
        dfrec_update(sensor);

        printf("EC = %f ms/cm\n", dfrec_get_ec(sensor));
        printf("Volts = %f, Temperature = %f C\n",
                dfrec_get_volts(sensor), dfrec_get_temperature(sensor));
        printf("\n");

        upm_delay(2);
    }

    //! [Interesting]

    printf("Exiting...\n");

    dfrec_close(sensor);

    return 0;
}
