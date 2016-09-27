/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
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

#include "upm.h"
#include "mraa/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @file water.h
   * @library water
   * @brief C API for the Water Sensor
   *
   * @include water.c
   */

  /**
   * Device context
   */
  typedef struct _water_context {
    mraa_gpio_context        gpio;
  } *water_context;

  /**
   * Water initializer
   *
   * @param pin Digital pin to use
   * @return an initialized device context on success, NULL on error.
   */
  water_context water_init(unsigned int pin);

  /**
   * Water close function
   */
  void water_close(water_context dev);

  /**
   * Gets the water (wet/not wet) value from the sensor.
   *
   * @return true if the sensor is wet, false otherwise
   */
  bool water_is_wet(const water_context dev);

#ifdef __cplusplus
}
#endif
