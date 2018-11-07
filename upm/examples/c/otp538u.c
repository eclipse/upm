/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
