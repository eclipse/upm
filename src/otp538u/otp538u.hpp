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
   * @brief OTP538U IR Temperature Sensor library
   * @defgroup otp538u libupm-otp538u
   * @ingroup seeed analog light hak
   */
  /**
   * @library otp538u
   * @sensor otp538u
   * @comname OTP538U IR Temperature Sensor
   * @altname Grove IR Temperature Sensor
   * @type light
   * @man generic
   * @con analog
   * @kit hak
   *
   * @brief API for the OTP538U IR Temperature Sensor
   *
   * UPM module for the OTP538U IR temperature sensor
   *
   * This module was tested with the Grove IR non-contact temperature
   * sensor.
   *
   * The sensor provides 2 analog outputs: one for the thermistor
   * that measures the ambient temperature, and the other for the thermopile
   * that measures the object temperature.
   *
   * Much of the code depends on analyzing Seeed Studio* examples
   * and the circuit design. As a result, there are several 'magic'
   * numbers derived from their circuit design. These values are used
   * by default.
   *
   * The tables used came from the "538U VT
   * Table__20_200(v1.3).pdf" and "538RT_table.pdf" datasheets.
   *
   * These tables assume the object to be measured is 9 cm (3.54
   * inches) from the sensor.
   *
   * @image html otp538u.jpg
   * @snippet otp538u.cxx Interesting
   */
  class OTP538U {
  public:
    /**
     * OTP538U constructor
     *
     * @param pinA Analog pin to use for the ambient temperature
     * @param pinO Analog pin to use for the object temperature
     * @param aref Analog reference voltage; default is 5.0 V
     */
    OTP538U(int pinA, int pinO, float aref = 5.0);

    /**
     * OTP538U destructor
     */
    ~OTP538U();

    /**
     * Gets the ambient temperature in Celsius
     *
     * @return Ambient temperature
     */
    float ambientTemperature();

    /**
     * Gets the object temperature in Celsius
     *
     * @return Object temperature
     */
    float objectTemperature();

    /**
     * Sets the offset voltage
     *
     * The Seeed Studio wiki gives an example of calibrating the sensor
     * and calculating the offset voltage to apply. Currently, the
     * default value is set, but you can use the function to set one
     * of your own.
     *
     * @param vOffset Desired offset voltage
     */
    void setVoltageOffset(float vOffset) { m_offsetVoltage = vOffset; };

    /**
     * Sets the output resistance value
     *
     * The Seeed Studio wiki example uses a value of 2,000,000 in one of
     * the equations used to calculate voltage. The value is the
     * resistance of a resistor they use in the output stage of their
     * SIG2 output. This was 'decoded' by looking at the EAGLE* files
     * containing their schematics for this device.
     *
     * @param outResistance Value of the output resistor; default is 2M Ohm
     */
    void setOutputResistence(int outResistance) { 
      m_vResistance = outResistance; };

    /**
     * Sets the reference voltage of the internal Seeed Studio voltage
     * regulator on the sensor board.
     *
     * The Seeed Studio wiki example uses a value of 2.5 in one of the
     * equations used to calculate the resistance of the ambient
     * thermistor. The value is the voltage of an internal voltage
     * regulator used on the sensor board. This was 'decoded' by
     * looking at the EAGLE files containing their schematics for this
     * device.
     *
     * @param vref Reference voltage of the internal sensor; default is 2.5 V
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


