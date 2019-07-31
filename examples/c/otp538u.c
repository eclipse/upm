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

#include <upm_utilities.h>
#include <otp538u.h>


bool shouldRun = true;

// analog voltage, usually 3.3 or 5.0
#define OTP538U_AREF   5.0

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}

int main()
{
    signal(SIGINT, sig_handler);

//! [Interesting]

    // Instantiate a OTP538U on analog pins A0 and A1
    // A0 is used for the Ambient Temperature and A1 is used for the
    // Object temperature.
    otp538u_context temps = otp538u_init(0, 1, OTP538U_AREF);

    // enable debugging if you would like
    // otp538u_set_debug(temps, true);

    // Output ambient and object temperatures
    while (shouldRun)
    {
        float ambient = 0, object = 0;

        if (otp538u_get_ambient_temperature(temps, &ambient))
            printf("otp538u_get_ambient_temperature() failed\n");
        else if (otp538u_get_object_temperature(temps, &object))
            printf("otp538u_get_object_temperature() failed\n");
        else
            printf("Ambient temp: %f C, Object temp: %f C\n",
                   ambient, object);

        printf("\n");
        upm_delay(1);
    }

    printf("Exiting\n");

    otp538u_close(temps);

//! [Interesting]
    return 0;
}
