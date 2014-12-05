/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Adapted from Seeed Studio library:
 * https://github.com/Seeed-Studio/RTC_DS1307
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
#include <mraa/i2c.h>

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
   * @brief C++ API for the DS1307 Real Time CLock
   *
   * UPM module for the DS1307 based RTC.  This device can also output 
   * a square wave at 1Khz, 4Khz, 8Khz, and 32Khz.  This capability is
   * not implemented in this module.
   *
   * @ingroup i2c ds1307 RTC
   * @snippet ds1307.cxx Interesting
   */
  class DS1307 {
  public:
    /**
     * ds1307 Real Time Clock constructor
     *
     * @param bus i2c bus to use
     */
    DS1307(int bus);

    /**
     * DS1307 Destructor
     */
    ~DS1307();

    /**
     * Load all of the time values
     *
     * @return True if time data loaded successfully
     */
    bool loadTime();

    /**
     * Set the time.  You should call loadTime() beforehand to
     * maintain consistancy
     *
     * @return True if time saved successfully
     */
    bool setTime();

    /**
     * enable the oscillator on the clock.  
     *
     * @return true if successful
     */
    mraa_result_t enableClock();

    /**
     * disable the oscillator on the clock.  This will prevent the clock
     * from updating any time/date values
     *
     * @return true if successful
     */
    mraa_result_t disableClock();

    /**
     * Write value(s) into registers
     *
     * @param reg register location to start writing into
     * @param buffer buffer for data storage
     * @param len number of bytes to write
     * @return mraa_result_t
     */
    mraa_result_t writeBytes(uint8_t reg, uint8_t *buffer, unsigned int len);

    /**
     * Read value(s) from registers
     *
     * @param reg register location to start reading from
     * @param buffer buffer for data storage
     * @param len number of bytes to read
     * @return number of bytes read
     */
    uint8_t readBytes(uint8_t reg, uint8_t *buffer, unsigned int len);

    /**
     * convert a BCD value into decimal
     *
     * @param val BCD value to convert
     */
    unsigned int bcdToDec(uint8_t val);

    /**
     * convert a decimal value into BCD
     *
     * @param val decimal value to convert
     */
    uint8_t decToBcd(unsigned int val);

    /**
     * These variables store the time data loaded with loadTime(), and
     * will be the source of data when setTime() is called.  It is a
     * good idea call loadTime() to setup the current values before
     * calling setTime() to ensure RTC data is consistent
     */
    unsigned int seconds;
    unsigned int minutes;
    unsigned int hours;
    unsigned int dayOfWeek;
    unsigned int dayOfMonth;
    unsigned int month;
    unsigned int year;
    // True if in AM/PM mode, false if 24h format.
    bool amPmMode;
    // If in AmPmMode, then this is true if it's PM, clear if AM
    bool pm;

  private:
    mraa_i2c_context m_i2c;
  };
}


