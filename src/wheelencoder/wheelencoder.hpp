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

#include <string>
#include <stdint.h>
#include <sys/time.h>
#include <mraa/gpio.hpp>

namespace upm {

  /**
   * @brief Wheel Encoder
   * @defgroup wheelencoder libupm-wheelencoder
   * @ingroup dfrobot gpio other
   */

  /**
   * @library wheelencoder
   * @sensor wheelencoder
   * @comname Wheel Encoder
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
   * @image html wheelencoder.jpg
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


