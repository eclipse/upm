/*
 * Author: Sergey Kiselev <sergey.kiselev@intel.com>
 * Copyright (c) 2017 Sergey Kiselev
 * Based on the apa102 driver writen by Yannick Adam <yannick.adam@gmail.com>
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

#include <mraa/gpio.hpp>
#include <vector>

namespace upm
{
/**
 * @brief P9813 Chainable RGB LEDs driver library
 * @defgroup p9813 libupm-p9813
 * @ingroup adafruit gpio led
 */

typedef struct {
    uint8_t red, green, blue;
} RgbColor;

/**
 * @library p9813
 * @sensor p9813
 * @comname Chainable RGB LED driver
 * @type led
 * @man adafruit
 * @con gpio
 * @web https://www.adafruit.com/product/2343
 *
 * @brief API for controlling P9813/Chainable RGB LEDs
 *
 * P9813 LED controller provides individually controllable LEDs through a two pin protocol
 * For each LED, an RGB (0-255) value can be set.
 *
 * @image html p9813.jpg
 * @snippet p9813.cxx Interesting
 */
class P9813
{
  public:
    /**
     * Instantiates a new P9813 LED chain
     *
     * @param ledCount  Number of P9813 leds in the chain
     * @param clkPin	Clock Pin
     * @param dataPin   Data Pin
     * @param batchMode (optional) Immediately write to the LED controllers (false, default) or wait for a pushState
     * call (true)
     */
    P9813(uint16_t ledCount, uint16_t clkPin, uint16_t dataPin, bool batchMode = false);

    /**
     * P9813 destructor
     */
    virtual ~P9813() {};

    /**
     * Set the color for a single LED
     *
     * @param ledIdx        Index of the LED in the chain (0 based)
     * @param r             Red component (0-255)
     * @param g             Green component (0-255)
     * @param b             Blue component (0-255)
     */
    void setLed(uint16_t ledIdx, uint8_t r, uint8_t g, uint8_t b);

    /**
     * Set the color for all LEDs
     *
     * @param r             Red component (0-255)
     * @param g             Green component (0-255)
     * @param b             Blue component (0-255)
     */
    void setAllLeds(uint8_t r, uint8_t g, uint8_t b);

    /**
     * Set the color for a range of LEDs
     *
     * @param startIdx      Start index of the range of LEDs in the chain (0 based)
     * @param endIdx        End index of the range of LEDs in the chain (0 based)
     * @param r             Red component (0-255)
     * @param g             Green component (0-255)
     * @param b             Blue component (0-255)
     */
    void
    setLeds(uint16_t startIdx, uint16_t endIdx, uint8_t r, uint8_t g, uint8_t b);

    /**
     * (Advanced) Manually control the colors and brightness of a range of LEDS
     * Best used to maximize performance
     *
     * @param startIdx      Start index of the range of LEDs to update (0 based)
     * @param endIdx        End index of the range of LEDs to update (0 based)
     * @param colors        Vector containing RgbColor structures for each LED
     *                      No check done on the boundaries
     */
    void setLeds(uint16_t startIdx, uint16_t endIdx, std::vector<RgbColor> colors);

    /**
     * Outputs the current LED data to the LED controllers
     * Note: Only required if batch mode is set to TRUE
     *
     */
    void pushState();

  private:
    std::vector<RgbColor> m_leds;
    mraa::Gpio m_clkGpio;
    mraa::Gpio m_dataGpio;
    bool m_batchMode;

    void sendByte(uint8_t data);
};
}
