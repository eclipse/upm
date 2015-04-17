/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 Intel Corporation.
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

#include <string>
#include <mraa/aio.h>

namespace upm {

/**
 * @brief LDT0-028 Piezo Vibration sensor library
 * @defgroup ldt0028 libupm-ldt0028
 * @ingroup seeed analog flexfor
 */

/**
 * @library ldt0028
 * @sensor ldt0028
 * @comname LDT0-028 Piezo Vibration Sensor
 * @altname Grove Piezo Vibration Sensor
 * @type flexfor
 * @man seeed
 * @con analog
 *
 * @brief C++ API for LDT0-028 PZT film-based sensors,
 * such as the Grove Piezo Vibration sensor
 *
 * This file defines the LDT0028 C++ interface for libupm-ldt0028
 *
 * @snippet ldt0028.cxx Interesting
 */
class LDT0028 {
    public:
        /**
         * LDT0028 Piezo Vibration sensor constructor
         *
         * @param pin AIO pin where sensor is connected
         */
        LDT0028(unsigned int pin);

        /**
         * LDT0028 destructor
         */
        ~LDT0028();

        /**
         * Return name of this sensor
         *
         * @return the name of this sensor
         */
        std::string name();

        /**
         * Return one sample from this sensor
         *
         * @return one value from this sensor
         */
        int getSample();

    protected:
        std::string         m_name; //!< name of this sensor
        mraa_aio_context    m_pin;  //!< AIO pin
};

}
