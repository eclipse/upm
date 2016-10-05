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

#include <string>
#include <iostream>
#include <map>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mraa/common.hpp>
#include <mraa/uart_ow.hpp>

#define DS2413_DEFAULT_UART 0

namespace upm {
    /**
     * @brief DS2413 modules
     * @defgroup ds2413 libupm-ds2413
     * @ingroup maxim uart gpio
     */

    /**
     * @library ds2413
     * @sensor ds2413
     * @comname DS2413 1-Wire Dual Channel Addressable Switch
     * @type gpio
     * @man maxim
     * @con uart
     * @web https://learn.adafruit.com/adafruit-1-wire-gpio-breakout-ds2413/overview
     *
     * @brief API for the DS2413 1-Wire Dual Channel Addressable Switch
     *
     * The DS2413 is a dual-channel programmable I/O 1-Wire(r) chip. The
     * PIO outputs are configured as open-drain and provide up to 20mA
     * continuous sink capability and off-state operating voltage up
     * to 28V.  Control and sensing of the PIO pins is performed with
     * a dedicated device-level command protocol.
     *
     * This device requires the use of a UART to provide access to a
     * Dallas 1-wire bus, via a new facility supported by MRAA (once
     * the relevant PR is accepted), using the UartOW access class.
     * It is important to realize that the UART is only being used to
     * access and control a Dallas 1-wire compliant bus, it is not
     * actually a UART device.
     *
     * Multiple DS2413 devices can be connected to this bus.  This
     * module will identify all such devices connected, and allow you
     * to access them using an index starting at 0.
     *
     * @snippet ds2413.cxx Interesting
     */

  class DS2413 {
  public:

    // The family code for these devices.  We handle all of them that
    // are found on the bus.
    static const uint8_t DS2413_FAMILY_CODE = 0x3a;

    // commands
    typedef enum {
      ACCESS_READ                         = 0xf5,
      ACCESS_WRITE                        = 0x5a
    } DS2413_CMD_T;

    // possible ack responses for gpio writes
    typedef enum {
      ACK_SUCCESS                         = 0xaa,
      ACK_FAILURE                         = 0xff
    } DS2413_ACK_T;

    /**
     * DS2413 object constructor
     *
     * @param uart Default UART to use (0 or 1). Default is 0.
     */
    DS2413(int uart=DS2413_DEFAULT_UART);

    /**
     * DS2413 object destructor
     */
    ~DS2413();

    /**
     * This method will search the 1-wire bus and store information on
     * each device detected on the bus.  If no devices are found, an
     * exception is thrown.  Once this function completes
     * successfully, you can use devicesFound() to determine how many
     * devices were detected.
     */
    void init();

    /**
     * This method will return the number of DS2413 devices that were
     * found on the bus.
     *
     * @return number of DS2413's that were found on the bus
     */
    int devicesFound()
    {
      return m_devicesFound;
    }

    /**
     * Read the GPIO latches and values for a given device.  Only the
     * lower 4 bits of the return value have any meaning.  The bits
     * are formatted as follows (from msb to lsb):
     *  <gpioB latch> <gpioB value> <gpioA latch> <gpioA value>
     *
     * @param index The device index to access (starts at 0). Default
     * is the first device (index = 0)
     * @return the 4 bit status of the 2 gpios
     */
    int readGpios(int index=0);

    /**
     * Write the given values to the gpio latches for a given device.
     * Only the lower 2 bits of the value have any meaning.  The lsb
     * is gor gpioA and the next lsb is for gpioB:
     *  <gpioB latch> <gpioA latch>
     *
     * @param index The device index to access (starts at 0).
     * @param value the value to write to the gpio latches
     */
    void writeGpios(int index, int value);

    /**
     * Return an 8 byte string representing the unique device ID for a
     * given device index.  If the index is invalid, an empty strig
     * will be returned.
     *
     * @param index The device index to access (starts at 0).
     * @return 8 byte string representing the 1-wire device's unique
     * rom code.
     */
    std::string getId(int index)
    {
      return m_deviceMap[index];
    }

  protected:
    mraa::UartOW m_uart;

    // the total number of devices found
    int m_devicesFound;

    std::map<int, std::string> m_deviceMap;

  private:
  };
}
