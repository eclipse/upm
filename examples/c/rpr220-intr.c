/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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
