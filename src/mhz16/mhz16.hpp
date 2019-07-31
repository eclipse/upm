/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2015 Intel Corporation.
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

#include <interfaces/iTemperature.hpp>
#include <interfaces/iGas.hpp>

#include <mraa/uart.h>

namespace upm {
    /**
     * @brief MHZ16 Serial CO2 Sensor
     * @defgroup mhz16 libupm-mhz16
     * @ingroup seeed uart gaseous
     */

    /**
     * @library mhz16
     * @sensor mhz16
     * @comname CO2 Sensor
     * @altname MHZ16 Serial CO2 Sensor
     * @type gaseous
     * @man seeed
     * @con uart
     * @web http://wiki.seeed.cc/Grove-CO2_Sensor/
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
  class MHZ16: virtual public iGas, virtual public iTemperature {
  public:


    /**
     * MHZ16 constructor
     *
     * @param uart Default mraa UART index to use
     */
    MHZ16(int uart);

    /**
     * MHZ16 constructor
     *
     * @param uart_raw Full path to UART device.
     */
     MHZ16(const std::string& uart_raw = "/dev/ttyS0");

    /**
     * MHZ16 destructor
     */
    virtual ~MHZ16();

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
     * Queries the sensor and gets the gas (CO2) concentration and
     * temperature data.
     * Should be called before other "get" functions.
     *
     * @return True if successful
     */
    bool getData();

    /**
     * Returns the gas (CO2) concentration data.
     *
     * @return Gas concentration
     */
    int getGas();
    float getConcentration();

    /**
     * Returns the temperature data.
     *
     * @return Temperature in Celsius
     */
    float getTemperature();

    /**
     * Sets the zero point of the sensor
     *
     */
    void calibrateZeroPoint();

  protected:
    int ttyFd() { return m_ttyFd; };

  private:
    mraa_uart_context m_uart;
    int m_ttyFd = 0;
    int gas = 0;
    int temp = 0;
  };
}


