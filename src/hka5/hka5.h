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
#include "mraa/uart.h"
#include "mraa/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @file hka5.h
   * @library hka5
   * @brief C API for the HKA5 Laser PM2.5 Sensor
   *
   * @include hka5.c
   */

  /**
   * Device context
   */
  typedef struct _hka5_context {
    mraa_uart_context        uart;

    // these two are optional
    mraa_gpio_context        gpio_set;
    mraa_gpio_context        gpio_reset;

    // PM 1.0
    uint16_t                 pm1;
    // PM 2.5
    uint16_t                 pm2_5;
    // PM 10.0
    uint16_t                 pm10;
  } *hka5_context;

  /**
   * HKA5 Initializer for generic UART operation
   *
   * @param uart Specify which uart to use.
   * @param set_pin Specify the GPIO pin to use for Set.  -1 to not
   * use a set pin.
   * @param reset_pin Specify the GPIO pin to use for Reset.  -1 to
   * not use a reset pin.
   * @return an initialized device context on success, NULL on error.
   */
  hka5_context hka5_init(unsigned int uart, int set_pin, int reset_pin);

  /**
   * HKA5 sensor close function
   */
  void hka5_close(hka5_context dev);

  /**
   * Enable or disable the device.  When disabled, the device enters a
   * low power mode and does not emit data.  You must have initialized
   * and connected the Set pin for this function to work.
   *
   * @param dev sensor context
   * @param enable true to enable the device, false otherwise.
   * @return UPM result
   */
  upm_result_t hka5_enable(const hka5_context dev, bool enable);

  /**
   * Reset the device.  You must have initialized and connected the
   * Reset pin for this function to work.
   *
   * @param dev sensor context
   * @return UPM result
   */
  upm_result_t hka5_reset(const hka5_context dev);

  /**
   * Query the device and store the latest values.  You must call this
   * function before calling any of the hka5_get_*() functions.
   *
   * @param dev sensor context
   * @return UPM result
   */
  upm_result_t hka5_update(const hka5_context dev);

  /**
   * Return the last read PM 1.0 value.  The value is in micrograms
   * per cubic meter (ug/m3). You must have called the hka5_update()
   * function before calling this function.
   *
   * @param dev sensor context
   * @return The value in ug/m3
   */
  unsigned int hka5_get_pm1(const hka5_context dev);

  /**
   * Return the last read PM 2.5 value.  The value is in micrograms
   * per cubic meter (ug/m3). You must have called the hka5_update()
   * function before calling this function.
   *
   * @param dev sensor context
   * @return The value in ug/m3
   */
  unsigned int hka5_get_pm2_5(const hka5_context dev);

  /**
   * Return the last read PM 10.0 value.  The value is in micrograms
   * per cubic meter (ug/m3). You must have called the hka5_update()
   * function before calling this function.
   *
   * @param dev sensor context
   * @return The value in ug/m3
   */
  unsigned int hka5_get_pm10(const hka5_context dev);

#ifdef __cplusplus
}
#endif
