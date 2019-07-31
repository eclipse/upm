/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <iostream>

#include <mraa/gpio.h>

namespace upm {
    /**
     * @brief ISD1820 Voice Recorder Module
     * @defgroup isd1820 libupm-isd1820
     * @ingroup seeed gpio sound
     */

    /**
     * @library isd1820
     * @sensor isd1820
     * @comname Voice Recording and Playback Module
     * @altname ISD1820 Recorder
     * @type sound
     * @man seeed
     * @con gpio
     * @web http://wiki.seeed.cc/Grove_Recorder/
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


