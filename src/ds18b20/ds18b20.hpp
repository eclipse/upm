/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
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
#include <map>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mraa/common.hpp>
#include <mraa/uart_ow.hpp>

#include <ds18b20.h>

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
     * @comname Programmable Resolution 1-Wire Digital Thermometer
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

      /**
       * DS18B20 object constructor
       *
       * This method will search the 1-wire bus and store information on
       * each device detected on the bus.  If no devices are found, an
       * exception is thrown.  Once this function completes
       * successfully, you can use devicesFound() to determine how many
       * devices were detected.
       *
       * @param uart Default UART to use (0 or 1). Default is 0.
       */
      DS18B20(int uart=DS18B20_DEFAULT_UART);

      /**
       * DS18B20 object destructor
       */
      ~DS18B20();

      /**
       * @deprecated This method is deprecated.  It's functionality is
       * handled in the constructor now.
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
      float getTemperature(unsigned int index, bool fahrenheit=false);

      /**
       * Set the device resolution for a device.  These devices support
       * 9, 10, 11, and 12 bits of resolution, with the default from the
       * factory at 12 bits.
       *
       * @param index The device index to access (starts at 0).
       * @param res One of the RESOLUTIONS_T values
       */
      void setResolution(unsigned int index, DS18B20_RESOLUTIONS_T res);

      /**
       * Copy the device's scratchpad memory to the EEPROM.  This
       * includes the configuration byte (resolution).
       *
       * @param index The device index to access (starts at 0).
       */
      void copyScratchPad(unsigned int index);

      /**
       * Copy the device's EEPROM memory to the scratchpad.  This method
       * will return when the copy completes.  This operation is
       * performed by the device automatically on power up, so it is
       * rarely needed.
       *
       * @param index The device index to access (starts at 0).
       */
      void recallEEPROM(unsigned int index);

      /**
       * This method will return the number of DS18B20 devices that were
       * found on the bus by init().
       *
       * @return number of DS18B20's that were found on the bus
       */
      int devicesFound()
          {
              return ds18b20_devices_found(m_ds18b20);
          }

      /**
       * Return an 8 byte string representing the unique device ID
       * (1-wire romcode) for a given device index.
       *
       * @param index The device index to access (starts at 0).
       * @return 8 byte string representing the 1-wire device's unique
       * romcode.
       */
      std::string getId(unsigned int index);

  protected:
      ds18b20_context m_ds18b20;

  private:
      /* Disable implicit copy and assignment operators */
      DS18B20(const DS18B20&) = delete;
      DS18B20 &operator=(const DS18B20&) = delete;
  };
}
