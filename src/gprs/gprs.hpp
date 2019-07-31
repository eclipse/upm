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

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mraa/common.hpp>
#include <mraa/uart.hpp>

#define GPRS_DEFAULT_UART 0

namespace upm {
    /**
     * @brief GPRS Modules Library
     * @defgroup gprs libupm-gprs
     * @ingroup seeed uart wifi
     */

    /**
     * @library gprs
     * @sensor gprs
     * @comname General Packet Radio Service (GPRS) Module
     * @altname Grove GPRS Module
     * @type wifi
     * @man seeed
     * @con uart
     * @web http://www.seeedstudio.com/wiki/GPRS_Shield_V2.0
     *
     * @brief API for the GPRS Module
     *
     * The driver was tested with the  GPRS Module, V2.  It's a
     * GSM GPRS module based on the SIM900.  This module uses a
     * standard 'AT' command set.  See the datasheet for a full list
     * of available commands and their possible responses:
     *
     * http://www.seeedstudio.com/wiki/images/7/72/AT_Commands_v1.11.pdf
     *
     * It is connected via a UART at 19200 baud.
     *
     * @image html gprs.jpg
     * @snippet gprs.cxx Interesting
     */

  class GPRS {
  public:

    /**
     * GPRS object constructor
     *
     * @param uart Default UART to use (0 or 1). Default is 0.
     */
    GPRS(int uart=GPRS_DEFAULT_UART);

    /**
     * GPRS object destructor
     */
    ~GPRS();

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
     * @return Number of bytes read
     */
    std::string readDataStr(int len);

    /**
     * Writes the data in the buffer to the device.  If you are
     * writing a command, be sure to terminate it with a carriage
     * return (\r)
     *
     * @param buffer Buffer to hold the data to write
     * @param len Length of the buffer
     * @return Number of bytes written
     */
    int writeData(char *buffer, unsigned len);

    /**
     * Writes the std:string data to the device.  If you are writing a
     * command, be sure to terminate it with a carriage return (\r)
     *
     * @param data Buffer to write to the device
     * @return Number of bytes written
     */
    int writeDataStr(std::string data);

    /**
     * Sets the baud rate for the device.  The default is 19200.
     *
     * @param baud Desired baud rate.
     * @return true if successful
     */
    int setBaudRate(int baud=19200);


  protected:
    mraa::Uart m_uart;

  private:
  };
}


