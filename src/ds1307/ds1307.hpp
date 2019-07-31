/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Adapted from Seeed Studio library:
 * https://github.com/Seeed-Studio/RTC_DS1307
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <mraa/i2c.hpp>
#include <mraa/initio.hpp>

#define DS1307_I2C_BUS     0
#define DS1307_I2C_ADDR    0x68

// Days of the week
#define DS1307_DAY_MON 1
#define DS1307_DAY_TUE 2
#define DS1307_DAY_WED 3
#define DS1307_DAY_THU 4
#define DS1307_DAY_FRI 5
#define DS1307_DAY_SAT 6
#define DS1307_DAY_SUN 7

namespace upm {
  /**
   * @brief DS1307 Real-Time Clock
   * @defgroup ds1307 libupm-ds1307
   * @ingroup seeed i2c time
   */

  /**
   * @library ds1307
   * @sensor ds1307
   * @comname Real Time Clock (RTC) Module
   * @altname DS1307 RTC
   * @type time
   * @man seeed
   * @con i2c
   * @web https://www.maximintegrated.com/en/products/digital/real-time-clocks/DS1307.html
   *
   * @brief API for the DS1307 Real-Time CLock
   *
   * UPM module for the DS1307-based real-time clock. The clock can provide information
   * about seconds, minutes, hours, day of the week, day of the month,
   * month, and year. It can operate in either a 24-hour or a 12-hour format.
   * This device can also output a square wave at 1Khz, 4Khz, 8Khz, and 32Khz.
   * However, this capability is not implemented in this module.
   *
   * @image html ds1307.jpg
   * @snippet ds1307.cxx Interesting
   */
  class DS1307 {
  public:
    /**
     * DS1307 constructor
     *
     * @param bus I2C bus to use
     */
    DS1307(int bus);
    
    /**
     * Instantiates DS1307 Clock based on a given string.
     *
     * @param initStr string containing specific information for DS1307 initialization.
     */
    DS1307(std::string initStr);

    /**
     * Loads all the time values
     *
     * @return True if time data loaded successfully
     */
    bool loadTime();

    /**
     * Sets the time. You should call loadTime() beforehand to
     * maintain consistency
     *
     * @return True if time is set successfully
     */
    bool setTime();

    /**
     * Enables an oscillator on the clock.
     *
     * @return 0 (mraa::SUCCESS) if successful; non-zero otherwise
     */
    mraa::Result enableClock();

    /**
     * Disables the oscillator on the clock. This prevents the clock
     * from updating any time/date values
     *
     * @return 0 (mraa::SUCCESS) if successful; non-zero otherwise
     */
    mraa::Result disableClock();

    /**
     * Writes value(s) into registers
     *
     * @param reg Register location to start writing into
     * @param buffer Buffer for data storage
     * @param len Number of bytes to write
     * @return 0 (mraa::SUCCESS) if successful; non-zero otherwise
     */
    mraa::Result writeBytes(uint8_t reg, uint8_t *buffer, int len);

    /**
     * Reads value(s) from registers
     *
     * @param reg Register location to start reading from
     * @param buffer Buffer for data storage
     * @param len Number of bytes to read
     * @return Number of bytes read
     */
    int readBytes(uint8_t reg, uint8_t *buffer, int len);

    /**
     * Converts a BCD value into decimal
     *
     * @param val BCD value to convert
     * @return Converted decimal value
     */
    unsigned int bcdToDec(uint8_t val);

    /**
     * Converts a decimal value into BCD
     *
     * @param val Decimal value to convert
     * @return Converted BCD value
     */
    uint8_t decToBcd(unsigned int val);

    // These variables store the time data loaded with loadTime(), and
    // are the source of data when setTime() is called.  It is a
    // good idea to call loadTime() to set up the current values before
    // calling setTime() to ensure RTC data is consistent

    /**
     * Holds seconds
     */
    unsigned int seconds;
    /**
     * Holds minutes
     */
    unsigned int minutes;
    /**
     * Holds hours; 1-12 in the am/pm format, 0-23 otherwise
     */
    unsigned int hours;
    /**
     * Holds a day of the week; 1-7, where 1 is Sunday
     */
    unsigned int dayOfWeek;
    /**
     * Holds a day of the month, 1-31
     */
    unsigned int dayOfMonth;
    /**
     * Holds a month, 1-12
     */
    unsigned int month;
    /**
     * Holds a year, 0-99
     */
    unsigned int year;
    /**
     * True if the am/pm format is used, false otherwise
     */
    bool amPmMode;
    /**
     * For the am/pm format, it is true if it's pm, false otherwise
     */
    bool pm;

  private:
    mraa::I2c m_i2c;
    mraa::MraaIo mraaIo;
  };
}
