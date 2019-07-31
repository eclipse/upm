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

#include "bh1750.h"
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

    // Instantiate a BH1750 sensor on default I2C bus (0), using the
    // default I2C address (0x23), and setting the mode to highest
    // resolution, lowest power mode.
    bh1750_context sensor = bh1750_init(BH1750_DEFAULT_I2C_BUS,
            BH1750_DEFAULT_I2C_ADDR,
            BH1750_OPMODE_H2_ONCE);

    if (!sensor)
    {
        printf("bh1750_init() failed.\n");
        return 1;
    }

    // Every second, sample the BH1750 and output the measured lux value

    while (shouldRun)
    {
        float lux;

        bh1750_get_lux(sensor, &lux);
        printf("Detected Light Level (lux): %f\n", lux);
        upm_delay(1);
    }

    //! [Interesting]

    printf("Exiting\n");

    bh1750_close(sensor);

    return 0;
}
