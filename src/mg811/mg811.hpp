/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <iostream>
#include <string>
#include <mraa/aio.hpp>
#include <mraa/gpio.hpp>

#include <interfaces/iGas.hpp>

namespace upm {
  /**
   * @brief MG811 CO2 Sensor
   * @defgroup mg811 libupm-mg811
   * @ingroup dfrobot analog gaseous
   */

  /**
   * @library mg811
   * @sensor mg811
   * @comname CO2 Sensor
   * @altname MG811
   * @type gaseous
   * @man dfrobot
   * @web http://www.dfrobot.com/index.php?route=product/product&product_id=1023
   * @con analog
   *
   * @brief API for the DFRobot CO2 Sensor
   *
   * This sensor returns an an analog voltage that falls as the
   * concentration of CO2 increases.  It contains a heater that must
   * be allowed to 'warm' up the sensor before measurements are stable
   * (hours to days is the recommendation).  It requires that the MCU
   * be powered from an external power supply (not USB) at 5v, since
   * the heater will consume significant current.
   *
   * The sensor should be allowed to 'heat' up for some time before
   * beginning use, typically a couple of hours minimum.  It also
   * needs fairly precise calibration at 400ppm and 1000ppm to return
   * meaningful results.
   *
   * The sensor also incorporates a potentiometer that can be adjusted
   * to specific threshold.  Once that threshold is reached, an LED
   * on the sensor will light, and the digital pin will be driven
   * high.
   *
   * @image html mg811.jpg
   * @snippet mg811.cxx Interesting
   */

  class MG811: virtual public iGas {
  public:

    /**
     * MG811 constructor
     *
     * @param pin Analog pin to use
     * @param dpin Digital pin that indicates threshold
     * @param aref Analog reference voltage; default is 5.0 V
     */
    MG811(int pin, int dpin, float aref=5.0);

    /**
     * MG811 destructor
     */
    virtual ~MG811();

    /**
     * Return a cumputed reference voltage to be used in calibration.
     * @return Computed reference voltage
     */
    float getReferenceVoltage();

    /**
     * Set calibration parameters.  You should measure the reference
     * voltage you get when at CO2 concentrations of 400ppm (ambient)
     * and 1000ppm using the getReferenceVoltage() method.  Then
     * specify those voltages here for more accurate results.
     *
     * @param ppm400 The measured reference voltage at 400 ppm
     * @param ppm1000 The measured reference voltage at 1000 ppm
     */
    void setCalibration(float ppm400, float ppm1000);

    /**
     * Returns the voltage detected on the analog pin
     *
     * @return The detected voltage
     */
    float volts();

    /**
     * Returns the computed CO2 concentration in ppm (Parts Per
     * Million).  This method will return 0.0 if the reference voltage
     * is greater than the ppm400 value.  Essentially, ppm values
     * below 400 will be reported as 0.
     *
     * @return The computed CO2 concentration in ppm
     */
    float ppm();
    float getConcentration();

    /**
     * Read the digital pin and return true if the set threshold has
     * been reached or exceeded.  This threshold is set by adjusting
     * the potentiometer on the sensor.
     *
     * @return true if the threshold has been reached, false otherwise
     */
    bool thresholdReached();

  protected:
    mraa::Aio m_aio;
    mraa::Gpio m_gpio;

    // calibration values
    float m_zeroPointValue;
    float m_reactionValue;

    // ADC resolution
    int m_aRes;

  private:
    float m_aref;
  };
}


