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
#include <signal.h>

#include "bmi160.h"
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

    // Instantiate a BMI160 sensor on I2C bus 0, address 0x69, and
    // disable the magnetometer (since it is an optional component
    // that may not be present).  Since we are using I2C, pass a valid
    // I2C address, and supply -1 as the cs_pin.
    bmi160_context sensor = bmi160_init(0, 0x69, -1, false);

    // For SPI, you would use something like this, using gpio 10 as
    // the cs pin.  Passing -1 as the address indicates SPI operation.
    //
//    bmi160_context sensor = bmi160_init(0, -1, 10, false);

    if (!sensor)
    {
        printf("bmi160_init() failed.\n");
        return(1);
    }

    // Every half second, update and print values

    while (shouldRun)
    {
        bmi160_update(sensor);

        float x, y, z;

        bmi160_get_accelerometer(sensor, &x, &y, &z);
        printf("Acceleration x = %f y = %f z = %f\n",
               x, y, z);

        bmi160_get_gyroscope(sensor, &x, &y, &z);
        printf("Gyroscope    x = %f y = %f z = %f\n",
               x, y, z);

        //bmi160_get_magnetometer(sensor, &x, &y, &z);
        //printf("Magnetometer x = %f y = %f z = %f\n",
        //       x, y, z);

        printf("\n");

        upm_delay_ms(500);
    }

    //! [Interesting]

    printf("Exiting...\n");

    bmi160_close(sensor);

    return 0;
}
