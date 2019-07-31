/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2017 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include "enc03r.h"

namespace upm {
/**
 * @brief ENC03R Single Axis Gyro
 * @defgroup enc03r libupm-enc03r
 * @ingroup seeed analog compass robok
 */

/**
 * @library enc03r
 * @sensor enc03r
 * @comname Single-axis Analog Gyro Module
 * @altname Grove Single Axis Analog Gyro
 * @type compass
 * @man seeed
 * @con analog
 * @kit robok
 * @web http://www.murata.com/en-us/products/productdetail?partno=ENC-03RC-R
 *
 * @brief API for the ENC03R Single Axis Analog Gyro
 *
 * UPM module for the ENC03R single axis analog gyro.
 * This gyroscope measures x-axis angular velocity, that is
 * how fast the sensor is rotating around the x-axis.
 * Calibration of the sensor is necessary for accurate readings.
 *
 * @image html enc03r.jpg 
 * @snippet enc03r.cxx Interesting
 */
class ENC03R {
    public:
        /**
            * ENC03R sensor constructor
            *
            * @param pin Analog pin to use
            * @param aref Reference voltage to use; default is 5.0 V
            */
        ENC03R(int pin, float aref=5.0);

        /**
            * ENC03R destructor
            */
        ~ENC03R();

        /**
            * Calibrates the sensor by determining an analog reading over many
            * samples with no movement of the sensor. This must be done
            * before attempting to use the sensor.
            *
            * @param samples Number of samples to use for calibration
            */
        void calibrate(unsigned int samples);

        /**
            * Update the internal state with the current reading.  This
            * function must be called prior to calling
            * angularVelocity().
            *
            * @param dev Device context
            */
        void update();

        /**
            * Returns the currently stored calibration value
            *
            * @return Current calibration value
            */
        float calibrationValue() { return enc03r_calibration_value(m_enc03r); };

        /**
            * Computes angular velocity based on the value and stored calibration
            * reference.
            *
            * @param val Value to use to compute angular velocity
            * @return Computed angular velocity
            */
        float angularVelocity();

        /**
            * Set sensor offset.  The offset is applied to the return value
            * before scaling. Default is 0.
            *
            * @param offset Offset to apply to value
            */
        void setOffset(float offset);

        /**
            * Set sensor scale.  The return value is scaled by this value
            * after the offset is applied.  Default is 1.0.
            *
            * @param scale Scale to apply to value
            */
        void setScale(float scale);

        /**
            * Get a normalized ADC value from the sensor.  The return
            * value will be between 0.0 (indicating no voltage) and 1.0
            * indicating max voltage (aref).  update() must be called
            * prior to calling this function.
            *
            * @return The normalized reading from the ADC.
            */
        float getNormalized();

    protected:
        enc03r_context m_enc03r;

    private:
        /* Disable implicit copy and assignment operators */
        ENC03R(const ENC03R&) = delete;
        ENC03R &operator=(const ENC03R&) = delete;
};
}
