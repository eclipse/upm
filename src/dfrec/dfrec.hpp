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

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "dfrec.h"

namespace upm {
  /**
   * @brief DFRobot EC Meter
   * @defgroup dfrec libupm-dfrec
   * @ingroup dfrobot ainput uart liquid
   */

  /**
   * @library dfrec
   * @sensor dfrec
   * @comname DFRobot EC Meter
   * @type liquid
   * @man dfrobot
   * @con ainput uart
   * @web https://www.dfrobot.com/index.php?route=product/product&product_id=1123&search=DFR0300&description=true#.Vx49p-_n-M8
   *
   * @brief DFRobot EC Meter
   *
   * The driver was tested with the DFRobot EC Analog Sensor.
   *
   * This device measure the electrical conductivity of an aqueous
   * solution.  The included probe is a K=1 model.
   *
   * Calibration is somewhat complicated - see the DFRobot wiki for
   * instructions on calibration.  Functions are provided to supply
   * the appropriate values.  By default, the values used in the
   * DFRobot arduino example are used.
   *
   * wiki: https://www.dfrobot.com/wiki/index.php/Analog_EC_Meter_SKU:DFR0300
   *
   * @snippet dfrec.cxx Interesting
   */

  class DFREC {
  public:

    /**
     * DFREC object constructor
     *
     * @param apin Analog pin to use.
     * @param uart_ow The UART that the ds10b20 temperature sensor is
     * connected to.
     * @param device_idx The device index of the ds18b20 sensor to use.
     * If you only have 1 ds18b20 sensor on your DS one wire bus, you
     * would pass 0 here.
     * @param a_ref The analog reference voltage in use
     * @return dfrec context
     */
    DFREC(unsigned int apin, unsigned int uart_ow,
                           unsigned int device_idx,
                           float a_ref);

    /**
     * DFREC object destructor
     */
    ~DFREC();

    /**
     * Read the sensor status and update internal state.
     * dfrec_update() must have been called before calling any of the
     * other get*() functions.
     */
    void update();

    /**
     * Set sensor offset.  This offset is applied to the return EC
     * value before scaling.  Default is 0.0.
     *
     * @param offset The offset to apply.
     */
    void setOffset(float offset);

    /**
     * Set sensor scale.  The return EC value is scaled by this value
     * before the offset is applied.  Default is 1.0.
     *
     * @param scale The scale to apply.
     */
    void setScale(float scale);

    /**
     * Get computed EC (ms/cm) value from the sensor. update() must
     * have been called prior to calling this function.
     *
     * @return EC value in ms/cm.
     */
    float getEC();

    /**
     * Get the measured volts from the sensor. update() must have been
     * called prior to calling this function.
     *
     * @return The voltage measurement.
     */
    float getVolts();

    /**
     * Get the normalized ADC value from the sensor.  update() must have
     * been called prior to calling this function.
     *
     * @return The normalized ADC value.
     */
    float getNormalized();

    /**
     * Get the measured temperature from the temperature sensor.  The
     * return value will be in Celsius.
     *
     * @return temperature read from the sensor in Celsius.
     */
    float getTemperature();

    /**
     * Set the global threshold min and max values.  See the DFRobot
     * wiki for details.
     *
     * @param min Minimum threshold to be valid.  Default 150.
     * @param max Maximum threshold to be valid.  Default 3300.
     */
    void setThresholdMinMax(float min, float max);

    /**
     * Set the threshold 1 values.  See the DFRobot wiki for details.
     *
     * @param thres Threshold maximum for value section 1.  Default 448.
     * @param scale Scale for value section 1.  Default 6.64.
     * @param offset Offset for value section 1.  Default -64.32.
     */
    void setThreshold1(float thres, float scale, float offset);

    /**
     * Set the threshold 2 values.  See the DFRobot wiki for details.
     *
     * @param thres Threshold maximum for value section 2.  Default 1457.
     * @param scale Scale for value section 2.  Default 6.98.
     * @param offset Offset for value section 2.  Default -127.0.
     */
    void setThreshold2(float thres, float scale, float offset);

    /**
     * Set the threshold 3 values.  See the DFRobot wiki for details.
     *
     * @param scale Scale for value section 3.  Default 5.3.
     * @param offset Offset for value section 3.  Default 2278.
     */
    void setThreshold3(float scale, float offset);

  protected:
    // dfrec device context
    dfrec_context m_dfrec;

  private:
  };
}
