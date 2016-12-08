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

#include <string>
#include <mraa/aio.hpp>
#include "lcm1602.hpp"

namespace upm
{
  /**
   * @library i2clcd
   * @sensor sainsmartks
   * @comname SainSmart LCD Keypad Shield
   * @type display
   * @man sainsmart dfrobot sparkfun
   * @web http://www.sainsmart.com/sainsmart-1602-lcd-keypad-shield-for-arduino-duemilanove-uno-mega2560-mega1280.html
   * @web http://www.dfrobot.com/index.php?route=product/product&product_id=51
   * @web https://www.sparkfun.com/products/13293
   * @con gpio analog
   *
   * @brief API for Sainsmart LCD Keypad Shield
   *
   * The Sainsmart LCD Keypad Shield uses 6 digital outputs and 1 analog input
   * (for the keypad). The outputs are used to drive an attached LCM1602 LCD
   * controller. This driver should also be compatible with the similar LCD
   * keypad shields from DFRobot and Sparkfun.
   *
   * @image html keypadlcd.jpg
   * @snippet sainsmartks.cxx Interesting
   */
  class SAINSMARTKS : public Lcm1602
  {
  public:
    /**
     * SAINSMARTKS constructor
     *
     * As this is a shield, you will not likely have any choice over
     * the pins that are used.  For this reason, we provide defaults
     * for all of them -- of course they can be changed if your device
     * is different.
     *
     * @param rs register select pin
     * @param enable enable pin
     * @param d0 data 0 pin
     * @param d1 data 1 pin
     * @param d2 data 2 pin
     * @param d3 data 3 pin
     * @param keypad analog pin of the keypad
     */
    SAINSMARTKS(int rs=8,  int enable=9,
                int d0=4, int d1=5, int d2=6, int d3=7,
                int keypad=0);
    /**
     * SAINSMARTKS destructor
     */
    ~SAINSMARTKS();

    /**
     * returns the floating point representation of the key that is
     * being pushed.  Each key produces a different value between 0.0
     * and 1.0, and only one key can be read at a time.
     *
     * @return the floating point value representing a key
     */
    float getRawKeyValue();

  private:
    mraa::Aio m_aioKeypad;
  };
}
