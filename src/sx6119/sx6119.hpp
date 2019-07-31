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
     * @brief SX6119-based FM Receiver
     * @defgroup sx6119 libupm-sx6119
     * @ingroup seeed gpio sound
     */

    /**
     * @sensor sx6119
     * @library sx6119
     * @comname FM Receiver
     * @altname SX6119 FM Receiver
     * @type sound
     * @man seeed
     * @web http://wiki.seeed.cc/Grove-FM_Receiver/
     * @con gpio
     *
     * @brief API support for the SX6119-based Grove FM Receiver
     *
     * This class implements support for the SX6119 FM Receiver. There are
     * two digital pins: one that toggles power on/off, and the other that
     * does a seek to the next station.
     *
     * @image html sx6119.jpg
     * @snippet sx6119.cxx Interesting
     */
  class SX6119 {
  public:

    /**
     * SX6119 constructor
     *
     * @param powerPin Pin to use for recording
     * @param seekPin Pin to use for seeking to the next station
     */
    SX6119(int powerPin, int seekPin);

    /**
     * SX6119 destructor
     */
    ~SX6119();

    /**
     * Toggles the device power on or off
     *
     */
    void togglePower();

    /**
     * Seeks to the next station
     *
     */
    void seek();

  private:
    mraa_gpio_context m_gpioPower;
    mraa_gpio_context m_gpioSeek;
  };
}


