/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015-2017 Intel Corporation.
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
#include "nunchuck.h"


namespace upm {

  /**
   * @brief Wii Nunchuk library
   * @defgroup nunchuck libupm-nunchuck
   * @ingroup generic i2c accelerometer robok
   */
  /**
   * @library nunchuck
   * @sensor nunchuck
   * @comname Wii Nunchuk
   * @type accelerometer
   * @man generic
   * @web http://wiibrew.org/wiki/Wiimote/Extension_Controllers
   * @con i2c
   * @kit robok
   *
   * @brief API for the Wii* Nunchuk controller
   *
   * UPM module for the Wii Nunchuk controller. This module was tested with
   * Wii Nunchuck connected to I2C via a Grove Wii Nunchuck adapter.
   *
   * See http://wiibrew.org/wiki/Wiimote/Extension_Controllers and
   * http://wiibrew.org/wiki/Wiimote/Extension_Controllers/Nunchuck
   * for more details on the controller and its protocol.
   *
   * A warning for the Grove Wii Nunchuk adapter: it has 2 traces on
   * one side, and 3 traces on the other.  Do not match these up with
   * the Nunchuk connector's traces. The connector's 'groove' should
   * be on the same side as the Grove interface socket on the adapter.
   *
   * @image html nunchuck.jpg
   * @snippet nunchuck.cxx Interesting
   */
  class NUNCHUCK {
  public:
    /**
     * NUNCHUCK constructor
     *
     * @param bus I2C bus to use
     */
    NUNCHUCK(int bus);

    /**
     * NUNCHUCK destructor
     */
    virtual ~NUNCHUCK();

    /**
     * Reads and updates the current state of the controller.
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

  protected:
      nunchuck_context m_nunchuck;

  private:
  };
}


