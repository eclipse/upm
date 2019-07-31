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
   * @sensor groveledbar
   * @comname LED Bar
   * @altname MY9221 LED Bar
   * @type display
   * @man seeed
   * @web http://www.seeedstudio.com/wiki/Grove_-_LED_Bar
   * @con gpio
   * @kit eak
   *
   * @brief API for Grove LED Bars base on the MY9221
   *
   * This is a 10-segment LED bar, with 8 green segments, 1 yellow
   * segment, and one red segment.  They can be daisy chained together
   * so that this module can control multiple LED bars.
   *
   * @image html my9221.jpg
   * @snippet my9221-groveledbar.cxx Interesting
   */

  class GroveLEDBar : public MY9221 {
  public:

    /**
     * Instantiates an GroveLEDBar object
     *
     * @param dataPin Data pin
     * @param clockPin Clock pin
     * @param instances Number of daisy-chained Grove LED Bars, default 1
     */
    GroveLEDBar(int dataPin, int clockPin, int instances=1);

    /**
     * GroveLEDBar destructor
     */
    virtual ~GroveLEDBar();

    /**
     * Sets the bar level
     *
     * @param level Selected level for the bar (0 - 10). 0 is off
     * @param greenToRed true if you start the level on the first
     * green LED, false otherwise
     * @param barNumber If you have multiple LED bars chained
     * together, this argument selects a specific bar starting at 0.
     * The default is 0.
     */
    void setBarLevel(uint8_t level, bool greenToRed=true,
                     unsigned int barNumber=0);

  protected:
  private:
  };

}
