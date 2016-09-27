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
#include "mraa/i2c.h"
#include "mraa/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

  /**
   * @file nmea-gps.h
   * @library nmea-gps
   * @brief C API for NMEA GPS Sensors
   *
   * An example using the UART.
   * @include nmea_gps.c
   * An example using I2C.
   * @include nmea_gps_i2c.c
   */
  
  /**
   * Device context
   */
  typedef struct _nmea_gps_context {
    mraa_uart_context        uart;
    mraa_gpio_context        gpio_en;
    mraa_i2c_context         i2c;
  } *nmea_gps_context;
  
  /**
   * NMEA_GPS Initializer for generic UART operation
   *
   * @param uart Specify which uart to use.
   * @param baudrate Specify the baudrate to use.  The device defaults
   * to 9600 baud.
   * @param enable_pin Specify the GPIO pin to use for the enable pin,
   * -1 to not use an enable pin.
   * @return an initialized device context on success, NULL on error.
   */
  nmea_gps_context nmea_gps_init(unsigned int uart, unsigned int baudrate,
                                 int enable_pin);

  /**
   * NMEA_GPS Initializer for UBLOX I2C operation
   *
   * @param bus Specify which the I2C bus to use.
   * @param addr Specify the I2C address to use.  For UBLOX devices,
   * this typically defaults to 0x42.
   * @return an initialized device context on success, NULL on error.
   */
  nmea_gps_context nmea_gps_init_ublox_i2c(unsigned int bus, uint8_t addr);

  /**
   * NMEA_GPS sensor close function
   */
  void nmea_gps_close(nmea_gps_context dev);

  /**
   * Read character data from the device.
   *
   * @param dev sensor context
   * @param buffer The character buffer to read data into.
   * @param len The maximum size of the buffer
   * @return The number of bytes successfully read, or -1 on error
   */
  int nmea_gps_read(const nmea_gps_context dev, char *buffer, size_t len);

  /**
   * Write character data to the device.  This is only valid for a
   * UART device.
   *
   * @param dev sensor context
   * @param buffer The character buffer containing data to write.
   * @param len The number of bytes to write.
   * @return The number of bytes successfully written, or -1 on error.
   */
  int nmea_gps_write(const nmea_gps_context dev, char *buffer, size_t len);

  /**
   * Enable or disable the device.  When disabled, the device enters a
   * low power mode and does not emit NMEA data.  It will still
   * maintain location data however.
   *
   * @param dev sensor context
   * @param enable true to enable the device, false otherwise.
   * @return UPM result
   */
  upm_result_t nmea_gps_enable(const nmea_gps_context dev, bool enable);

  /**
   * Set the baudrate of the device.  By default, nmea_gps_init() will
   * set the baudrate to 9600.  This is only valid for UART devices.
   *
   * @param dev sensor context
   * @param baudrate The baud rate to set for the device.
   * @return UPM result
   */
  upm_result_t nmea_gps_set_baudrate(const nmea_gps_context dev,
                                     unsigned int baudrate);

  /**
   * Determine whether there is data available to be read.  In the
   * case of a UART, this function will wait up to "millis"
   * milliseconds for data to become available.  In the case of an I2C
   * device, the millis argument is ignored and the function will
   * return immediately, indicating whether data is available.
   *
   * @param dev sensor context
   * @param millis The number of milliseconds to wait for data to
   * become available.
   * @return true if data is available to be read, false otherwise.
   */
  bool nmea_gps_data_available(const nmea_gps_context dev,
                               unsigned int millis);

#ifdef __cplusplus
}
#endif
