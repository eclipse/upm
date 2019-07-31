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

#include <stdint.h>

#include <mraa/gpio.h>

namespace upm {
  /**
   * @brief HT9170 DTMF Decoder
   * @defgroup ht9170 libupm-ht9170
   * @ingroup seeed gpio other
   */

  /**
   * @library ht9170
   * @sensor ht9170
   * @comname Dual Tone Multi Frequency (DTMF) Decoder
   * @altname HT9170B, HT9170D
   * @type other
   * @man seeed 
   * @web http://www.seeedstudio.com/depot/DTMFDual-Tone-Multi-Frequency-Shield-p-1839.html
   * @con gpio
   *
   * @brief API for the HT9170 DTMF Decoder
   *
   * This driver was developed using the DTMF (Dual-Tone
   * Multi-Frequency) Shield by Seeed Studio*. It can decode DTMF signals presented at
   * its audio input. It does not generate DTMF signals.
   *
   * @image html ht9170.jpg
   * @snippet ht9170.cxx Interesting
   */
  class HT9170 {
  public:

    /**
     * HT9170 constructor
     *
     * @param dr Data ready pin
     * @param o1 Digital pin for data output 1
     * @param o2 Digital pin for data output 2
     * @param o3 Digital pin for data output 3
     * @param o4 Digital pin for data output 4
     */
    HT9170(int dr, int o1, int o2, int o3, int o4);

    /**
     * HT9170 destructor
     */
    ~HT9170();

    /**
     * Checks to see if a DTMF number is ready to be read
     *
     * @return True if there is a digit available to decode
     */
    bool digitReady();

    /**
     * Decodes a digit and returns it
     *
     */
    char decodeDigit();

  private:
    mraa_gpio_context m_dr;
    mraa_gpio_context m_o1;
    mraa_gpio_context m_o2;
    mraa_gpio_context m_o3;
    mraa_gpio_context m_o4;
  };
}


