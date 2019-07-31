/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014-2016 Intel Corporation.
 *
 * This program and the accompanying materials are made available under the
 * terms of the The MIT License which is available at
 * https://opensource.org/licenses/MIT.
 *
 * SPDX-License-Identifier: MIT
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
