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
#include <mraa/aio.h>

namespace upm {

  /**
   * @brief otp538u IR Temperature Sensor library
   * @defgroup otp538u libupm-otp538u
   * @ingroup seeed analog light
   */
  /**
   * @library libupm-otp538u
   * @sensor otp538u
   * @comname IR Temperature Sensor
   * @type light
   * @man generic
   * @con analog
   *
   * @brief C++ API for the OTP538U IR Temperature Sensor
   *
   * UPM module for the OTP538U IR Temperature Sensor
   *
   * This module was tested with the Grove IR non-contact temperature
   * sensor.
   *
   * The sensor provides 2 analog outputs - one for the thermistor
   * that measures ambient temperature, and another for the thermopile
   * that measures object temperature.
   *
   * Much of the code depends on analyzing the SeeedStudio examples
   * and circuit design.  As a result, there are several 'magic'
   * numbers that were derived from their circuit design.  By default,
   * these values will be used.
   *
   * The tables used came from the datasheets "538U VT
   * Table__20_200(v1.3).pdf" and "538RT_table.pdf".
   *
   * These tables assume the object to be measured is 9cm (3.54
   * inches) from the sensor.
   *
   * @snippet otp538u.cxx Interesting
   */
  class OTP538U {
  public:
    /**
     * OTP538U sensor constructor
     *
     * @param pinA analog pin to use for Ambient temperature
     * @param pinO analog pin to use for Object temperature
     * @param aref analog reference voltage, default 5.0
     */
    OTP538U(int pinA, int pinO, float aref = 5.0);

    /**
     * OTP538U Destructor
     */
    ~OTP538U();

    /**
     * Get the ambient temperature in C
     *
     * @return the ambient temperature
     */
    float ambientTemperature();

    /**
     * Get the object temperature in C
     *
     * @return the object's temperature
     */
    float objectTemperature();

    /**
     * Set the offset voltage
     *
     * The Seeedstudio wiki gives an example on calibrating the sensor
     * and calculating the offset voltage to apply.  Currently, the
     * default value is set, but you can use the function to set one
     * of your own.
     *
     * @param vOffset the desired offset voltage
     */
    void setVoltageOffset(float vOffset) { m_offsetVoltage = vOffset; };

    /**
     * Set the output resistance value
     *
     * The Seeedstudio wiki example uses a value, 2000000 in one of
     * the equations used to calculate a voltage.  The value is the
     * resistance of a resistor they use in the output stage of their
     * SIG2 output.  This was 'decoded' by looking at the eagle files
     * containing their schematics for this device.
     *
     * @param outResistance value of output resistor, default 2M Ohm.
     */
    void setOutputResistence(int outResistance) { 
      m_vResistance = outResistance; };

    /**
     * Set the voltage reference of the internal seedstudio voltage
     * regulator on the sensor board.
     *
     * The Seeedstudio wiki example uses a value, 2.5 in one of the
     * equations used to calculate the resistance of the ambient
     * thermistor.  The value is the voltage of an internal voltage
     * regulator used in the sensor board.  This was 'decoded' by
     * looking at the eagle files containing their schematics for this
     * device.
     *
     * @param vref internal sensor voltage reference, default 2.5
     */
    void setVRef(float vref) { m_vref = vref; };


  private:
    float m_vref;
    float m_aref;
    int m_vResistance;
    float m_offsetVoltage;
    int m_adcResolution;
    mraa_aio_context m_aioA;
    mraa_aio_context m_aioO;
  };
}


