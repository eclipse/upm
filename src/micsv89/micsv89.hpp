/*
 * Author: Marc Graham <marc@m2ag.net>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <iostream>
#include <string>

#include <interfaces/iGas.hpp>

namespace mraa { class I2c;}

namespace upm {
  /**
   * @brief MICS-VZ89 Environmental Sensor
   * @defgroup micsv89 libupm-micsv89
   * @ingroup generic i2c gaseous
   */
  /**
   * @library micsv89
   * @sensor micsv89
   * @comname Low-power Air Quality Sensor
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
    class MICSV89: virtual public iGas {
        public:
            /**
             * MICSV89 constructor
             *
             * @param bus i2c bus the sensor is attached to.
             * @param address Device address. Default is 0x70.
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
             * Returns the  CO2 equivalent value.
             */
            float getConcentration();

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
            /* Disable implicit copy and assignment operators */
            MICSV89(const MICSV89&) = delete;
            MICSV89 &operator=(const MICSV89&) = delete;

            std::string m_name;
            bool m_valid;
            uint8_t m_address;
            uint8_t rx_buf[6];
            uint8_t tx_buf[3];
            mraa::I2c* i2c;
    };
}
