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

#define DS18B20_DEFAULT_UART 0

namespace upm {
    /**
     * @brief DS18B20 1-Wire Temperature Sensor
     * @defgroup ds18b20 libupm-ds18b20
     * @ingroup maxim uart
     */

    /**
     * @library ds18b20
     * @sensor ds18b20
     * @comname DS18B20 1-Wire Temperature Sensor
     * @type uart
     * @man maxim
     * @con uart
     * @web https://www.sparkfun.com/products/11050
     *
     * @brief API for the DS18B20 1-Wire Temperature Sensor
     *
     * This driver supports, and was tested with, a DS18B20 with
     * external power.
     *
     * Multiple DS18B20 devices can be connected to this bus.  This
     * module will identify all such devices connected, and allow you
     * to access them using an index starting at 0.
     *
     * Parasitic power is not currently supported due
     * to the very tight 10us limit on switching a GPIO properly to
     * supply power during certain operations.  For this reason, you
     * should use external power for your sensors.
     *
     * Setting the alarm values (Tl, Th) is also not supported, since
     * this is only useful when doing a 1-wire device search looking
     * for devices in an alarm state, a capability not yet supported
     * in MRAA.  In reality, this is trivial to handle yourself in
     * your application.
     *
     * This device requires the use of a TTL level UART (specifically
     * through the UartOW MRAA context class) to provide access to a
     * Dallas 1-wire bus. It is important to realize that the UART is
     * only being used to provide an interface to devices on a Dallas
     * 1-wire compliant bus.
     *
     * A circuit like the following should be used for the UART
     * interface (ASCII schematic - best viewed in a fixed monospace
     * font):
     *
     * -|
     * U|      (D1)
     * A| TX---|<--+
     * R|          |
     * T| RX-------o--------o  >to 1-wire data bus
     * -|
     *
     *
     *             Vcc
     *              |
     *              o------------+
     *              |            |
     *              R1           |
     *              |            o(+)
     * >1-wire------o--------o[DS18B20]
     *                           o(-)
     *                           |
     *                          GND
     *
     * D1 = 1N4148
     * R1 = 4.7K
     *
     * The cathode of D1 is connected to the UART TX.
     *
     * @snippet ds18b20.cxx Interesting
     */

  class DS18B20 {
  public:

    // The family code for these devices.  We handle all of them that
    // are found on the bus.
    static const uint8_t DS18B20_FAMILY_CODE = 0x28;

    // commands
    typedef enum {
      CMD_CONVERT                         = 0x44, // start a temp conversion
      CMD_WRITE_SCRATCHPAD                = 0x4e,
      CMD_READ_SCRATCHPAD                 = 0xbe,
      CMD_COPY_SCRATCHPAD                 = 0x48, // copy scratchpad to EEPROM
      CMD_RECALL_EEPROM                   = 0xb8, // copy EEPROM to scratchpad
      CMD_READ_POWER_SUPPLY               = 0xb4  // parasitically powered?
    } CMD_T;

    // config register (scratchpad[4])
    typedef enum {
      CFG_RESOLUTION_R0                   = 0x20,
      CFG_RESOLUTION_R1                   = 0x40,
      _CFG_RESOLUTION_MASK                = 3,
      _CFG_RESOLUTION_SHIFT               = 5

      // all other bits reserved and non-writable
    } CFG_BITS_T;

    typedef enum {
      RESOLUTION_9BITS                    = 0, // 93.75ms (tconv/8)
      RESOLUTION_10BITS                   = 1, // 187.5 (tconv/4)
      RESOLUTION_11BITS                   = 2, // 375ms (tconv/2)
      RESOLUTION_12BITS                   = 3  // 750ms (tconv)
    } RESOLUTIONS_T;

    /**
     * DS18B20 object constructor
     *
     * @param uart Default UART to use (0 or 1). Default is 0.
     */
    DS18B20(int uart=DS18B20_DEFAULT_UART);

    /**
     * DS18B20 object destructor
     */
    ~DS18B20();

    /**
     * This method will search the 1-wire bus and store information on
     * each device detected on the bus.  If no devices are found, an
     * exception is thrown.  Once this function completes
     * successfully, you can use devicesFound() to determine how many
     * devices were detected.  This method must be executed first
     * before any others below.
     */
    void init();

    /**
     * Update our stored temperature for a device.  This method must
     * be called prior to getTemperature().
     *
     * @param index The device index to access (starts at 0).  Specify
     * -1 to query all detected devices.  Default: -1
     */
    void update(int index=-1);

    /**
     * Get the current temperature.  update() must have been called
     * prior to calling this method.
     *
     * @param index The device index to access (starts at 0).
     * @param fahrenheit true to return the temperature in degrees
     * fahrenheit, false to return the temperature in degrees celsius.
     * The default is false (degrees Celsius).
     * @return The last temperature reading in Celsius or Fahrenheit
     */
    float getTemperature(int index, bool fahrenheit=false);

    /**
     * Set the device resolution for a device.  These devices support
     * 9, 10, 11, and 12 bits of resolution, with the default from the
     * factory at 12 bits.
     *
     * @param index The device index to access (starts at 0).
     * @param res One of the RESOLUTIONS_T values
     */
    void setResolution(int index, RESOLUTIONS_T res);

    /**
     * Copy the device's scratchpad memory to the EEPROM.  This
     * includes the configuration byte (resolution).
     *
     * @param index The device index to access (starts at 0).
     */
    void copyScratchPad(int index);

    /**
     * Copy the device's EEPROM memory to the scratchpad.  This method
     * will return when the copy completes.  This operation is
     * performed by the device automatically on power up, so it is
     * rarely needed.
     *
     * @param index The device index to access (starts at 0).
     */
    void recallEEPROM(int index);

    /**
     * This method will return the number of DS18B20 devices that were
     * found on the bus by init().
     *
     * @return number of DS18B20's that were found on the bus
     */
    int devicesFound()
    {
      return m_devicesFound;
    }

    /**
     * Return an 8 byte string representing the unique device ID
     * (1-wire romcode) for a given device index.
     *
     * @param index The device index to access (starts at 0).
     * @return 8 byte string representing the 1-wire device's unique
     * romcode.
     */
    std::string getId(int index)
    {
      if (index < 0 || index >= m_devicesFound)
        {
          throw std::out_of_range(std::string(__FUNCTION__) +
                                  ": device index out of range");
        }
      return m_deviceMap[index].id;
    }

  protected:
    mraa::UartOW m_uart;

    // the total number of devices found
    int m_devicesFound;

    // this struct will generate SWIG warnings on build, but as it's not
    // exposed outside the class, they can be safely ignored

    // data we need to store for each sensor we are dealing with
    typedef struct {
      std::string id;           // 8-byte romcode id
      float temperature;
      RESOLUTIONS_T resolution;
    } sensor_info_t;

    std::map<int, sensor_info_t> m_deviceMap;

  private:
    // internal utility function to read temperature from a single
    // device
    float readSingleTemp(int index);
  };
}
