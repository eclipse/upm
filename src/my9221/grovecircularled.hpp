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
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
   * @comname Circular LED
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
   * @snippet my9221-grovecircularled.cxx Interesting
   */

  class GroveCircularLED : public MY9221 {
  public:
    /**
     * Instantiates an GroveCircularLED object
     *
     * @param dataPin Data pin
     * @param clockPin Clock pin
     */
    GroveCircularLED(int dataPin, int clockPin);

    /**
     * GroveCircularLED destructor
     */
    virtual ~GroveCircularLED();

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
