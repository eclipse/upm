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

#include <iostream>

#include <mraa/gpio.h>

namespace upm {
    /**
     * @brief ISD1820 Voice Recorder Module library
     * @defgroup isd1820 libupm-isd1820
     * @ingroup seeed gpio sound
     */

    /**
     * @library isd1820
     * @sensor isd1820
     * @comname Grove Recorder
     * @altname ISD1820 Recorder
     * @type sound
     * @man seeed
     * @con gpio
     *
     * @brief API support for the ISD1820-based Grove Voice Recorder
     *
     * This class implements support for the Grove Voice Recorder. There
     * are two digital pins: one that enables recording, and the other
     * that plays back what was previously recorded.
     *
     * @image html isd1820.jpg
     * @snippet isd1820.cxx Interesting
     */
  class ISD1820 {
  public:

    /**
     * ISD1820 constructor
     *
     * @param recPin Pin to use for recording
     * @param playPin Pin to use for playback
     */
    ISD1820(int playPin, int recPin);

    /**
     * ISD1820 destructor
     */
    ~ISD1820();

    /**
     * Starts or stops playback
     *
     * @param enable Starts playback if true, stops if false
     */
    void play(bool enable);

    /**
     * Starts or stops recording
     *
     * @param enable Starts recording if true, stops if false
     */
    void record(bool enable);

  private:
    mraa_gpio_context m_gpioRec;
    mraa_gpio_context m_gpioPlay;
  };
}


