/*
 * Author: Jon Trulson <jtrulson@ics.com>
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
#include <mraa/i2c.hpp>

#define MPR121_I2C_BUS     0
#define MPR121_DEFAULT_I2C_ADDR    0x5a

namespace upm {
  /**
   * @brief MPR121 Touch Sensor library
   * @defgroup mpr121 libupm-mpr121
   * @ingroup seeed i2c touch tsk
   */
  /**
   * @library mpr121
   * @sensor mpr121
   * @comname MPR121 Touch Sensor
   * @altname Grove I2C Touch Sensor
   * @type touch
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_I2C_Touch_Sensor
   * @con i2c
   * @kit tsk
   *
   * @brief API for the MPR121 I2C Touch Sensor
   *
   * UPM module for the MPR121 touch sensor
   *
   * @image html mpr121.jpg
   * @snippet mpr121.cxx Interesting
   */
  class MPR121 {
  public:
    /**
     * MPR121 constructor
     *
     * @param bus I2C bus to use
     */
    MPR121(int bus, uint8_t address = MPR121_DEFAULT_I2C_ADDR);

    /**
     * MPR121 destructor
     * ~MPR121();
     * there is no need for the destructor
     **/

    /**
     * Sets up a default configuration, based on Application Note 3944
     * (AN3944):
     * http://cache.freescale.com/files/sensors/doc/app_note/AN3944.pdf
     *
     * After configuration, the sensor is left in the run state.
     *
     * @return True if configuration is successful
     */
    bool configAN3944();

    /**
     * Reads button states in the m_buttonStates member variable. Also,
     * sets the m_overCurrentFault variable if overcurrent is detected.
     */
    void readButtons();

    /**
     * Writes value(s) into registers
     *
     * @param reg Register location to start writing into
     * @param buffer Buffer for data storage
     * @param len Number of bytes to write
     * @return mraa::Result
     */
    mraa::Result writeBytes(uint8_t reg, uint8_t *buffer, int len);

    /**
     * Reads value(s) from registers
     *
     * @param reg Register location to start reading from
     * @param buffer Buffer for data storage
     * @param len Number of bytes to read
     */
    int readBytes(uint8_t reg, uint8_t *buffer, int len);

    /**
     * Button states
     */
    uint16_t m_buttonStates;

    /**
     * Overcurrent detected
     */
    bool m_overCurrentFault;

  private:
    mraa::I2c m_i2c;
    uint8_t m_addr;
  };
}


