/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
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
#include <iostream>

#include <mraa/aio.hpp>

// Unlikey to be changable
#define TEAMS_DEFAULT_AREF 5.0

namespace upm {
    /**
     * @brief Veris TEAMS Temperature Transmitter
     * @defgroup teams libupm-teams
     * @ingroup veris ainput temp 
     */

    /**
     * @library teams
     * @sensor teams
     * @comname Veris TEAMS Temperature Transmitter
     * @type temp
     * @man veris
     * @con ainput
     * @web http://www.veris.com/Item/TEAMS.aspx
     *
     * @brief API for the Veris TEAMS Temperature Transmitter
     *
     * The Veris TEAMS temperature sensor provides it's output via a
     * 4-20ma current loop.  The supported temperature range is 10C to
     * 35C.
     *
     * This sensor was developed with a Cooking Hacks (Libelium)
     * 4-channel 4-20ma Arduino interface shield.  For this interface,
     * the receiver resistance (rResistor) was specified as 165.0
     * ohms.
     *
     * When using a 4-20ma current loop interface which scales the
     * sensors' values to a 0-5vdc range, you can supply 0.0 as the
     * rResistor value in the constructor (default), and it will act
     * just like a normal analog input.
     *
     * @snippet teams.cxx Interesting
     */

  class TEAMS {
  public:

    /**
     * TEAMS object constructor
     *
     * @param tPin Analog pin to use for temperature.
     * @param rResistor The receiver resistance in ohms, when using a
     * 4-20ma current loop interface.  When specified, this value will
     * be used in computing the current based on the voltage read when
     * scaling the return values.  Default is 0.0, for standard
     * scaling based on voltage output rather than current (4-20ma
     * mode).
     * @param aref The analog reference voltage, default 5.0
     */
    TEAMS(int tPin, float rResistor=0.0, float aref=TEAMS_DEFAULT_AREF);

    /**
     * TEAMS object destructor
     */
    ~TEAMS();

    /**
     * Read current values from the sensor and update internal stored
     * values.  This method must be called prior to querying any
     * values, such as temperature.
     */
    void update();

    /**
     * Get the current temperature.  update() must have been called
     * prior to calling this method.
     *
     * @param fahrenheit true to return the temperature in degrees
     * fahrenheit, false to return the temperature in degrees celsius.
     * The default is false (degrees Celsius).
     * @return The last temperature reading in Celsius or Fahrenheit
     */
    float getTemperature(bool fahrenheit=false);

    /**
     * When using a direct 4-20ma interface (rResistor supplied in the
     * constructor is >0.0), this function will return false when the
     * computed milliamps falls below 4ma, indicating that the sensor
     * is not connected.  If rResistor was specified as 0.0 in the
     * constructor, this function will always return true.
     *
     * @return true if the sensor is connected, false otherwise.
     */
    bool isConnected()
    {
      return m_connected;
    };

    /**
     * When using a direct 4-20ma interface (rResistor supplied in the
     * constructor is >0.0), this function will return the computed
     * milliamps (after the offset has been applied).  If rResistor was
     * specified as 0.0 in the constructor, this function will always
     * return 0.0.
     *
     * @return The last measured current in milliamps after any offset
     * has been applied.
     */
    float getRawMilliamps()
    {
      return m_rawMilliamps;
    };

    /**
     * Specify an offset in milliamps to be applied to the computed
     * current prior to scaling and conversion.  This can be used to
     * 'adjust' the computed value.  If rResistor was specified as 0.0
     * in the constructor, this function will have no effect.
     *
     * @param offset a positive or negative value that will be applied
     * to the computed current measured.
     */
    void setOffsetMilliamps(float offset)
    {
      m_offset = offset;
    };

  protected:
    mraa::Aio m_aioTemp;

  private:
    float m_aref;
    float m_rResistor;
    int m_aResTemp;

    // for a 4-20 ma loop
    bool m_connected;
    float m_rawMilliamps;

    // in Celsius
    float m_temperature;

    // in case an offset should be applied to the cumputed current
    float m_offset;

    int average(int samples);
  };
}


