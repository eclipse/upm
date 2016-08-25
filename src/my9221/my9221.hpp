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

namespace upm {

  /**
   * @brief MY9221 LED Controller library
   * @defgroup my9221 libupm-my9221
   * @ingroup seeed display gpio eak
   */
  class MY9221 {
  public:

    // 12 LED channels per chip (instance)
    static const int LEDS_PER_INSTANCE = 12;

    /**
     * Instantiates an MY9221 object
     *
     * @param dataPin Data pin
     * @param clockPin Clock pin
     * @param instances Number of daisy-chained my9221s, default 1
     */
    MY9221(uint8_t dataPin, uint8_t clockPin, int instances=1);

    /**
     * MY9221 destructor
     */
    ~MY9221();

    /**
     * Enable or disable auto refresh.  When auto refresh is enabled,
     * update the LED display as soon as the internal state changes.
     * When false, the display(s) will not be updated until the
     * refresh() method is called.
     *
     * @param enable true to enable auto refresh, false otherwise
     */
    void setAutoRefresh(bool enable)
    {
      m_autoRefresh = enable;
    }

    /**
     * Set an LED to a specific on (high intensity) or off (low
     * intensity) value.
     *
     * @param led The LED whose state you wish to change
     * @param on true to turn on the LED, false to turn the LED off
     */
    void setLED(int led, bool on);

    /**
     * Set the greyscale intensity of an LED in the OFF state.  The
     * intensity is a value from 0 (fully off) to 255 (fully on).
     * This will take effect on any future LED set or clear
     * operations.
     *
     * @param intensity a value from 0 (fully off) to 255 (fully on)
     */
    void setLowIntensityValue(int intensity);

    /**
     * Set the greyscale intensity of an LED in the ON state.  The
     * intensity is a value from 0 (fully off) to 255 (fully on).
     * This will take effect on any future LED set or clear
     * operations.
     *
     * @param intensity a value from 0 (fully off) to 255 (fully on)
     */
    void setHighIntensityValue(int intensity);

    /**
     * Set all of the LEDS to the ON (high intensity value) state.
     */
    void setAll();

    /**
     * Set all of the LEDS to the OFF (low intensity value) state.
     */
    void clearAll();

    /**
     * Set the LED states to match the internal stored states.  This
     * is useful when auto refresh (setAutoRefresh()) is false to
     * update the display.
     */
    void refresh();

  protected:
    virtual void lockData();
    virtual void send16bitBlock(uint16_t data);

    bool m_autoRefresh;
    // we're only doing 8-bit greyscale, so the high order bits are
    // always 0
    uint16_t m_lowIntensity;
    uint16_t m_highIntensity;

    unsigned int m_instances;

    // an array of uint16_t's representing our bit states (on/off)
    // intensities.  Only the low 8 bits are used, but in the future
    // 16bit support can work here as well.
    uint16_t *m_bitStates;

    uint16_t m_commandWord;

    mraa::Gpio m_gpioClk;
    mraa::Gpio m_gpioData;

  private:
  };

}
