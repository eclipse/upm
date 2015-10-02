/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

const int MHZ16_DEFAULT_UART = 0;

// protocol start and end codes
const uint8_t MHZ16_START = 0x7e;
const uint8_t MHZ16_END   = 0x7e;

namespace upm {
    /**
     * @brief MHZ16 Serial CO2 Sensor library
     * @defgroup mhz16 libupm-mhz16
     * @ingroup seeed uart gaseous
     */

    /**
     * @library mhz16
     * @sensor mhz16
     * @comname Grove CO2 Sensor
     * @altname MHZ16 Serial CO2 Sensor
     * @type gaseous
     * @man seeed
     * @con uart
     *
     * @brief API support for the Grove CO2 sensor
     *
     * This class implements support for the Grove CO2 sensor.
     *
     * Its CO2 detection range is 0-2,000 ppm. It requires a
     * 2-3 minute warm-up time before reporting valid data.
     *
     * @image html mhz16.jpg
     * @snippet mhz16.cxx Interesting
     */
  class MHZ16 {
  public:


    /**
     * MHZ16 constructor
     *
     * @param uart Default UART to use (0 or 1)
     */
    MHZ16(int uart);

    /**
     * MHZ16 destructor
     */
    ~MHZ16();

    /**
     * Checks to see if there is data available for reading
     *
     * @param millis Number of milliseconds to wait; 0 means no waiting.
     * @return True if there is data available for reading
     */
    bool dataAvailable(unsigned int millis);

    /**
     * Reads any available data in a user-supplied buffer. Note: the
     * call blocks until data is available to be read. Use
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
     * Sets up proper tty I/O modes and the baud rate. The default
     * baud rate is 9,600 (B9600).
     *
     * @param baud Desired baud rate.
     * @return True if successful
     */
    bool setupTty(speed_t baud=B9600);

    /**
     * Verifies the packet header and indicates its validity
     *
     * @param pkt Packet to check
     * @return True if the checksum is valid, false otherwise
     */
    bool verifyPacket(uint8_t *pkt, int len);

    /**
     * Queries the sensor and returns gas (CO2) concentration and
     * temperature data.
     *
     * @param gas Returned gas concentration
     * @param temp Returned temperature in Celsius
     * @return True if successful
     */
    bool getData(int *gas, int *temp);

    /**
     * Sets the zero point of the sensor
     *
     */
    void calibrateZeroPoint();

  protected:
    int ttyFd() { return m_ttyFd; };

  private:
    mraa_uart_context m_uart;
    int m_ttyFd;
  };
}


