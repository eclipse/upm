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

#include <upm_utilities.h>
#include <ecezo.h>

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

    // Instantiate a ECEZO sensor on uart 0 at 9600 baud.
    ecezo_context sensor = ecezo_uart_init(0, 9600);

    // For I2C, assuming the device is configured for address 0x64 on
    // I2C bus 0, you could use something like:
    //
    // ecezo_context sensor = ecezo_i2c_init(0, 0x64);

    if (!sensor)
    {
        printf("ecezo_init() failed.\n");
        return 1;
    }

    while (shouldRun)
    {
        // this will take about 1 second to complete
        if (ecezo_update(sensor))
        {
            printf("ecezo_update() failed\n");
        }
        else
        {
            printf("EC %f uS/cm, TDS %f mg/L, Salinity %f PSS-78, SG %f\n",
                   ecezo_get_ec(sensor),
                   ecezo_get_tds(sensor),
                   ecezo_get_salinity(sensor),
                   ecezo_get_sg(sensor));
        }

        upm_delay(5);
    }

    //! [Interesting]

    printf("Exiting\n");

    ecezo_close(sensor);

    return 0;
}
