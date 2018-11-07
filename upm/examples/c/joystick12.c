/*
 * Author: Noel Eck <noel.eck@intel.com>
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
#include <signal.h>

#include "joystick12.h"
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

    // Instantiate a joystick12 sensor
    //    X = Analog pin A0
    //    Y = Analog pin A1
    joystick12_context sensor = joystick12_init(0, 1);

    if (!sensor)
    {
        printf("joystick12_init() failed.\n");
        return -1;
    }

    printf("Press any key to zero joystick...\n");
    getchar();

    // Zero x and y axis
    joystick12_zero(sensor);

    printf("Hold the X axis to the far neg or pos value, then press a key...\n");
    getchar();
    joystick12_calibrate_x(sensor);
    printf("Hold the Y axis to the far neg or pos value, then press a key...\n");
    getchar();
    joystick12_calibrate_y(sensor);

    // Every half a second, sample the sensor output
    float x, y;
    while (shouldRun)
    {
        joystick12_get_value_x(sensor, &x);
        joystick12_get_value_y(sensor, &y);

        printf("X: %5.02f Y: %5.02f\n", x, y);
        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting\n");

    joystick12_close(sensor);

    return 0;
}
