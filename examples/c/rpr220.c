/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
#include <rpr220.h>

int shouldRun = true;

void sig_handler(int signo)
{
    if (signo == SIGINT)
        shouldRun = false;
}


int main()
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    // This example uses a simple method to determine current status

    // Instantiate an RPR220 digital pin D2
    // This was tested on the Grove IR Reflective Sensor
    rpr220_context sensor = rpr220_init(2);

    if (!sensor)
    {
        printf("rpr220_init() failed\n");
        return 1;
    }

    while (shouldRun)
    {
        if (rpr220_black_detected(sensor))
            printf("Black detected\n");
        else
            printf("Black NOT detected\n");

        upm_delay_ms(100);           // 100ms
    }

    printf("Exiting...\n");

    rpr220_close(sensor);
//! [Interesting]
    return 0;
}
