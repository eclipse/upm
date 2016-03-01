/*
 * Author: Yannick Adam <yannick.adam@gmail.com>
 * Copyright (c) 2016 Yannick Adam
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
#include <mraa/spi.hpp>
#include <string>

#define HIGH 1
#define LOW 0

namespace upm
{
/**
 * @brief APA102 RGB LED Strip driver library
 * @defgroup apa102 libupm-apa102
 * @ingroup spi led
 */

/**
 * @library apa102
 * @sensor apa102
 * @comname APA102/DotStar LED Strip
 * @type led
 * @man adafruit
 * @con spi
 *
 * @brief API for controlling APA102/DotStar RGB LED Strips
 *
 * APA102 LED Strips provide individually controllable LEDs through a SPI interface.
 * For each LED, brightness (0-31) and RGB (0-255) values can be set.
 *
 * @image html apa102.jpg
 */
class APA102
{
  public:
    /**
     * Instantiates a new APA102 LED Strip
     *
     * @param ledCount 	Number of APA102 leds in the strip
     * @param spiBus 	SPI Bus number
     * @param batchMode (optional) Immediatly write to SPI (false, default) or wait for a pushState
     * call (true)
     * @param csn 		(optional) Chip Select Pin
     */
    APA102(uint16_t ledCount, uint8_t spiBus, bool batchMode = false, int8_t csn = -1);

    /**
     * APA102 destructor
     */
    ~APA102();

    /**
     * Change the color for a single led
     *
     * @param ledIdx		Index of the LED in the strip (0 based)
     * @param brightness	Brightness value (0-31)
     * @param r				Red component (0-255)
     * @param g				Green component (0-255)
     * @param b				Blue component (0-255)
     */
    void setLed(uint16_t ledIdx, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

    /**
     * Change the color for all leds
     *
     * @param brightness	Brightness value (0-31)
     * @param r				Red component (0-255)
     * @param g				Green component (0-255)
     * @param b				Blue component (0-255)
     */
    void setAllLeds(uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

    /**
     * Change the color for a range of leds
     *
     * @param startIdx		Start index of the range of LEDs in the strip (0 based)
     * @param endIdx		End index of the range of LEDs in the strip (0 based)
     * @param brightness	Brightness value (0-31)
     * @param r				Red component (0-255)
     * @param g				Green component (0-255)
     * @param b				Blue component (0-255)
     */
    void
    setLeds(uint16_t startIdx, uint16_t endIdx, uint8_t brightness, uint8_t r, uint8_t g, uint8_t b);

    /**
     * (Advanced) Manually control the colors of a range of LEDS
     * Best used to maximize performance
     *
     * @param startIdx		Start index of the range of LEDs to update (0 based)
     * @param endIdx		End index of the range of LEDs to update (0 based)
     * @param colors		Pointer to an array of bytes. Each color is described as the following:
     *						B1: Brightness (224-255) B2: Blue (0-255) B3: Green (0-255) B4: Red
     *(0-255)
     *						No check done on the boundaries
     */
    void setLeds(uint16_t startIdx, uint16_t endIdx, uint8_t* colors);

    /**
     * Outputs the current LED data to the SPI bus
     * Note: Only required if batch mode is set to TRUE
     *
     */
    void pushState();

  private:
    mraa::Spi* m_spi;
    mraa::Gpio* m_csnPinCtx;

    uint16_t m_ledCount;
    uint8_t* m_leds;
    uint16_t m_frameLength;

    bool m_batchMode;

    mraa::Result CSOn();
    mraa::Result CSOff();
};
}
