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

#include <stdint.h>
#include "upm.h"
#include "mraa/aio.h"

#include "ds18b20.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @file dfrec.h
   * @library dfrec
   * @brief C API for the DFRobot EC Meter
   *
   * @include dfrec.c
   */

  /**
   * Device context
   */
  typedef struct _dfrec_context {
    mraa_aio_context    aio;

    // ds18b20 context (for temperature measurement
    ds18b20_context     ds18b20;
    unsigned int        device_idx;

    // analog ADC resolution
    float               a_res;

    // analog reference voltage
    float               a_ref;

    // for external offset and scaling of the results
    float               offset;
    float               scale;

    // volts
    float               volts;

    // normalized ADC
    float               normalized;

    // the EC value
    float               ec;

    // EC threshold min/max (mV)
    float               thres_min; // 150
    float               thres_max; // 3300

    // the temperature read from the ds18b29
    float               temperature;

    // thresholds, scales, and offsets taken from the DFRobot code
    float               thres_1;   // 448
    float               scale_1;   // 6.84
    float               offset_1;  // -64.32

    float               thres_2;   // 1457
    float               scale_2;   // 6.98
    float               offset_2;  // -127.0

    float               scale_3;   // 5.3
    float               offset_3;  // +2278.0
  } *dfrec_context;

  /**
   * DFREC Initializer
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
  dfrec_context dfrec_init(unsigned int apin, unsigned int uart_ow,
                           unsigned int device_idx,
                           float a_ref);

  /**
   * DFREC sensor close function
   */
  void dfrec_close(dfrec_context dev);

  /**
   * Read the sensor status and update internal state.  dfrec_update()
   * must have been called before calling any of the other
   * dfrec_get*() functions
   *
   * @param dev sensor context
   * @return UPM result
   */
  upm_result_t dfrec_update(const dfrec_context dev);

  /**
   * Set sensor offset.  This offset is applied to the EC value
   * before scaling.  Default is 0.0.
   *
   * @param dev sensor context pointer
   * @param offset Offset to apply to the computed EC value
   */
  void dfrec_set_offset(const dfrec_context dev, float offset);

  /**
   * Set sensor scale.  The EC return value is scaled by this value
   * before the offset is applied.  Default is 1.0.
   *
   * @param dev sensor context pointer
   * @param scale The scale to apply to the computed EC value
   */
  void dfrec_set_scale(const dfrec_context dev, float scale);

  /**
   * Get computed EC (in ms/cm) value from the sensor. dfrec_update()
   * must have been called prior to calling this function.
   *
   * @param dev sensor context pointer
   * @return EC value in millivolts
   */
  float dfrec_get_ec(const dfrec_context dev);

  /**
   * Get the raw measured volts from the sensor. dfrec_update() must
   * have been called prior to calling this function.
   *
   * @param dev sensor context pointer
   * @return Voltage read from the sensor
   */
  float dfrec_get_volts(const dfrec_context dev);

  /**
   * Get the measured temperature from the temperature sensor.  The
   * return value will be in Celsius.
   *
   * @param dev sensor context pointer
   * @return Temperature read from the sensor in Celsius.
   */
  float dfrec_get_temperature(const dfrec_context dev);

  /**
   * Get the raw normalized ADC values from the sensor.
   * dfrec_update() must have been called prior to calling this
   * function.
   *
   * @param dev sensor context pointer
   * @return Normalized ADC value read from the sensor
   */
  float dfrec_get_normalized(const dfrec_context dev);

  /**
   * Set the global threshold min and max values.  See the DFRobot
   * wiki for details.
   *
   * @param dev sensor context pointer
   * @param min Minimum threshold to be valid.  Default 150.
   * @param max Maximum threshold to be valid.  Default 3300.
   */
  void dfrec_set_threshold_min_max(const dfrec_context dev, float min,
                                   float max);

  /**
   * Set the threshold 1 values.  See the DFRobot wiki for details.
   *
   * @param dev sensor context pointer
   * @param thres Threshold maximum for value section 1.  Default 448.
   * @param scale Scale for value section 1.  Default 6.64.
   * @param offset Offset for value section 1.  Default -64.32.
   */
  void dfrec_set_threshold_1(const dfrec_context dev, float thres,
                             float scale, float offset);

  /**
   * Set the threshold 2 values.  See the DFRobot wiki for details.
   *
   * @param dev sensor context pointer
   * @param thres Threshold maximum for value section 2.  Default 1457.
   * @param scale Scale for value section 2.  Default 6.98.
   * @param offset Offset for value section 2.  Default -127.0.
   */
  void dfrec_set_threshold_2(const dfrec_context dev, float thres,
                             float scale, float offset);

  /**
   * Set the threshold 3 values.  See the DFRobot wiki for details.
   *
   * @param dev sensor context pointer
   * @param scale Scale for value section 3.  Default 5.3.
   * @param offset Offset for value section 3.  Default 2278.
   */
  void dfrec_set_threshold_3(const dfrec_context dev, float scale,
                             float offset);


#ifdef __cplusplus
}
#endif
