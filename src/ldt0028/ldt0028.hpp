/*
 * Author: Sarah Knepper <sarah.knepper@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/aio.h>
#include <mraa/initio.hpp>

namespace upm {

/**
 * @brief LDT0-028 Piezo Vibration Sensor
 * @defgroup ldt0028 libupm-ldt0028
 * @ingroup seeed analog flexfor
 */

/**
 * @library ldt0028
 * @sensor ldt0028
 * @comname Piezo Vibration Sensor
 * @altname Grove Piezo Vibration Sensor
 * @type flexfor
 * @man seeed
 * @con analog
 * @web http://wiki.seeed.cc/Grove-Piezo_Vibration_Sensor/
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
         * Instantiates LDT0-028 Sensor object based on a given string.
         *
         * @param initStr string containing specific information for LDT0-028 Sensor initialization.
         */
        LDT0028(std::string initStr);

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
        mraa::MraaIo        mraaIo;
        mraa_aio_context    m_pin;  //!< AIO pin
};

}
