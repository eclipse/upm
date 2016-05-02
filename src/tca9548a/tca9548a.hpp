/*
 * Author: Keelan Lightfoot <keelanlightfoot@gmail.com>
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

#define TCA9548A_I2C_BUS 0
#define TCA9548A_DEFAULT_ADDR 0x70

#include <iostream>
#include <string>
#include "mraa.hpp"
#include "mraa/i2c.hpp"

#define TCA9548A_NO_PORTS 0x00
#define TCA9548A_ALL_PORTS 0xFF

#define TCA9548A_MIN_PORT 0
#define TCA9548A_MAX_PORT 7




namespace upm {
  /**
   * @brief TCA9548A i2c Switch Library
   * @defgroup tca9548a libupm-tca9548a
   * @ingroup i2c
   */
  /**
   * @library tca9548a
   * @sensor tca9548a
   * @comname TCA9548A i2c Switch
   * @type other
   * @man ti
   * @con i2c
   * @web http://www.ti.com/product/TCA9548A
   *
   * @brief API for TCA9548A i2c Multiplexer
   *
   * The TCA9548A device has eight bidirectional translating switches that can be
   * controlled through the I2C bus. The SCL/SDA upstream pair fans out to eight
   * downstream pairs, or channels. Any individual SCn/SDn channel or combination
   * of channels can be selected, determined by the contents of the programmable
   * control register.
   *
   * Tested with the Adafriut TCA9548A board.
   *
   * @image html tca9548a.jpg
   * @snippet tca9548a.cxx Interesting
   */
    class TCA9548A {

        public:
            /**
            * @enum TCA9548A_PORT_STATE
            * @brief boolean enum containing port state
            *
            * @var TCA9548A_PORT_STATE::DISABLED = 0
            * @var TCA9548A_PORT_STATE::ENABLED = 1
            */
           typedef enum {
                DISABLED = 0,
                ENABLED = 1
           } TCA9548A_PORT_STATE;

           /**
           * @enum TCA9548A_PORT_MODE
           * @brief boolean enum containing port access mode
           *
           * @var TCA9548A_PORT_MODE::EXCLUSIVE = 0
           * @var TCA9548A_PORT_MODE::INCLUSIVE = 1
           */
          typedef enum {
               EXCLUSIVE = 0,
               INCLUSIVE = 1
          } TCA9548A_PORT_MODE;

            /**
             * TCA9548A constructor
             *
             * @param bus i2c bus the sensor is attached to.
             * @param address Device address. Default is 0x18.
             */
            TCA9548A(int bus, uint8_t address);

            /**
             * TCA9548A destructor
             */
            ~TCA9548A();

            /**
             * Returns the name of the switch
             */
            std::string name() {
                return m_name;
            }

            /**
            * Returns the status of a port as configured in the multiplexer.
            *
            *  @param port Switch port to check
            */
            bool getPort(int port);

            /**
            *  Configure an individual port on the multiplexer
            *
            *  @param port Port to configure
            *  @param enabled Set to true to enable the port, false to disable the port.
            *  @param exclusive Set to true to disable all other ports, false to leave
            *  existing port config untouched. When exclusive is set to false, an
            *  additional i2c read is required to read the current port setting.
            */
            void setPort(int port, TCA9548A_PORT_STATE state, TCA9548A_PORT_MODE mode);

            /**
            *  Disables all ports on the multiplexer.
            */
            void disableAllPorts();

            /**
            *  Enables all ports on the multiplexer. Useful when the multiplexer is used
            *  to electrically extend the bus rather than resolve address conflicts.
            */
            void enableAllPorts();

        private:
            std::string m_name;

            mraa::I2c* i2c;

            uint8_t getPortConfig();
            void setPortConfig(uint8_t config);
            bool validPort(int port);
    };
}
