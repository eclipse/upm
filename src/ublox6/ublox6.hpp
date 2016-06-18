/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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

const int  UBLOX6_DEFAULT_UART = 0;

namespace upm {
    /**
     * @brief UBLOX6 & SIM28 GPS Module library
     * @defgroup ublox6 libupm-ublox6
     * @ingroup seeed uart gps tsk
     */
    /**
     * @library ublox6
     * @sensor ublox6
     * @comname Grove GPS
     * @altname U-BLOX 6 SIM28
     * @type gps
     * @man seeed
     * @web http://www.seeedstudio.com/depot/Grove-GPS-p-959.html
     * @con uart
     * @kit tsk
     *
     * @brief API for the U-BLOX 6 and SIM28 GPS Modules
     *
     * UPM support for the U-BLOX 6 GPS module. It is also compatible with
     * the SIM28 GPS module.
     *
     * @image html ublox6.jpg
     * @snippet ublox6.cxx Interesting
     */
  class Ublox6 {
  public:
    /**
     * Ublox6 object constructor
     *
     * @param uart Default UART to use (0 or 1)
     */
    Ublox6(int uart);

    /**
     * Ublox6 object destructor
     */
    ~Ublox6();

    /**
     * Checks to see if there is data available for reading
     *
     * @return True if there is data available for reading
     */
    bool dataAvailable();

    /**
     * Reads any available data in a user-supplied buffer. Note: the
     * call blocks until data is available to be read. Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param buffer Buffer to hold the data read
     * @param len Length of the buffer
     * @return the Number of bytes read
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
     * Sets up proper tty I/O modes and the baud rate. The default
     * baud rate is 9,600 (B9600).
     *
     * @param baud Desired baud rate
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


