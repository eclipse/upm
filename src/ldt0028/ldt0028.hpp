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
 * @brief LDT0-028 Piezo Vibration Sensor library
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
 * @brief API for LDT0-028 PZT film-based sensors,
 * such as a Grove Piezo Vibration sensor
 *
 * This module defines the LDT0-028 interface for libupm-ldt0028
 *
 * @image html ldt0028.jpg
 * @snippet ldt0028.cxx Interesting
 */
class LDT0028 {
    public:
        /**
         * LDT0028 constructor
         *
         * @param pin AIO pin where the sensor is connected
         */
        LDT0028(unsigned int pin);

        /**
         * LDT0028 destructor
         */
        ~LDT0028();

        /**
         * Returns the name of this sensor
         *
         * @return Name of this sensor
         */
        std::string name();

        /**
         * Returns one sample from this sensor
         *
         * @return One value from this sensor
         */
        int getSample();

    protected:
        std::string         m_name; //!< name of this sensor
        mraa_aio_context    m_pin;  //!< AIO pin
};

}
