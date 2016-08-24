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
   * @brief UPM C API for the DFRobot VK2828U7 GPS Module
   *
   * The driver was tested with the DFRobot VK2828U7 GPS Module.  It
   * is accessed via a UART and provides NMEA data.
   *
   * @snippet vk2828u7.c Interesting
   */
  
  /**
   * Device context
   */
  typedef struct _vk2828u7_context {
    mraa_uart_context        uart;
    mraa_gpio_context        gpio_en;
  } *vk2828u7_context;
  
  /**
   * VK2828U7 Initializer
   *
   * @param uart Specify which uart to use.
   * @param baudrate Specify the baudrate to use.  The device defaults
   * to 9600 baud.
   * @param enable_pin Specify the GPIO pin to use for the enable pin,
   * -1 to not use an enable pin.
   * @return an initialized device context on success, NULL on error.
   */
  vk2828u7_context vk2828u7_init(unsigned int uart, unsigned int baudrate,
                                 int enable_pin);

  /**
   * VK2828U7 sensor close function
   */
  void vk2828u7_close(vk2828u7_context dev);

  /**
   * Read character data from the device.
   *
   * @param dev sensor context
   * @param buffer The character buffer to read data into.
   * @param len The maximum size of the buffer
   * @return The number of bytes successfully read, or -1 on error
   */
  int vk2828u7_read(const vk2828u7_context dev, char *buffer, size_t len);

  /**
   * Write character data to the device.
   *
   * @param dev sensor context
   * @param buffer The character buffer containing data to write.
   * @param len The number of bytes to write.
   * @return The number of bytes successfully written, or -1 on error.
   */
  int vk2828u7_write(const vk2828u7_context dev, char *buffer, size_t len);

  /**
   * Enable or disable the device.  When disabled, the device enters a
   * low power mode and does not emit NMEA data.  It will still
   * maintain location data however.
   *
   * @param dev sensor context
   * @param enable true to enable the device, false otherwise.
   * @return UPM result
   */
  upm_result_t vk2828u7_enable(const vk2828u7_context dev, bool enable);

  /**
   * Set the baudrate of the device.  By default, vk2828u7_init() will
   * set the baudrate to 9600.
   *
   * @param dev sensor context
   * @param baudrate The baud rate to set for the device.
   * @return UPM result
   */
  upm_result_t vk2828u7_set_baudrate(const vk2828u7_context dev,
                                     unsigned int baudrate);

  /**
   * Determine whether there is data available to be read.
   *
   * @param dev sensor context
   * @param millis The number of milliseconds to wait for data to
   * become available.
   * @return true if data is available to be read, false otherwise.
   */
  bool vk2828u7_data_available(const vk2828u7_context dev,
                               unsigned int millis);

#ifdef __cplusplus
}
#endif
