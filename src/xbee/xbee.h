/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Thanks to Adafruit for supplying a google translated version of the
 * Chinese datasheet and some clues in their code.
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

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mraa/common.hpp>
#include <mraa/uart.hpp>

#define XBEE_DEFAULT_UART 0

namespace upm {
    /**
     * @brief XBee modules
     * @defgroup xbee libupm-xbee
     * @ingroup sparkfun uart wifi
     */

    /**
     * @library xbee
     * @sensor xbee
     * @comname XBee modules
     * @type wifi
     * @man sparkfun
     * @con uart
     * @web https://www.sparkfun.com/products/11215
     *
     * @brief API for the XBee modules
     *
     * This is a generic UART driver for use with Digi XBee modules.
     * It was tested with the XBee S1 802.15.4 module and the XBee S6B
     * WiFi module.
     *
     * It provides basic UART support for sending and receiving data
     * to and from the device.  It is controlled by an AT or API
     * command set.
     *
     * It is connected at 9600 baud by default.
     *
     * These devices are typically configured using Digi's X-CTU
     * windows software, however it is possible of course to configure
     * them manually using AT commands.  See the examples.
     *
     * @snippet xbee.cxx Interesting
     */

  class XBee {
  public:

    /**
     * XBee object constructor
     *
     * @param uart Default UART to use (0 or 1). Default is 0.
     */
    XBee(int uart=XBEE_DEFAULT_UART);

    /**
     * XBee object destructor
     */
    ~XBee();

    /**
     * Checks to see if there is data available for reading
     *
     * @param millis Number of milliseconds to wait; 0 means no waiting
     * @return true if there is data available for reading
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
    int readData(char *buffer, unsigned int len);

    /**
     * Reads any available data and returns it in a std::string. Note:
     * the call blocks until data is available for reading. Use
     * dataAvailable() to determine whether there is data available
     * beforehand, to avoid blocking.
     *
     * @param len Maximum length of the data to be returned
     * @return string containing the data read
     */
    std::string readDataStr(int len);

    /**
     * Writes the data in the buffer to the device.  If you are
     * writing an AT command, be sure to terminate it with a carriage
     * return (\r)
     *
     * @param buffer Buffer to hold the data to write
     * @param len Length of the buffer
     * @return Number of bytes written
     */
    int writeData(char *buffer, unsigned len);

    /**
     * Writes the std:string data to the device.  If you are writing an
     * AT command, be sure to terminate it with a carriage return (\r)
     *
     * @param data Buffer to write to the device
     * @return Number of bytes written
     */
    int writeDataStr(std::string data);

    /**
     * Sets the baud rate for the device.  The default is 9600.
     *
     * @param baud Desired baud rate
     * @return true if successful
     */
    mraa::Result setBaudRate(int baud=9600);

    /**
     * Attempts to enter AT Command Mode.  When Idle, data sent to the
     * device (assuming it is not in API mode) is silently transmitted
     * to the configured destination.  Running this command attempts
     * to place the device into command mode, allowing you to send AT
     * commands.  Note, after a configurable period of inactivity, the
     * device will exit command mode automatically (default 10
     * seconds).
     *
     * Both the cmdChars (+++) and the Guard Time can be configured
     * on the device to different values using AT configuration
     * commands.
     *
     * @param cmdChars The command mode characters, default "+++"
     * @param guardTimeMS The number of milliseconds to wait before and
     * after sending the command characters.  Default is 1000 (1 second).
     * @return true if successful (received an "OK"), false otherwise
     */
    bool commandMode(std::string cmdChars="+++", int guardTimeMS=1000);

    /**
     * This is a convenience method that converts each CR (\r) in a
     * string to a LF (\n) and returns it.  This is useful for
     * outputting the response to an AT command for instance, which is
     * typically CR terminated.
     *
     * @param str The string to convert
     * @return The converted string
     */
    std::string stringCR2LF(std::string str);

  protected:
    mraa::Uart m_uart;

  private:
  };
}


