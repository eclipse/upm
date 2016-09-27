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
#include "mraa/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @file sht1x.h
   * @library sht1x
   * @brief C API for SHT1X Temperature and Humidity Sensors
   *
   * @include sht1x.c
   */

  /**
   * Device context
   */
  typedef struct _sht1x_context {
    mraa_gpio_context        gpio_clk;
    mraa_gpio_context        gpio_data;

    // high res? (temp/hum 14/12b vs. 12/8b)
    bool                     hires;

    // our data
    float                    temperature;
    float                    humidity;

    // temperature coeff (we only care about C)
    float                    coeff_d1;
    float                    coeff_d2;

    // humidity coeff
    float                    coeff_c1;
    float                    coeff_c2;
    float                    coeff_c3;

    float                    coeff_t1;
    float                    coeff_t2;
  } *sht1x_context;

  // SHT1X commands.  The first 3 msb's are the address, which are
  // always 0.  The following 5 bits are the actual command.
  typedef enum {
    SHT1X_CMD_MEAS_TEMPERATURE            = 0x03,
    SHT1X_CMD_MEAS_HUMIDITY               = 0x05,
    SHT1X_CMD_WRITE_STATUS                = 0x06,
    SHT1X_CMD_READ_STATUS                 = 0x07,
    SHT1X_CMD_SOFT_RESET                  = 0x1e
  } SHT1X_CMD_T;

  // status register bits
  typedef enum {
    SHT1X_STATUS_RESOLUTION_LOW           = 0x01, // 0=12b RH/14b temp (dflt)
    SHT1X_STATUS_NO_RELOAD_FROM_OTP       = 0x02,
    SHT1X_STATUS_HEATER_EN                = 0x04,

    // 0x08-0x20 reserved

    SHT1X_STATUS_LOW_VOLT                 = 0x40 // low battery

    // 0x80 reserved
  } SHT1X_STATUS_BITS_T;

  // The Vdd voltage can affect the temperature coefficients, so we
  // provide a way to indicate the closest voltage and set up the
  // compensation accordingly.
  typedef enum {
    SHT1X_VOLTS_5                         = 0, // 5 volts
    SHT1X_VOLTS_4                         = 1,
    SHT1X_VOLTS_3_5                       = 2, // 3.5v
    SHT1X_VOLTS_3                         = 3,
    SHT1X_VOLTS_2_5                       = 4
  } SHT1X_VOLTS_T;

  /**
   * SHT1X Initializer
   *
   * @param clk_pin Specify the GPIO pin to use for the clock.
   * @param data_pin Specify the GPIO pin to use for data.
   * @return an initialized device context on success, NULL on error.
   */
  sht1x_context sht1x_init(unsigned int clk_pin, unsigned int data_pin);

  /**
   * SHT1X sensor close function
   */
  void sht1x_close(sht1x_context dev);

  /**
   * Perform a soft reset of the device.
   *
   * @param dev sensor context
   * @return UPM result
   */
  void sht1x_reset(const sht1x_context dev);

  /**
   * Query the device and store the latest values.  You must call this
   * function before querying the temperature or the humidity.
   *
   * @param dev sensor context
   * @return UPM result
   */
  upm_result_t sht1x_update(const sht1x_context dev);

  /**
   * Query the temperature in degrees Celsius.  sht1x_update() must
   * have been called prior to calling this function.
   *
   * @param dev sensor context
   * @return The temperature in Celsius
   */
  float sht1x_get_temperature(const sht1x_context dev);

  /**
   * Query the relative humidity.  sht1x_update() must have been
   * called prior to calling this function.
   *
   * @param dev sensor context
   * @return The relative humidity.
   */
  float sht1x_get_humidity(const sht1x_context dev);

  /**
   * Read the status register.
   *
   * @param dev sensor context
   * @param status The pointer to a uint8_t the status register will
   * be stored in.
   * @return UPM result
   */
  upm_result_t sht1x_read_status(const sht1x_context dev, uint8_t *status);

  /**
   * Write a value to the status register.
   *
   * @param dev sensor context
   * @param status The uint8_t to write to the register.
   * @return UPM result
   */
  upm_result_t sht1x_write_status(const sht1x_context dev, uint8_t status);

  /**
   * The Vdd voltage the sensor is being driven at can affect the
   * temperature measurements.  This function allows you to specify a
   * voltage as close as you are using to power the sensor, so that
   * the appropriate compensation can be made.  By default, the
   * coefficients are set for 5v operation.
   *
   * @param dev sensor context
   * @param volts One of the SHT1X_VOLTS_T values.
   */
  void sht1x_set_volts(const sht1x_context dev, SHT1X_VOLTS_T volts);

  /**
   * Send a command to the device.  This is a low level command that
   * should not be used directly unless you know exactly what you are
   * doing.
   *
   * @param dev sensor context
   * @param cmd One of the SHT1X_CMD_T values.
   * @return UPM result
   */
  upm_result_t sht1x_send_command(const sht1x_context dev, SHT1X_CMD_T cmd);

  /**
   * Wait for the appropriate response when starting a temperature or
   * humidity measurement.  This is a low level command that should
   * not be used directly unless you know exactly what you are doing.
   *
   * @param dev sensor context
   * @return UPM result
   */
  upm_result_t sht1x_wait_for_response(const sht1x_context dev);

  /**
   * Issue the start transmission sequence.  This is a low level
   * command that should not be used directly unless you know exactly
   * what you are doing.
   *
   * @param dev sensor context
   * @return UPM result
   */
  void sht1x_start_xmit(const sht1x_context dev);

  /**
   * Read 8 bits (a byte) from the device.  This is a low level
   * command that should not be used directly unless you know exactly
   * what you are doing.
   *
   * @param dev sensor context
   * @param value Pointer to a uint8_t that will hold the received byte.
   * @return UPM result
   */
  void sht1x_read_8bits(const sht1x_context dev, uint8_t *value);

  /**
   * Write 8 bits (a byte) to the device.  This is a low level command
   * that should not be used directly unless you know exactly what you
   * are doing.
   *
   * @param dev sensor context
   * @param value A uint8_t that will be sent to the device.
   * @return UPM result
   */
  upm_result_t sht1x_write_8bits(const sht1x_context dev, uint8_t byte);

#ifdef __cplusplus
}
#endif
