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
#include <stdint.h>
#include <sys/time.h>
#include <mraa/gpio.hpp>

namespace upm {

  /**
   * @brief DFRobot Wheel Encoder library
   * @defgroup wheelencoder libupm-wheelencoder
   * @ingroup dfrobot gpio other
   */

  /**
   * @library wheelencoder
   * @sensor wheelencoder
   * @comname DFRobot Wheel Encoder
   * @type other
   * @man dfrobot
   * @web http://www.dfrobot.com/index.php?route=product/product&product_id=98
   * @con gpio

   * @brief API for the DFRobot Wheel Encoder
   *
   * This sensor was developed for the DFRobot Wheel Encoder, though
   * it could be used for any counting time-based task.
   * 
   * When you instantiate a class of this type, the gpio pin specified
   * is connected to an interrupt.  Whenever a low to high transition
   * occurs on the gpio pin, the internal counter is incremented by
   * one.
   *
   * This class also includes a millisecond counter, so that you can
   * correlate the number of counts to a time period for calculating
   * an RPM or other value as needed.
   *
   * @snippet wheelencoder.cxx Interesting
   */
  class WheelEncoder {
  public:

    /**
     * DFRobot Wheel Encoder sensor constructor
     *
     * @param pin Digital pin to use
     */
    WheelEncoder(int pin);

    /**
     * WheelEncoder destructor
     */
    ~WheelEncoder();

    /**
     * Returns the number of milliseconds elapsed since initClock()
     * was last called.
     *
     * @return Elapsed milliseconds
     */
    uint32_t getMillis();

    /**
     * Resets the clock
     *
     */
    void initClock();

    /**
     * Resets the counter to 0. The counter should be
     * stopped via stopCounter() prior to calling this function.
     *
     */
    void clearCounter() { m_counter = 0; };

    /**
     * Starts the counter.  This function will also clear the current
     * count and reset the clock.
     *
     */
    void startCounter();

    /**
     * Stops the counter
     *
     */
    void stopCounter();

    /**
     * Gets the current counter value
     *
     * @return counter value
     */
    uint32_t counter() { return m_counter; };

  protected:
    mraa::Gpio m_gpio;
    static void wheelISR(void *ctx);

  private:
    volatile uint32_t m_counter;
    struct timeval m_startTime;
    bool m_isrInstalled;
  };
}


