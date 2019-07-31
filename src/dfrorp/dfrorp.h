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

#include <stdint.h>
#include "upm.h"
#include "mraa/aio.h"
#include "mraa/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @file dfrorp.h
   * @library dfrorp
   * @brief C API for the DFRobot ORP (Oxidation/Reduction Potential) Sensor
   *
   * @include dfrorp.c
   */

  /**
   * Device context
   */
  typedef struct _dfrorp_context {
    mraa_aio_context    aio;

    // analog ADC resolution
    float               a_res;

    // analog reference voltage
    float               a_ref;

    // for external offset and scaling of the results
    float               offset;
    float               scale;

    // For sensor interface board calibration
    float               orp_cal_offset;

    // our measurements

    // ORP measurement (mV)
    float               orp;

    // volts
    float               volts;

    // normalized ADC
    float               normalized;
  } *dfrorp_context;

  /**
   * DFRORP Initializer
   *
   * @param apin Analog pin to use.
   * @param a_ref The analog reference voltage in use
   */
  dfrorp_context dfrorp_init(unsigned int apin, float a_ref);

  /**
   * DFRORP sensor close function
   */
  void dfrorp_close(dfrorp_context dev);

  /**
   * Read the sensor status and update internal state.  dfrorp_update()
   * must have been called before calling dfrorp_get_orp(),
   * dfrorp_get_normalized(), or dfrorp_get_volts().
   *
   * @param dev sensor context
   * @return UPM result
   */
  upm_result_t dfrorp_update(const dfrorp_context dev);

  /**
   * Set sensor offset.  This offset is applied to the ORP value
   * before scaling.  Default is 0.0.
   *
   * @param dev sensor context pointer
   * @param offset Offset to apply to the computed ORP value
   */
  void dfrorp_set_offset(const dfrorp_context dev, float offset);

  /**
   * Set sensor scale.  The ORP return value is scaled by this value
   * before the offset is applied.  Default is 1.0.
   *
   * @param dev sensor context pointer
   * @param scale The scale to apply to the computed ORP value
   */
  void dfrorp_set_scale(const dfrorp_context dev, float scale);

  /**
   * Get computed ORP (in millivolts) value from the
   * sensor. dfrorp_update() must have been called prior to calling
   * this function.
   *
   * @param dev sensor context pointer
   * @return ORP value in millivolts
   */
  float dfrorp_get_orp(const dfrorp_context dev);

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
   * @param dev sensor context pointer
   * @param offset The ORP offset obtained during calibration.
   */
  void dfrorp_set_calibration_offset(const dfrorp_context dev, float offset);

  /**
   * Get the raw measured volts from the sensor. dfrorp_update() must
   * have been called prior to calling this function.
   *
   * @param dev sensor context pointer
   * @return voltage read from the sensor
   */
  float dfrorp_get_volts(const dfrorp_context dev);

  /**
   * Get the raw normalized ADC values from the sensor.
   * dfrorp_update() must have been called prior to calling this
   * function.
   *
   * @param dev sensor context pointer
   * @return normalized ADC value read from the sensor
   */
  float dfrorp_get_normalized(const dfrorp_context dev);

#ifdef __cplusplus
}
#endif
