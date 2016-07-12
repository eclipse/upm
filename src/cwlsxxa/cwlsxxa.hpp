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
#define CWLSXXA_DEFAULT_AREF 5.0

namespace upm {
    /**
     * @brief Veris CWLSXXA CO2 Sensor
     * @defgroup cwlsxxa libupm-cwlsxxa
     * @ingroup veris ainput temp gaseous
     */

    /**
     * @library cwlsxxa
     * @sensor cwlsxxa
     * @comname Veris CWLSXXA CO2 Sensor Family
     * @type temp gaseous
     * @man veris
     * @con ainput
     * @web http://www.veris.com/Item/CWLSHTA.aspx
     *
     * @brief API for the Veris CWLSXXA CO2 Sensor Family
     *
     * The driver was developed using the CWLSHTA CO2 Gas sensor.  The
     * 'T' variant supports a temperature transmitter, and the 'H'
     * variant supports a humidity sensor.  All 3 signals are provided
     * by the device as analog 0-5Vdc, 0-10Vdc, or 4-20ma loop current
     * outputs.  For devices supporting temperature, the valid
     * temperature range is 10C to 50C.  The humidity ranges from 0%
     * to 100% (non-condensing).  The CO2 sensor ranges from 0 to 2000
     * ppm.
     *
     * This driver was developed using the 5Vdc outputs and the 4-20ma
     * outputs.  For voltage outputs, your MCU must be configured for
     * 5V operation.  In addition, you must configure the sensor (via
     * it's configuration switches) to output 0-5VDC only.  Using any
     * other analog reference voltage will require the appropriate
     * external circuitry (such as a voltage divider) in order to
     * interface safely with your MCU.
     *
     * In addition, the sensor can be configured for 4-20ma usage, by
     * specifying the correct receiver resistance (in ohms) in the
     * constructor.  This sensor was tested with a Cooking Hacks
     * (Libelium) 4-channel 4-20ma Arduino interface shield.  For this
     * interface, the receiver resistance was specified as 165.0 ohms.
     *
     * For devices which do not support temperature, use '-1' as the
     * temperature pin number in the object constructor.  If
     * temperature measurement is disabled, getTemperature() will always
     * return 0C/32F.
     *
     * For devices which do not support humidity, use '-1' as the
     * temperature pin number in the object constructor.  If
     * humidity measurement is disabled, getHumidity() will always
     * return 0.
     *
     * @snippet cwlsxxa.cxx Interesting
     */

  class CWLSXXA {
  public:

    /**
     * CWLSXXA object constructor
     *
     * @param gPin Analog pin to use for the CO2 measurement
     * @param hPin Analog pin to use for the humidity measurement.  If
     * your device does not support humidity, use -1 as the value so
     * that humidity will not be queried and an analog pin won't be
     * wasted.
     * @param tPin Analog pin to use for temperature.  If your device
     * does not support temperature, use -1 as the value so that
     * temperature will not be queried and an analog pin won't be
     * wasted.
     * @param rResistor The receiver resistance in ohms, when using a
     * 4-20ma current loop interface.  When specified, this value will
     * be used in computing the current based on the voltage read when
     * scaling the return values.  Default is 0.0, for standard
     * scaling based on voltage output rather than current (4-20ma
     * mode).
     * @param aref The analog reference voltage, default 5.0
     */
    CWLSXXA(int gPin, int hPin, int tPin, float rResistor=0.0,
            float aref=CWLSXXA_DEFAULT_AREF);

    /**
     * CWLSXXA object destructor
     */
    ~CWLSXXA();

    /**
     * Read current values from the sensor and update internal stored
     * values.  This method must be called prior to querying any
     * values, such as CO2, temperature, or humidity.
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
     * prior to calling this method. If humidity measurement was
     * disabled (by passing -1 as the te pin in the
     * constructor) then this function will always return 0.
     *
     * @return The last humidity reading
     */
    float getHumidity();

    /**
     * Get the current CO2 concentration in Parts Per Million (PPM).
     * update() must have been called prior to calling this method.
     *
     * @return The last CO2 reading
     */
    float getCO2();


  protected:
    // temperature and humidity are optional features of this transmitter
    mraa::Aio *m_aioTemp;
    mraa::Aio *m_aioHum;

    // CO2 reporting is always supported
    mraa::Aio m_aioCO2;

  private:
    float m_aref;
    float m_rResistor;
    int m_aResTemp;
    int m_aResHum;
    int m_aResCO2;

    // does this sensor support temperature and/or humidity reporting?
    bool m_hasTemp;
    bool m_hasHum;

    // in Celsius
    float m_temperature;

    float m_humidity;

    // in PPM
    float m_co2;

    int average(mraa::Aio *aio, int samples);
  };
}


