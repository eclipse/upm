/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * These modules were rewritten, based on original work by:
 *
 * (original my9221/groveledbar)
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * (grovecircularled)
 * Author: Jun Kato and Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtrulson@ics.com>
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
#include <mraa/common.hpp>
#include <mraa/gpio.hpp>
#include "my9221.hpp"

namespace upm {

  /**
   * @library my9221
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

  class GroveCircularLED : public MY9221 {
  public:
    /**
     * Instantiates an GroveCircularLED object
     *
     * @param dataPin Data pin
     * @param clockPin Clock pin
     */
    GroveCircularLED(uint8_t dataPin, uint8_t clockPin);

    /**
     * GroveCircularLED destructor
     */
    ~GroveCircularLED();

    /**
     * Sets the spinner (turns off all LEDs but selected one)
     *
     * @param position Selected position for the spinner (0-23)
     */
    void setSpinner(uint8_t position);

    /**
     * Sets the lighting status
     *
     * @param level Selected level for the circular LED (0-23)
     * @param direction Up or down; up is true and default
     */
    void setLevel(uint8_t level, bool direction=true);

  protected:
  private:
  };

}
