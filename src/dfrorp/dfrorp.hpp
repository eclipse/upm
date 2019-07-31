/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "dfrorp.h"
#include <mraa/initio.hpp>
#include <interfaces/iOrp.hpp>

namespace upm {
  /**
   * @brief DFRobot ORP (Oxidation/Reduction Potential) Sensor
   * @defgroup dfrorp libupm-dfrorp
   * @ingroup dfrobot analog liquid
   */

  /**
   * @library dfrorp
   * @sensor dfrorp
   * @comname Analog Oxidation Reduction Potential (ORP) Sensor
   * @type liquid
   * @man dfrobot
   * @con ainput
   * @web http://www.dfrobot.com/index.php?route=product/product&path=36&product_id=1071#.V8Wywt9ytNJ
   *
   * @brief API for the DFRobot ORP (Oxidation/Reduction Potential) Sensor
   *
   * This library was tested with the DFRobot ORP (Oxidation/Reduction
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

  class DFRORP : virtual public iOrp {
  public:

    /**
     * DFRORP object constructor
     *
     * @param apin Analog pin to use
     * @param a_ref The analog reference voltage in use.  Default 5.0.
     */
    DFRORP(int apin, float a_ref=5.0);

    /**
     * Instantiates DFRobot object based on a given string.
     *
     * @param initStr string containing specific information for DFRobot initialization.
     */
    DFRORP(std::string initStr);

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
     * Get computed ORP (in millivolts) value from the
     * sensor.
     *
     * @return ORP value in millivolts
     */
    virtual float getORPValue();

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
    mraa::MraaIo mraaIo;

  private:
    /* Disable implicit copy and assignment operators */
    DFRORP(const DFRORP&) = delete;
    DFRORP &operator=(const DFRORP&) = delete;
  };
}
