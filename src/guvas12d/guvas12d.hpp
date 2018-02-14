/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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

#include <guvas12d.h>

namespace upm {
    /**
     * @brief GUVA-S12D UV Sensors
     * @defgroup guvas12d libupm-guvas12d
     * @ingroup seeed analog light eak
     */

    /**
     * @library guvas12d
     * @sensor guvas12d
     * @comname Analog UV Sensor
     * @altname GUVA-S12D UV Sensor
     * @type light
     * @man seeed
     * @con analog
     * @web http://wiki.seeed.cc/Grove-UV_Sensor/
     * @kit eak
     *
     * @brief API for the GUVA-S12D UV Sensor
     *
     * UPM module for the GUVA-S12D UV sensor
     *
     * @image html guvas12d.jpg
     * @snippet guvas12d.cxx Interesting
     */
    class GUVAS12D {
    public:

        /**
         * GUVA-S12D UV sensor constructor
         *
         * @param pin Analog pin to use
         * @param aref Analog reference voltage to use
         */
        GUVAS12D(int pin, float aref=5.0);

        /**
         * GUVAS12D destructor
         */
        ~GUVAS12D();

        /**
         * @deprecated This method is being replaced by the volts()
         * and illumination() methods.
         *
         * Gets the average voltage value from the sensor
         *
         * @param aref Reference voltage in use (usually 5.0 V or 3.3 V)
         * @param samples Number of samples to average over (currently
         * ignored)
         * @return Average voltage reading
         *
         */
        float value(float aref, unsigned int samples);

        /**
         * Gets the voltage value from the sensor
         *
         * @return Voltage reading
         *
         */
        float volts();

        /**
         * Gets the computed illumination intensity from the sensor in
         * mW/m^2.
         *
         * @return Intensity over the sensitive wavelengths in mW/m^2
         *
         */
        float intensity();

        /**
         * Set sensor scale.  This scale is applied to the intensity value
         * before the offset is applied.  Default is 1.0.
         *
         * @param scale Scale to apply.
         */
        void setScale(float scale);

        /**
         * Set sensor offset.  This offset is applied to the intensity
         * value before scaling.  Default is 0.0.
         *
         * @param offset Offset to apply.
         */
        void setOffset(float offset);

    private:
        /* Disable implicit copy and assignment operators */
        GUVAS12D(const GUVAS12D&) = delete;
        GUVAS12D &operator=(const GUVAS12D&) = delete;

        guvas12d_context m_guvas12d;
    };
}
