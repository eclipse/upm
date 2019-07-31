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

volatile unsigned int counter = 0;

// Our interrupt handler
void rpr220_isr(void *arg)
{
    counter++;
}


int main()
{
    signal(SIGINT, sig_handler);

//! [Interesting]
    // This example uses an interrupt handler to increment a counter

    // Instantiate an RPR220 digital pin D2
    // This was tested on the Grove IR Reflective Sensor
    rpr220_context sensor = rpr220_init(2);

    if (!sensor)
    {
        printf("rpr220_init() failed\n");
        return 1;
    }

    // Here, we setup our Interupt Service Routine (ISR) to count
    // 'black' pulses detected.  We do not pass an argument to the
    // interrupt handler (NULL).

    rpr220_install_isr(sensor, rpr220_isr, NULL);

    while (shouldRun)
    {
        printf("Counter: %d\n", counter);

        upm_delay(1);
    }

    printf("Exiting...\n");

    rpr220_close(sensor);
//! [Interesting]

    return 0;
}
