/*
 * Author: Jon Trulson <jtrulson@ics.com>
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

#include <string>
#include <mraa/i2c.h>

#define NUNCHUCK_I2C_ADDR    0x52

namespace upm {

  /**
   * @brief Wii Nunchuck library
   * @defgroup nunchuck libupm-nunchuck
   * @ingroup generic i2c accelerometer
   */
  /**
   * @library nunchuck
   * @sensor nunchuck
   * @comname Wii Nunchuck
   * @type accelerometer
   * @man generic
   * @web http://wiibrew.org/wiki/Wiimote/Extension_Controllers
   * @con i2c
   *
   * @brief C++ API for the Wii Nunchuck
   *
   * UPM module for the Wii Nunchuck.  This module was tested with the
   * Wii Nunchuck connected to I2C via a Grove Wii Nunchuck adapter.
   *
   * See http://wiibrew.org/wiki/Wiimote/Extension_Controllers and
   * http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
   * for more detailed information on the controller and it's protocol.
   *
   * A warning for the Grove nunchuck adapter: It has 2 traces on one
   * side, and 3 traces on the other.  Do not match these up with the
   * nunchuck connector's traces.  The nunchuck connector 'groove'
   * should be on the same side as the grove interface socket on the
   * adapter.
   *
   * @snippet nunchuck.cxx Interesting
   */
  class NUNCHUCK {
  public:
    /**
     * NUNCHUCK constructor
     *
     * @param bus i2c bus to use
     * @param addr i2c address to use
     */
    NUNCHUCK(int bus, uint8_t addr=NUNCHUCK_I2C_ADDR);

    /**
     * NUNCHUCK destructor
     */
    ~NUNCHUCK();

    /**
     * Write value(s) into registers
     *
     * @param reg register location to start writing into
     * @param byte byte to write
     * @return true if successful
     */
    bool writeByte(uint8_t reg, uint8_t byte);

    /**
     * Read value(s) from registers
     *
     * @param reg register location to start reading from
     * @param buffer buffer for data storage
     * @param len number of bytes to read
     * @return number of bytes read
     */
    uint8_t readBytes(uint8_t reg, uint8_t *buffer, unsigned int len);

    /**
     * Initialize the controller.  Here, we disable encryption after
     * delaying for a time to ensure the controller is ready.
     *
     * @return true if initialized successfully
     */
    bool init();

    /**
     * Read and update the current state of the controller.
     *
     */
    void update();

    /**
     * Current analog stick X position
     *
     */
    int stickX;

    /**
     * Current analog stick Y position
     *
     */
    int stickY;

    /**
     * Current accelerometer X value
     *
     */
    int accelX;

    /**
     * Current accelerometer Y value
     *
     */
    int accelY;

    /**
     * Current accelerometer Z value
     *
     */
    int accelZ;

    /**
     * Button C pressed?
     *
     */
    bool buttonC;

    /**
     * Button Z pressed?
     *
     */
    bool buttonZ;

  private:
    mraa_i2c_context m_i2c;
  };
}


