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
#include "mraa/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @file mma7361.h
   * @library mma7361
   * @brief C API for the MMA7361 Analog Accelerometer
   *
   * @include mma7361.c
   */
  
  /**
   * Device context
   */
  typedef struct _mma7361_context {
    // at least one of these must be intialized
    mraa_aio_context    aio_x;
    mraa_aio_context    aio_y;
    mraa_aio_context    aio_z;
    
    // optional - enable selftest
    mraa_gpio_context   gpio_selftest;
    
    // optional - enable sleep mode
    mraa_gpio_context   gpio_sleep;
    
    // optional - detect free fall (0g)
    mraa_gpio_context   gpio_freefall;
    
    // optional - if unconnected, range is 1.5g
    mraa_gpio_context   gpio_range;
    
    // selected G range
    float g_range;

    // analog ADC resolution
    float               a_res;

    // analog reference voltage
    float               a_ref;
    
    // for external offset and scaling of the results
    float               offset_x;
    float               offset_y;
    float               offset_z;

    float               scale_x;
    float               scale_y;
    float               scale_z;
    
    // our measurements
    float               accel_x;
    float               accel_y;
    float               accel_z;

    // volts
    float               volts_x;
    float               volts_y;
    float               volts_z;

    // normalized ADC
    float               normalized_x;
    float               normalized_y;
    float               normalized_z;
  } *mma7361_context;
  
  /**
   * MMA7361 Initializer
   *
   * This sensor isn't designed for navigational purposes - rather
   * it's intended for less precise measurements such as determining
   * tilt and orientation.
   *
   * All of these pins are optional, and will depend on how your
   * device is connected.  For those pins you do not need, supply -1
   * as the pin. You must supply a valid pin for at least one of the
   * axes, or what's the point?
   *
   * The DFRobot variant of this sensor uses a mechanical switch on
   * the board to determine the G-range to use, so on this board you
   * would supply -1 for the range_pin, but be sure to call
   * mma7361_set_range() with the correct parameter so that the
   * internal logic will generate correct results.
   *
   * @param x_pin Analog pin to use for X axis. -1 to disable.
   * @param y_pin Analog pin to use for Y axis. -1 to disable.
   * @param z_pin Analog pin to use for Z axis. -1 to disable.
   * @param selftest_pin GPIO pin to use for self test. -1 to disable.
   * @param sleep_pin GPIO pin to use for sleep function. -1 to disable.
   * @param freefall_pin GPIO pin to use for free fall (0g)
   * detection. -1 to disable.
   * @param range_pin GPIO pin to select range (1.5g or 6g).  -1 to
   * disable.
   * @param a_ref The analog reference voltage in use
   */
  mma7361_context mma7361_init(int x_pin, int y_pin, int z_pin,
                               int selftest_pin, int sleep_pin,
                               int freefall_pin, int range_pin,
                               float a_ref);

  /**
   * MMA7361 sensor close function
   */
  void mma7361_close(mma7361_context dev);

  /**
   * Set the range of the device.  This device supports two G ranges:
   * 1.5 and 6.  The default is 1.5G.
   *
   * @param dev sensor context
   * @param range true for 6G, false for 1.5G
   */
  void mma7361_set_range(const mma7361_context dev, bool range);

  /**
   * Set sleep mode.  When in sleep mode the sensor uses minimal power.
   *
   * @param dev sensor context
   * @param sleep true to go into sleep mode, false to wake up
   * @return UPM result
   */
  upm_result_t mma7361_sleep(const mma7361_context dev, bool sleep);

  /**
   * Get freefall detection status.
   *
   * @param dev sensor context
   * @param freefall O pointer to a boolean value indicating whether a
   * freefall condition is being detected.
   * @return UPM result
   */
  upm_result_t mma7361_freefall(const mma7361_context dev, bool *freefall);

  /**
   * Enable self test mode.
   *
   * @param dev sensor context
   * @param selftest true to enable the self test mode, false otherwise.
   * @return UPM result
   */
  upm_result_t mma7361_selftest(const mma7361_context dev, bool selftest);

  /**
   * Read the sensor status an update internal state.
   * mma7361_update() must have been called before calling
   * mma7361_get_acceleration(), mma7361_get_normalized(), or
   * mma7361_get_volts().
   *
   * @param dev sensor context
   * @return UPM result
   */
  upm_result_t mma7361_update(const mma7361_context dev);

  /**
   * Set sensor offset.  This offset is applied to the return values
   * before scaling.  Default is 0.0.
   *
   * @param dev sensor context pointer
   * @param x Scale to apply to X value
   * @param y Scale to apply to Y value
   * @param z Scale to apply to Z value
   */
  void mma7361_set_offset(const mma7361_context dev, float x, float y,
                          float z);

  /**
   * Set sensor scale.  The acceleration return values are scaled by
   * this value before the offset is applied.  Default is 1.0.
   *
   * @param dev sensor context pointer
   * @param x Offset to apply to X value
   * @param y Offset to apply to Y value
   * @param z Offset to apply to Z value
   */
  void mma7361_set_scale(const mma7361_context dev, float x, float y,
                         float z);

  /**
   * Get computed acceleration from the sensor. mma7361_update() must
   * have been called prior to calling this function.
   *
   * @param dev sensor context pointer
   * @param x a pointer in which X acceleration data will be returned
   * @param y a pointer in which Y acceleration data will be returned
   * @param z a pointer in which Z acceleration data will be returned
   */
  void mma7361_get_acceleration(const mma7361_context dev,
                                float *x, float *y, float *z);

  /**
   * Get the measured volts from the sensor. mma7361_update() must
   * have been called prior to calling this function.
   *
   * @param dev sensor context pointer
   * @param x a pointer in which X volt data will be returned
   * @param y a pointer in which Y volt data will be returned
   * @param z a pointer in which Z volt data will be returned
   */
  void mma7361_get_volts(const mma7361_context dev,
                         float *x, float *y, float *z);

  /**
   * Get the normalized ADC values from the sensor.  mma7361_update() must
   * have been called prior to calling this function.
   *
   * @param dev sensor context pointer
   * @param x a pointer in which X normalized ADC data will be returned
   * @param y a pointer in which Y normalized ADC data will be returned
   * @param z a pointer in which Z normalized ADC data will be returned
   */
  void mma7361_get_normalized(const mma7361_context dev,
                              float *x, float *y, float *z);

#ifdef __cplusplus
}
#endif
