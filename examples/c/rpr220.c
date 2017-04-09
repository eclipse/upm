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
