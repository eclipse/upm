/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Thanks to Adafruit for supplying a google translated version of the
 * Chinese datasheet and some clues in their code.
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

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <mraa/uart.h>

#define HM11_DEFAULT_UART 0

namespace upm {
    /**
     * @brief HM-11 Bluetooth 4.0 Low Energy Module
     * @defgroup hm11 libupm-hm11
     * @ingroup seeed uart wifi
     */

    /**
     * @library hm11
     * @sensor hm11
     * @comname Bluetooth Low Energy Module
     * @altname Grove BLE
     * @altid HM-10, HM-12
     * @type wifi
     * @man seeed
     * @con uart
     * @web http://wiki.seeed.cc/Bluetooth_V4.0_HM_11_BLE_Module/
     *
     * @brief API for the HM-11 4.0 Bluetooth Low Energy Module
     *
     * The driver was tested with the Grove BLE module.  It's an HM-11
     * BLE 4.0 module based on a TI CC2541 chip.  It operates using a
     * standard 'AT' command set.  See the datasheet for a full list
     * of available commands and their possible responses:
     *
     * http://www.seeedstudio.com/wiki/images/c/cd/Bluetooth4_en.pdf
     *
     * It is connected via a UART at 9,600 baud.
     *
     * @image html hm11.jpg
     * @snippet hm11.cxx Interesting
     */

  class HM11 {
  public:

    /**
     * HM11 object constructor
     *
     * @param uart Default UART to use (0 or 1)
     */
    HM11(int uart);

    /**
     * HM11 object destructor
     */
    ~HM11();

    /**
     * Checks to see if there is data available for reading
     *
     * @param millis Number of milliseconds to wait; 0 means no waiting
     * @return True if there is data available for reading
     */
    bool dataAvailable(unsigned int millis);

    /**
     * Reads any available data into a user-supplied buffer. Note: the
     * call blocks until data is available for reading. Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes read
     */
    int readData(char *buffer, int len);

    /**
     * Writes the data in the buffer to the device
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return Number of bytes written
     */
    int writeData(char *buffer, int len);

    /**
     * Sets up proper tty I/O modes and the baud rate. For this device, the default
     * baud rate is 9,600 (B9600).
     *
     * @param baud Desired baud rate.
     * @return True if successful
     */
    bool setupTty(speed_t baud=B9600);


  protected:
    int ttyFd() { return m_ttyFd; };

  private:
    mraa_uart_context m_uart;
    int m_ttyFd;
  };
}


