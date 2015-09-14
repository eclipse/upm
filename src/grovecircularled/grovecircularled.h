/*
 * Author: Jun Kato and Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * This module is based on the my9221 driver
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
#include <mraa/aio.hpp>
#include <mraa/common.hpp>

#include <mraa/gpio.hpp>

#define MAX_BIT_PER_BLOCK     16
#define CMDMODE               0x0000
#define BIT_HIGH              0x00ff
#define BIT_LOW               0x0000

#define HIGH                  1
#define LOW                   0

namespace upm {

  /**
   * @brief Grove Circular LED library
   * @defgroup grovecircularled libupm-grovecircularled
   * @ingroup seeed display gpio
   */

  /**
   * @library grovecircularled
   * @sensor grovecircularled
   * @comname Grove Circular LED
   * @type display
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_Circular_LED
   * @con gpio
   *
   * @brief API for the Grove Circular LED module
   *
   * This is a circular LED ring based on the MY9221 chip. It is often used
   * with a rotary encoder and has 24 controllable LEDs.
   *
   * @image html grovecircularled.jpg
   * @snippet grovecircularled.cxx Interesting
   */
  class GroveCircularLED {
  public:
    /**
     * Instantiates an MY9221 object
     *
     * @param di Data pin
     * @param dcki Clock pin
     */
    GroveCircularLED (uint8_t di, uint8_t dcki);

    /**
     * Sets the lighting status
     *
     * @param level Selected level for the circular LED (0-24)
     * @param direction Up or down; up is true and default
     */
    mraa::Result setLevel (uint8_t level, bool direction=true);

    /**
     * Sets the spinner (lights up all the other LEDs but one)
     *
     * @param position Selected position for the spinner (0-23)
     */
    mraa::Result setSpinner (uint8_t position);

    /**
     * Sets the lighting status
     *
     * @param status Boolean array (24 elements)
     */
    mraa::Result setStatus (bool status[24]);

    /**
     * Returns the name of the component
     */
    std::string name()
      {
        return m_name;
      }
  private:
    mraa::Result lockData ();
    mraa::Result send16bitBlock (short data);

    std::string m_name;
    mraa::Gpio m_clkPinCtx;
    mraa::Gpio m_dataPinCtx;
  };

}
