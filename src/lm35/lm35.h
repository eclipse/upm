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

#include <stdlib.h>
#include <stdio.h>
#include <upm.h>
#include <mraa/aio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file lm35.h
     * @library lm35
     * @brief C API for the LM35 Temperature Sensor
     *
     * @include lm35.c
     */

    /**
     * device context
     */
    typedef struct _lm35_context
    {
        mraa_aio_context    aio;

        // ADC reference voltage
        float               aref;
        // ADC resolution
        float               ares;

        // scale and offset
        float               scale;
        float               offset;
    } *lm35_context;

    /**
     * LM35 initialization.
     *
     * @param pin Analog pin to use
     * @param aref Analog reference voltage
     */
    lm35_context lm35_init(int pin, float aref);

    /**
     * LM35 close.
     */
    void lm35_close(lm35_context dev);

    /**
     * Returns the temperature in degrees Celsius
     *
     * @param temperature A pointer to a float that will contain the
     * measured temperature.
     * @return UPM status
     */
    upm_result_t lm35_get_temperature(const lm35_context dev,
                                      float *temperature);

    /**
     * Set sensor offset.  This offset is applied to the return values
     * before scaling.  Default is 0.0.
     *
     * @param dev sensor context pointer
     * @param offset Offset to apply.
     */
    void lm35_set_offset(const lm35_context dev, float offset);

    /**
     * Set sensor scale.  This scale is applied to the return values
     * before the offset is applied.  Default is 1.0.
     *
     * @param dev sensor context pointer
     * @param scale Scale to apply.
     */
    void lm35_set_scale(const lm35_context dev, float scale);


#ifdef __cplusplus
}
#endif
