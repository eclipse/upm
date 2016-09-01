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

#include "dfrorp.h"

namespace upm {
  /**
   * @brief UPM C++ API for the DFRobot ORP (Oxidation/Reduction
   * Potential) Sensor
   * @defgroup dfrorp libupm-dfrorp
   * @ingroup dfrobot ainput liquid
   */

  /**
   * @library dfrorp
   * @sensor dfrorp
   * @comname DFRobot ORP (Oxidation/Reduction Potential) Sensor
   * @type liquid
   * @man dfrobot
   * @con ainput
   * @web http://www.dfrobot.com/index.php?route=product/product&path=36&product_id=1071#.V8Wywt9ytNJ
   *
   * @brief API for the DFRobot ORP (Oxidation/Reduction Potential) Sensor
   *
   * The driver was tested with the DFRobot ORP (Oxidation/Reduction
   * Potential) Sensor.
   *
   * To calibrate:
   *
   * Disconnect the sensor probe (but leave the sensor interface board
   * connected).  Then run one of the examples while holding down the
   * 'calibrate' button on the device.  Read the ORP value reported
   * (it should be fairly small).
   *
   * This value is what you should supply to
   * setCalibrationOffset().  Then reconnect the probe to the
   * interface board and you should be ready to go.
   *
   * DO NOT press the calibrate button on the interface board while
   * the probe is attached or you can permanently damage the probe.
   *
   * @snippet dfrorp.cxx Interesting
   */

  class DFRORP {
  public:

    /**
     * DFRORP object constructor
     *
     * @param apin Analog pin to use
     * @param a_ref The analog reference voltage in use.  Default 5.0.
     */
    DFRORP(int apin, float a_ref=5.0);

    /**
     * DFRORP object destructor
     */
    ~DFRORP();

    /**
     * Read the sensor status an update internal state.
     * update() must have been called before calling
     * getORP(), getNormalized(), or getVolts().
     */
    void update();

    /**
     * Set sensor offset.  This offset is applied to the return ORP
     * value before scaling.  Default is 0.0.
     *
     * @param offset The offset to apply.
     */
    void setOffset(float offset);

    /**
     * Set sensor scale.  The return ORP value is scaled by this value
     * before the offset is applied.  Default is 1.0.
     *
     * @param scale The scale to apply.
     */
    void setScale(float scale);

    /**
     * Get computed ORP (in millivolts) value from the
     * sensor. update() must have been called prior to calling this
     * function.
     *
     * @return ORP value in millivolts
     */
    float getORP();

    /**
     * Set the calibration offset for the device.  This is
     * determined by disconnecting the sensor probe (but leaving the
     * sensor interface board connected).  Then run one of the examples
     * while holding down the 'calibrate' button on the device.  Read
     * the ORP value reported.
     *
     * This (low) ORP value is what you should supply to this function.
     * Then reconnect the probe to the interface board and you should be
     * ready to go.
     *
     * DO NOT press the calibrate button on the interface board while
     * the probe is attached or you can permanently damage the probe.
     *
     * @param offset The ORP offset obtained during calibration.
     */
    void setCalibrationOffset(float offset);

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

  protected:
    // dfrorp device context
    dfrorp_context m_dfrorp;

  private:
  };
}
