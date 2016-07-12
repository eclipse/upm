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

// Unlikey to be changable without external circuitry (voltage divider)
#define HDXXVXTA_DEFAULT_AREF 5.0

namespace upm {
    /**
     * @brief Veris HDXXVXTA Humidity Transmitter
     * @defgroup hdxxvxta libupm-hdxxvxta
     * @ingroup veris ainput temp
     */

    /**
     * @library hdxxvxta
     * @sensor hdxxvxta
     * @comname Veris HDXXVXTA Humidity Transmitter family
     * @type temp
     * @man veris
     * @con ainput
     * @web http://www.veris.com/Item/HD2NVSTA1.aspx
     *
     * @brief API for the Veris HDXXVXTA Humidity Transmitter
     *
     * The driver was developed using the HD2NVSTA1 humidity
     * transmitter.  The 'T' variant supports a temperature
     * transmitter as well.  Both signals are provided by the device
     * as analog 0-5Vdc or 0-10Vdc outputs.  The A1 variant supports a
     * temperature range of -40C-50C, while the A2 variant supports a
     * range of 0C-50C.  Humidity ranges for all devices in this
     * device family range from 0% to 100% (non-condensing).
     *
     * This driver used the 5Vdc outputs for obvious reasons.  Your
     * MCU must be configured for 5V operation.  Using any other
     * analog reference voltage will require the appropriate external
     * circuitry (such as a voltage divider) in order to interface
     * safely with your MCU.
     *
     * For devices which do not support temperature, use '-1' as the
     * temperature pin number in the object constructor.  If
     * temperature measurement is disabled, getTemperature() will always
     * return 0C/32F.
     *
     * @snippet hdxxvxta.cxx Interesting
     */

  class HDXXVXTA {
  public:

    typedef enum {
      // *A1 series (-40C-50C)
      RANGE_MINUS40_50                      = 1,
      // *A2 series (0C-50C)
      RANGE_0_50                            = 2
    } RANGE_T;

    /**
     * HDXXVXTA object constructor
     *
     * @param hPin Analog pin to use for the humidity measurement
     * @param tPin Analog pin to use for temperature.  If your device
     * does not support temperature, use -1 as the value so that
     * temperature will not be queried and an analog pin won't be
     * wasted.
     * @param aref The analog reference voltage, default 5.0
     */
    HDXXVXTA(int hPin, int tPin, float aref=HDXXVXTA_DEFAULT_AREF);

    /**
     * HDXXVXTA object destructor
     */
    ~HDXXVXTA();

    /**
     * Set the temperature range of the sensor.  HD*A1 sensors support
     * a range of -40C-50C, while HD*A2 devices support a temperature
     * range of 0C-50C.  The constructor sets a default of
     * RANGE_MINUS40_50.
     *
     * @param One of the RANGE_T values, default is RANGE_MINUS40_50
     */
    void setRange(RANGE_T range=RANGE_MINUS40_50)
    {
      m_range = range;
    };

    /**
     * Read current values from the sensor and update internal stored
     * values.  This method must be called prior to querying any
     * values, such as temperature or humidity.
     */
    void update();

    /**
     * Get the current temperature.  update() must have been called
     * prior to calling this method.  If temperature measurement was
     * disabled (by passing -1 as the temperature pin in the
     * constructor) then this function will always return 0C/32F.
     *
     * @param fahrenheit true to return the temperature in degrees
     * fahrenheit, false to return the temperature in degrees celsius.
     * The default is false (degrees Celsius).
     * @return The last temperature reading in Celsius or Fahrenheit
     */
    float getTemperature(bool fahrenheit=false);

    /**
     * Get the current relative humidity.  update() must have been called
     * prior to calling this method.
     *
     * @return The last humidity reading
     */
    float getHumidity();


  protected:
    // temperature is an optional feature of the humidity transmitter
    mraa::Aio *m_aioTemp;

    mraa::Aio m_aioHum;

  private:
    float m_aref;
    int m_aResTemp;
    int m_aResHum;

    // does this sensor support temperature reporting?
    bool m_hasTemp;

    // in Celsius
    float m_temperature;

    float m_humidity;

    RANGE_T m_range;
  };
}


