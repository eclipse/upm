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

#pragma once

#include <stdint.h>
#include <upm.h>
#include <mraa/i2c.h>
#include <mraa/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file mb704x.h
     * @library mb704x
     * @brief C API for the MB704x MaxSonar-WR Ultrasonic Ranger
     *
     * @include mb704x.c
     */

    /**
     * Device context
     */
    typedef struct _mb704x_context {
        mraa_i2c_context         i2c;
    } *mb704x_context;

    /**
     * MB704X Initializer
     *
     * @param bus Specify which the I2C bus to use.
     * @param addr Specify the I2C address to use.  The default is 112.
     * @return an initialized device context on success, NULL on error.
     */
    mb704x_context mb704x_init(unsigned int bus, int addr);

    /**
     * MB704X sensor close function
     */
    void mb704x_close(mb704x_context dev);

    /**
     * Query the device for a range reading.  The range will be
     * reported in centimeters (cm).
     *
     * @param dev Device context
     * @return Measured range, -1 on error. The range is reported in
     * centimeters (cm).
     */
    int mb704x_get_range(const mb704x_context dev);


#ifdef __cplusplus
}
#endif
