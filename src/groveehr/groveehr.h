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
   * @brief Grove Ear-clip Heart Rate sensor library
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
   * @brief C++ API for the Grove Ear-clip Heart Rate sensor
   *
   * UPM module for the GroveEHR sensor.  It is used to measure your
   * heartbeat. 
   *
   * @snippet groveehr.cxx Interesting
   */
  class GroveEHR {
  public:
    /**
     * GroveEHR constructor
     *
     * @param pin digital pin to use
     */
    GroveEHR(int pin);
    /**
     * GroveEHR Destructor
     */
    ~GroveEHR();
    /**
     * Return the number of milliseconds elapsed since initClock()
     * was last called.
     *
     * @return elapsed milliseconds
     */
    uint32_t getMillis();

    /**
     * Reset the Clock
     *
     */
    void initClock();

    /**
     * Reset the beat counter to 0.  The beat Counter should be
     * stopped via stopBeatCounter() prior to calling this function.
     *
     */
    void clearBeatCounter();

    /**
     * Start the beat counter
     *
     */
    void startBeatCounter();

    /**
     * Stop the beat counter
     *
     */
    void stopBeatCounter();

    /**
     * Get the beat Counter
     *
     * @return the beat counter
     */
    uint32_t beatCounter();

    /**
     * Beat Interrupt Service Routine
     *
     */
    static void beatISR(void *ctx);

    /**
     * Compute the heart rate
     *
     * @return the computed heart rate
     */
    int heartRate();

  private:
    volatile uint32_t m_beatCounter;
    struct timeval m_startTime;
    mraa_gpio_context m_gpio;
  };
}


