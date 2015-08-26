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

#include <stdint.h>

#include <mraa/gpio.h>

namespace upm {
  /**
   * @brief HT9170 DTMF Decoder library
   * @defgroup ht9170 libupm-ht9170
   * @ingroup seeed gpio other
   */

  /**
   * @library ht9170
   * @sensor ht9170
   * @comname HT9170 DTMF Decoder
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


