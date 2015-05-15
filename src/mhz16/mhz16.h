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
     * @brief MHZ16 Serial CO2 sensor library
     * @defgroup mhz16 libupm-mhz16
     * @ingroup seeed serial gaseous
     */

    /**
     * @library mhz16
	 * @sensor mhz16
     * @comname Grove CO2 sensor
     * @type gaseous
     * @man seeed
     * @con uart
     *
     * @brief C++ API support for the Grove CO2 sensor
     *
     * This class implements support for the Grove CO2 sensor.
     *
     * It's CO2 detection ranges from 0PPM to 2000PPM.  It requires a
     * 2-3 minute warm up time before reporting valid data.
     * 
     * @snippet mhz16.cxx Interesting
     */
  class MHZ16 {
  public:

    
    /**
     * MHZ16 module constructor
     *
     * @param uart default uart to use (0 or 1)
     */
    MHZ16(int uart);

    /**
     * MHZ16 module Destructor
     */
    ~MHZ16();

    /**
     * Check to see if there is data available for reading
     *
     * @param millis number of milliseconds to wait, 0 means no wait.
     * @return true if there is data available to be read
     */
    bool dataAvailable(unsigned int millis);

    /**
     * read any available data into a user-supplied buffer.  Note, the
     * call will block until data is available to be read.  Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param buffer the buffer to hold the data read
     * @param len the length of the buffer
     * @return the number of bytes read
     */
    int readData(char *buffer, size_t len);

    /**
     * write the data in buffer to the device
     *
     * @param buffer the buffer to hold the data read
     * @param len the length of the buffer
     * @return the number of bytes written
     */
    int writeData(char *buffer, size_t len);

    /**
     * setup the proper tty i/o modes and the baudrate.  The default
     * baud rate is 9600 (B9600).
     *
     * @param baud the desired baud rate.  
     * @return true if successful
     */
    bool setupTty(speed_t baud=B9600);

    /**
     * verify the packet header and indicate it's validity
     *
     * @param pkt the packet to check
     * @return true if valid checksum, false otherwise
     */
    bool verifyPacket(unsigned char *pkt);

    /**
     * Query the sensor and return gas (CO2) concentration and
     * temperature data.
     *
     * @param gas the returned gas concentration
     * @param temp the returned temperature in celsius
     * @return true if successful
     */
    bool getData(int *gas, int *temp);

    /**
     * Set the zero point of the sensor
     *
     */
    void calibrateZeroPoint();

  protected:
    int ttyFd() { return m_ttyFd; };
    int setTtyFd(int fd) { m_ttyFd = fd; };

  private:
    mraa_uart_context m_uart;
    int m_ttyFd;
  };
}


