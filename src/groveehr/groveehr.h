/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <mraa/gpio.h>

namespace upm {
  /**
   * @brief Grove Ear-clip Heart Rate Sensor library
   * @defgroup groveehr libupm-groveehr
   * @ingroup seeed gpio medical
   */

  /**
   * @library groveehr
   * @sensor groveehr
   * @comname Grove Ear-clip Heart Rate Sensor
   * @type medical
   * @man seeed
   * @con gpio
   *
   * @brief API for the Grove Ear-clip Heart Rate Sensor
   *
   * UPM module for the Grove ear-clip heart rate sensor. It is used to measure your
   * heart rate. 
   *
   * @image html groveehr.jpg 
   * @snippet groveehr.cxx Interesting
   */
  class GroveEHR {
  public:
    /**
     * GroveEHR constructor
     *
     * @param pin Digital pin to use
     */
    GroveEHR(int pin);
    /**
     * GroveEHR destructor
     */
    ~GroveEHR();
    /**
     * Returns the time of milliseconds elapsed since initClock()
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
     * Resets the beat counter to 0. The beat counter should be
     * stopped via stopBeatCounter() prior to calling this function.
     *
     */
    void clearBeatCounter();

    /**
     * Starts the beat counter
     *
     */
    void startBeatCounter();

    /**
     * Stops the beat counter
     *
     */
    void stopBeatCounter();

    /**
     * Gets the beat Counter
     *
     * @return Beat counter
     */
    uint32_t beatCounter();

    /**
     * Computes the heart rate
     *
     * @return Computed heart rate
     */
    int heartRate();

  private:
    /**
     * Beat interrupt service routine (ISR)
     *
     */
    static void beatISR(void *ctx);
    
    volatile uint32_t m_beatCounter;
    struct timeval m_startTime;
    mraa_gpio_context m_gpio;
  };
}


