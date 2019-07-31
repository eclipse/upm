/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
