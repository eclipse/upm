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

#include <iostream>
#include <string>
#include <mraa/aio.hpp>

namespace upm {
  /**
   * @brief DFRobot ADXRS610 Gyro Beakout board
   * @defgroup adxrs610 libupm-adxrs610
   * @ingroup dfrobot analog compass
   */

  /**
   * @library adxrs610
   * @sensor adxrs610
   * @comname DFRobot ADXRS610 Gyro Beakout board
   * @altname ADXRS610
   * @type compass
   * @man dfrobot
   * @web http://www.dfrobot.com/index.php?route=product/product&product_id=642
   * @con analog
   *
   * @brief DFRobot ADXRS610 Gyro Beakout board
   *
   * The ADXRS610 is a MEMS based single axis gyroscope with a range
   * of +/- 300 degrees/sec.  It also incorporates a temperature
   * sensing unit that can be used for advanced calibration.
   *
   * This sensor returns an analog voltage proportional to the
   * rotation about the Z-axis in degrees/sec.  The temperature
   * component returns a proportional analog values in degrees C.
   *
   * This driver was developed using the DFRobot ADXRS610 Gyro Beakout board.
   *
   * @snippet adxrs610.cxx Interesting
   */

  class ADXRS610 {
  public:

    /**
     * ADXRS610 constructor
     *
     * @param dPin Analog pin to use for DATAOUT
     * @param tPin Analog pin to use for temperature measurement
     * @param aref Analog reference voltage; default is 5.0 V
     */
    ADXRS610(int dPin, int tPin, float aref=5.0);

    /**
     * ADXRS610 destructor
     */
    ~ADXRS610();

    /**
     * Returns the voltage detected on the DATA analog pin
     *
     * @return The detected voltage
     */
    float getDataVolts();

    /**
     * Returns the voltage detected on the TEMP analog pin
     *
     * @return The detected voltage
     */
    float getTemperatureVolts();

    /**
     * This method allows you to specify a deadband region around the
     * zero point of the gyro (at rest).  This can be used as a
     * primitive filter to ignore movment around the zero point.
     *
     * @param deadband The voltage around the zero point which will be
     * ignored
     */
    void setDeadband(float deadband) { m_deadband = deadband; };

    /**
     * Set the zero point.  This is the point measured and averaged
     * when the sensor is not moving.  It is set at construction time
     * (averaged over a number of samples), but can be overriden here.
     *
     * @param zeroPoint The averaged zero point of the sensor at rest
     */
    void setZeroPoint(float zeroPoint) { m_zeroPoint = zeroPoint; };

    /**
     * This method samples the data pin samples times to produce an
     * average.  This value can then be used as the zero point
     * (setZeroPoint()).
     *
     * @param samples the number of samples to take an average over.
     * The default is 50.  
     * @return the average of the reading over samples times.
     */
    float calibrateZeroPoint(unsigned int samples=50);

    /**
     * Return the zero point value.
     *
     * @return the current zero point value
     */
    float getZeroPoint() { return m_zeroPoint; };

    /**
     * Return the measured temperature in Celcius.  Note, the
     * datasheet says that this value is very repeatable, but is not
     * an accurate absolute temperature.
     *
     * @return the current temperature in C
     */
    float getTemperature();

    /**
     * Return the measured angular velocity in degrees/sec.
     *
     * @return the current angular velocity in degrees/sec
     */
    float getAngularVelocity();

  protected:
    mraa::Aio m_aioData;
    mraa::Aio m_aioTemp;

  private:
    float m_aref;
    // ADC resolution
    int m_aRes;

    // measured or set zero point value
    float m_zeroPoint;
    // desired deadband, default is 0.0
    float m_deadband;

    // aref / 2
    float m_centerVolts;

    // volts per degree / second (typ)
    static const float m_degreeCoeff = 0.006;

    // volts per degree C (typ)
    static const float m_temperatureCoeff = 0.009;
    // nominal temperature at m_centerVolts
    static const float m_temperatureNom = 25.0;
  };
}


