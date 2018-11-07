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
     * @file guvas12d.h
     * @library guvas12d
     * @brief C API for the guvas12d driver
     *
     * @include guvas12d.c
     */

    /**
     * Device context
     */
    typedef struct _guvas12d_context {
        mraa_aio_context    aio;

        // ADC reference voltage
        float               aref;
        // ADC resolution
        float               ares;

        float               scale;
        float               offset;
    } *guvas12d_context;

    /**
     * GUVA-S12D UV sensor constructor
     *
     * @param pin Analog pin to use
     */
    guvas12d_context guvas12d_init(int pin, float aref);

    /**
     * GUVAS12D destructor
     */
    void guvas12d_close(guvas12d_context dev);

    /**
     * Gets the voltage value from the sensor
     *
     * @param volts Pointer to average voltage reading
     * @return UPM status.
     */
    upm_result_t guvas12d_get_volts(const guvas12d_context dev, float *volts);

    /**
     * Gets the illumination intensity in mW/m^2
     *
     * @param volts Pointer to average voltage reading
     * @return UPM status.
     * @return illumination intensity in mW/m^2
     */
    upm_result_t guvas12d_get_intensity(const guvas12d_context dev,
                                        float *intensity);

    /**
     * Set sensor offset.  This offset is applied to the illumination
     * intensity value before scaling.  Default is 0.0.
     *
     * @param dev sensor context pointer
     * @param offset Offset to apply.
     */
    void guvas12d_set_offset(const guvas12d_context dev, float offset);

    /**
     * Set sensor scale.  This offset is applied to the illumination
     * intensity value before scaling.  Default is 1.0.
     *
     * @param dev sensor context pointer
     * @param scale Scale to apply.
     */
    void guvas12d_set_scale(const guvas12d_context dev, float scale);

#ifdef __cplusplus
}
#endif
