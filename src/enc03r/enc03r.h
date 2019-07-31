/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2017 Intel Corporation.
emacs . *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <mraa/aio.h>
#include <upm.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * @file enc03r.h
     * @library enc03r
     * @brief Generic API for AT command based UART devices
     *
     */

    /**
     * Device context
     */
    typedef struct _enc03r_context {
        mraa_aio_context aio;

        // determined by calibrate();
        float calibrationValue;

        // our computed value
        float angular_velocity;

        // analog reference voltage
        float a_ref;
        // analog ADC resolution (max value)
        float a_res;

        // offset
        float offset;

        // scale
        float scale;

        // normalized ADC value
        float normalized;

    } *enc03r_context;

    /**
     * ENC03R sensor constructor
     *
     * @param pin Analog pin to use
     * @param vref Reference voltage to use; default is 5.0 V
     * @return Device context
     */
    enc03r_context enc03r_init(int pin, float aref);

    /**
     * ENC03R destructor
     *
     * @param dev Device context
     */
    void enc03r_close(enc03r_context dev);

    /**
     * Update the internal state with the current reading.  This
     * function must be called prior to calling
     * enc03r_angular_velocity().
     *
     * @param dev Device context
     * @return UPM result
     */
    upm_result_t enc03r_update(enc03r_context dev);

    /**
     * Calibrates the sensor by determining an analog reading over many
     * samples with no movement of the sensor. This must be done
     * before attempting to use the sensor.
     *
     * @param dev Device context
     * @param samples Number of samples to use for calibration
     * @return UPM result
     */
    upm_result_t enc03r_calibrate(const enc03r_context dev,
                                  unsigned int samples);

    /**
     * Returns the currently stored calibration value
     *
     * @param dev Device context
     * @return Current calibration value
     */
    float enc03r_calibration_value(const enc03r_context dev);

    /**
     * Return the computed Angular Velocity in degrees per second.
     * You must have called encr03r_update() prior to calling this
     * function.
     *
     * @param dev Device context
     * @return Computed angular velocity
     */
    float enc03r_angular_velocity(const enc03r_context dev);

    /**
     * Set sensor offset.  The offset is applied to the return value
     * before scaling. Default is 0.
     *
     * @param dev Device context
     * @param offset Offset to apply to value
     */
    void enc03r_set_offset(const enc03r_context dev, float offset);

    /**
     * Set sensor scale.  The return value is scaled by this value
     * after the offset is applied.  Default is 1.0.
     *
     * @param dev Device context
     * @param scale Scale to apply to value
     */
    void enc03r_set_scale(const enc03r_context dev, float scale);

    /**
     * Get a normalized ADC value from the sensor.  The return value
     * will be between 0.0 (indicating no voltage) and 1.0 indicating
     * max voltage (aref).  encr03r_update() must be called prior to
     * calling this function.
     *
     * @param dev Device context
     * @return The normalized reading from the ADC.
     */
    float enc03r_get_normalized(const enc03r_context dev);

#ifdef __cplusplus
}
#endif
