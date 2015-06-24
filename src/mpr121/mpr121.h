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
#include <mraa/i2c.h>

#define MPR121_I2C_BUS     0
#define MPR121_DEFAULT_I2C_ADDR    0x5a

namespace upm {
  /**
   * @brief MPR121 touch sensor library
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
   * @brief API for the MPR121 I2C touch sensor
   *
   * UPM module for the MPR121 touch sensor
   *
   * @image html mpr121.jpg
   * @snippet mpr121.cxx Interesting
   */
  class MPR121 {
  public:
    /**
     * mpr121 touch sensor constructor
     *
     * @param bus i2c bus to use
     */
    MPR121(int bus, uint8_t address = MPR121_DEFAULT_I2C_ADDR);

    /**
     * MPR121 Destructor
     */
    ~MPR121();

    /**
     * Setup a default configuration, based on Application Note 3944
     * (AN3944):
     * http://cache.freescale.com/files/sensors/doc/app_note/AN3944.pdf
     *
     * After configuration, the sensor will be left in the Run State.
     *
     * @return True if configuration succeeded
     */
    bool configAN3944();

    /**
     * Read the button states into the m_buttonStates member variable.  Also
     * set the m_overCurrentFault variable if an over current is detected.
     */
    void readButtons();

    /**
     * Write value(s) into registers
     *
     * @param reg register location to start writing into
     * @param buffer buffer for data storage
     * @param len number of bytes to write
     * @return mraa_result_t
     */
    mraa_result_t writeBytes(uint8_t reg, uint8_t *buffer, unsigned int len);

    /**
     * Read value(s) from registers
     *
     * @param reg register location to start reading from
     * @param buffer buffer for data storage
     * @param len number of bytes to read
     */
    void readBytes(uint8_t reg, uint8_t *buffer, unsigned int len);

    /**
     * button states
     */
    uint16_t m_buttonStates;

    /**
     * Over current fault detected
     */
    bool m_overCurrentFault;

  private:
    mraa_i2c_context m_i2c;
    uint8_t m_addr;
  };
}


