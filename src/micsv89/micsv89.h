/*
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation.
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

#include <iostream>
#include <string>
#include "mraa.hpp"
#include "mraa/i2c.hpp"

namespace upm {
  /**
   * @brief MICS-VZ89 environmental sensor library
   * @defgroup micsv89 libupm-micsv89
   * @ingroup generic i2c gaseous
   */
  /**
   * @library micsv89
   * @sensor micsv89
   * @comname MICS-VZ89 Gas Sensor
   * @type gaseous
   * @man generic
   * @con i2c
   * @web http://sgx.cdistore.com/datasheets/e2v/MiCS-VZ-86%20and%20VZ-89%20rev%204.pdf
   * @web http://www.sgxsensortech.com/content/uploads/2015/01/MICS-VZ-89-I2C-specs-rev-A.pdf
   *
   * @brief API for the MICS-VZ89 Gas Sensor
   *
   * The MiCS-VZ-86/89 combines state-of-the-art MOS sensor technology with
   * intelligent detection algorithms to monitor VOCs and CO2 equivalent
   * variations in confined spaces.
   *
   * The MICSV89 comes in 4 variants, PWM and I2C in 3.3 volts and 5 volts.
   * This library only implements the I2c version of the device.
   *
   * Device output is not valid until a warm up of 15 minutes of operation.
   *
   * @image html micsv89.jpg
   * @snippet micsv89.cxx Interesting
   */
    class MICSV89 {
        public:
            /**
             * MICSV89 constructor
             *
             * @param bus i2c bus the sensor is attached to.
             * @param address. Device address. Default is 0x70.
             */
            MICSV89 (int bus, uint8_t address = 0x70);

            /**
             * MICSV89 destructor
             */
            ~MICSV89 ();

            /**
             * Returns the name of the sensor
             */
            std::string name()
            {
                return m_name;
            }

            /**
             * Returns the  CO2 equivalent value.
             */
            float co2equ();

            /**
             * Returns VOC Short value.
             */
            int vocshort();

            /**
             * Returns Total VOC.
             */
            float tvoc();

            /**
             * Returns resistor value.
             */
            float resistor();

            /**
             * Performs a write/read cycle.
             */
            void update();

            /**
             * Returns true if a valid write/read cycle has been completed.
             * At startup and during write/read cycle will be false.
             */
            bool valid()
            {
                return m_valid;
            }

        private:
            std::string m_name;
            bool m_valid;
            uint8_t m_address;
            uint8_t rx_buf[6];
            uint8_t tx_buf[3];
            mraa::I2c* i2c;
    };
}
