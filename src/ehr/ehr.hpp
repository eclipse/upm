/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#include <mraa/gpio.h>
#include <mraa/initio.hpp>
#include <interfaces/iHeartRate.hpp>

namespace upm {
  /**
   * @brief Ear-clip Heart Rate Sensor
   * @defgroup ehr libupm-ehr
   * @ingroup seeed gpio medical
   */

  /**
   * @library ehr
   * @sensor ehr
   * @comname Ear-clip Heart Rate Sensor
   * @type medical
   * @man seeed
   * @con gpio
   *
   * @brief API for the  Ear-clip Heart Rate Sensor
   *
   * UPM module for the  ear-clip heart rate sensor. It is used to measure your
   * heart rate.
   *
   * @image html ehr.jpg
   * @snippet ehr.cxx Interesting
   */
  class EHR : virtual public iHeartRate {
  public:
    /**
     * EHR constructor
     *
     * @param pin Digital pin to use
     */
    EHR(int pin);
    /**
     * Instantiates EHR Heart Rate sensor based on a given string.
     *
     * @param initStr string containing specific information for EHR initialization.
     */
    EHR(std::string initStr);

    /**
     * EHR destructor
     */
    ~EHR();
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

    /**
     * Retrieve the computed heart rate
     *
     * @return Computed heart rate
     */
    virtual int getHeartRate();

  private:
    /**
     * Beat interrupt service routine (ISR)
     *
     */
    static void beatISR(void *ctx);

    volatile uint32_t m_beatCounter;
    struct timeval m_startTime;
    mraa_gpio_context m_gpio;
    mraa::MraaIo mraaIo;
  };
}
