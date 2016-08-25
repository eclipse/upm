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
#include <vector>

#include <mraa/aio.hpp>

#define TEX00_DEFAULT_AREF 5.0

namespace upm {
    /**
     * @brief Veris TEX00 Temperature Sensor
     * @defgroup tex00 libupm-tex00
     * @ingroup veris ainput temp 
     */

    /**
     * @library tex00
     * @sensor tex00
     * @comname Veris TEX00 Temperature Sensor
     * @type temp
     * @man veris
     * @con ainput
     * @web http://www.veris.com/Item/TED00.aspx
     *
     * @brief API for the Veris TEX00 Temperature Sensor
     *
     * The Veris TEX00 temperature sensor family is made up of a
     * series of RTD thermistors in wall mount packaging.  
     * 
     * This driver was developed using the TED00, which utilizes a 10K
     * Ohm Type 2 thermistor.  However, this driver can support the
     * other 12 variants of the TE series as well by providing the
     * correct sensor type to the class constructor.  These other
     * sensor types have not been tested.  Only the TED00 was tested
     * with this driver.
     *
     * This sensor must be connected as part of a voltage divider,
     * with the balancing resistor ideally matched to the sensor's 25C
     * detection range.  For the TED00 (10kt2), a 10K Ohm (1%
     * tolerance) resistor was used in a circuit like the following:
     *
     * GND o----|TED00(10k2)|----o----|balanceResistor(10K)|----o VCC (+5vdc)
     *                           |
     *                           |
     *                           |----o A0 (analog input to MCU)
     *
     * A 3.3vdc voltage can be used as well if desired.
     *
     * @snippet tex00.cxx Interesting
     */

  class TEX00 {
  public:

    typedef enum {
      STYPE_THERMISTOR_TED               = 0, // 10k type 2
      STYPE_THERMISTOR_TEB,                   // 100 Ohm
      STYPE_THERMISTOR_TEC,                   // 1000 Ohm
      STYPE_THERMISTOR_TEI,                   // 1000 Ohm
      STYPE_THERMISTOR_TEE,                   // 2.2k
      STYPE_THERMISTOR_TEF,                   // 3k
      STYPE_THERMISTOR_TEH,                   // 10k type 3
      STYPE_THERMISTOR_TEJ,                   // 10k Dale
      STYPE_THERMISTOR_TES,                   // 10k 3A221
      STYPE_THERMISTOR_TER,                   // 10k "G" US
      STYPE_THERMISTOR_TEM,                   // 20k NTC
      STYPE_THERMISTOR_TEU,                   // 20k "D"
      STYPE_THERMISTOR_TET                    // 100k
    } SENSOR_TYPES_T;

    /**
     * TEX00 object constructor
     *
     * @param tPin Analog pin to use for temperature.
     * @param balanceResistor Resistance (in Ohms) of the balance
     * resistor used in your voltage divider.
     * @param stype The sensor type.  One of the SENSOR_TYPES_T values.
     * @param aref The analog reference voltage, default 5.0
     */
    TEX00(int tPin, float balanceResistor, SENSOR_TYPES_T stype,
          float aref=TEX00_DEFAULT_AREF);

    /**
     * TEX00 object destructor
     */
    ~TEX00();

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
     * Return the smallest temperature that can be measured by the
     * current sensor.
     *
     * @return The smallest detectable temperature this sensor can
     * measure, in C.
     */
    float getTemperatureRangeMin();

    /**
     * Return the largest temperature that can be measured by the
     * current sensor.  
     *
     * @return The largest detectable temperature this sensor can
     * measure, in C.
     */
    float getTemperatureRangeMax();

    /**
     * Detect whether the last measurement exceeded the sensors
     * detection range.  update() must have been called prior to
     * calling this method.
     *
     * @return true if the last measurement was out of range, false
     * otherwise.
     */
    bool isOutOfRange()
    {
      return m_outOfRange;
    }

  protected:
    mraa::Aio m_aioTemp;

    // compute the temperature based on the resistance of the thermistor
    float thermistor(float ohms);

  private:
    float m_aref;
    int m_aResTemp;

    // in Celsius
    float m_temperature;
    // temp reading out of range
    bool m_outOfRange;

    // resistance of the other half of our voltage divider
    float m_balanceResistor;

    int average(int samples);

    // Negative Temperature Coefficient (NTC) or Positive Temperature
    // Coefficient (PTC)
    bool m_isNTC;

    // This may generate a SWIG warning.  It can be safely ignored
    // since this structure is never exposed outside the class.
    struct tempEntry
    {
      tempEntry(float o, float t) : ohms(o), temp(t) {};
      
      float ohms;
      float temp; // in C
    };

    // ohms/temperature table store
    std::vector<tempEntry> m_tempVector;

    // table temperature init functions
    void initThermistorTED();
    void initThermistorTEB();
    void initThermistorTEC();
    void initThermistorTEI();
    void initThermistorTEE();
    void initThermistorTEF();
    void initThermistorTEH();
    void initThermistorTEJ();
    void initThermistorTES();
    void initThermistorTER();
    void initThermistorTEM();
    void initThermistorTEU();
    void initThermistorTET();
  };
}


