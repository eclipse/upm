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

#include <modbus/modbus.h>

namespace upm {

  /**
   * @brief T3311 Temperature and Humidity Sensor
   * @defgroup t3311 libupm-t3311
   * @ingroup uart temp
   */

  /**
   * @library t3311
   * @sensor t3311
   * @comname T3311 Temperature and Humidity Sensor
   * @type temp
   * @man comet
   * @con uart
   * @web http://www.cometsystem.com/products/reg-T3311
   *
   * @brief UPM API for the T3311 MODBUS Temperature and Humidity Sensor
   *
   * This module implements support for the Comet System T3311
   * Temperature and Humidity transmitter.  It uses MODBUS over an
   * RS232 serial port.  You must have libmodbus v3.1.2 (or greater)
   * installed to compile and use this driver.
   *
   * This module was developed using libmodbus 3.1.2, and T3311
   * Firmware revison 2.66 connected via a Prolific RS232 Serial to
   * USB adaptor. You cannot use the built in TTL UART pins for
   * accessing this device -- you must use a full serial RS232
   * interface connected via USB.
   *
   * @snippet t3311.cxx Interesting
   */

  class T3311 {
  public:

    // MODBUS input registers
    typedef enum {
      REG_TEMPERATURE                       = 0x0030,
      REG_HUMIDITY                          = 0x0031,

      // This value is configured in the device itself.  By default,
      // it represents the Dew Point Temperature.  Use the
      // configuration utility from Comet to adjust the configuration.
      REG_COMPUTED                          = 0x0032,

      // available in devices with FW > 2.44
      REG_DEW_POINT                         = 0x0034,
      REG_ABS_HUMIDITY                      = 0x0035,
      REG_SPECIFIC_HUMIDITY                 = 0x0036,
      REG_MIXING_RATIO                      = 0x0037,
      REG_SPECIFIC_ENTHALPY                 = 0x0038,

      // 32 bit serial number.  These appear to be reversed when
      // comparing against the TSensor config utility, so the
      // datasheet is probably wrong.
      REG_SERIAL_HI                         = 0x1034,
      REG_SERIAL_LO                         = 0x1035,

      // this is 'somewhat' documented (middle of page 15 in the
      // Advantech-ADAM standard section) in the "Description of
      // communications protocols of TXXXX series" document.  We use
      // it to simply detect whether the device is configured for
      // Celsius or Fahrenheit data and compensate internally.

      REG_UNIT_SETTINGS                     = 0x203F,

      // firmware revision, BCD byte encoded. We only care about the
      // LO word - the HI word just contains the manufacturing date.
      REG_FW_HI                             = 0x3000,
      REG_FW_LO                             = 0x3001
    } REGS_T;


    /**
     * T3311 constructor
     *
     * @param device Path to the serial device
     * @param address The MODBUS slave address
     * @param baud The baudrate of the device.  Default: 9600
     * @param bits The number of bits per byte.  Default: 8
     * @param parity The parity of the connection, 'N' for None, 'E'
     * for Even, 'O' for Odd.  Default: 'N'
     * @param stopBits The number of stop bits.  Default: 2
     */
    T3311(std::string device, int address, int baud=9600, int bits=8,
          char parity='N', int stopBits=2);

    /**
     * T3311 Destructor
     */
    ~T3311();

    /**
     * Indicate whether the attached sensor supports extended Computed
     * Data registers.  Firmware versions at, or higher than 2.44
     * provide this data.
     *
     * @return true if Extended Data is available, false otherwise
     */
    bool extendedDataAvailable()
    {
      return m_isExtendedDataAvailable;
    };

    /**
     * Read current values from the sensor and update internal stored
     * values.  This method must be called prior to querying any
     * values, such as temperature or humidity.
     */
    void update();

    /**
     * Get the current temperature.  update() must have been called
     * prior to calling this method.
     *
     * @param fahrenheit true to return the temperature in degrees
     * fahrenheit, false to return the temperature in degrees celsius.
     * The default is false (degrees Celsius).
     * @return The last temperature reading in Celsius or Fahrenheit
     */
    float getTemperature(bool fahrenheit=false);

    /**
     * Get the current relative humidity.  update() must have been called
     * prior to calling this method.
     *
     * @return The last humidity reading
     */
    float getHumidity();

    /**
     * Get the current computed value.  update() must have been
     * called prior to calling this method.  This value is configured
     * via the configuration of the sensor using the sensors
     * configuration utility, and can represent several different
     * computed values.  The default value from the factory is the
     * current Dew Point Temperature.
     *
     * Since the actual value configured is unknown (and unknowable)
     * to this driver, the units represented depend on how you have
     * configured the device. This function simply returns the value
     * without any conversion or interpretation, other than the
     * default scaling.
     *
     * @return The last Computed Value
     */
    float getComputedValue();

    /**
     * Get the current dew point temperature.  update() must have been
     * called prior to calling this method.  This information is only
     * available if the sensor supports Extended Data (ie:
     * extendedDataAvailable() returns true).
     *
     * @param fahrenheit true to return the temperature in degrees
     * fahrenheit, false to return the temperature in degrees celsius.
     * The default is false (degrees Celsius).
     * @return The last dew point temperature reading in Celsius or
     * Fahrenheit
     */
    float getDewPointTemperature(bool fahrenheit=false);

    /**
     * Get the current absolute humidity.  update() must have been
     * called prior to calling this method.  This information is only
     * available if the sensor supports Extended Data (ie:
     * extendedDataAvailable() returns true).
     *
     * @return The last absolute humidity reading in g/m3 (grams per
     * cubic meter).
     */
    float getAbsoluteHumidity();

    /**
     * Get the current specific humidity.  update() must have been
     * called prior to calling this method.  This information is only
     * available if the sensor supports Extended Data (ie:
     * extendedDataAvailable() returns true).
     *
     * @return The last specific humidity reading in g/kg (grams per
     * kilogram).
     */
    float getSpecificHumidity();

    /**
     * Get the current mixing ratio.  update() must have been
     * called prior to calling this method.  This information is only
     * available if the sensor supports Extended Data (ie:
     * extendedDataAvailable() returns true).
     *
     * @return The last mixing ratio reading in g/kg (grams per
     * kilogram).
     */
    float getMixingRatio();

    /**
     * Get the current specific enthalopy, a measure of energy
     * density.  update() must have been called prior to calling this
     * method.  This information is only available if the sensor
     * supports Extended Data (ie: extendedDataAvailable() returns
     * true).
     *
     * @return The last specific enthalopy reading in kJ/kg
     * (kilojoules per kilogram).
     */
    float getSpecificEnthalpy();

    /**
     * Get the serial number of the sensor.
     *
     * @return The serial number
     */
    std::string getSerialNumber()
    {
      return m_serialNumber;
    };

    /**
     * Get the firmware revision major number.
     *
     * @return The firmware revsion major number.
     */
    int getFirmwareMajor()
    {
      return m_fwRevHi;
    };

    /**
     * Get the firmware revision minor number.
     *
     * @return The firmware revsion minor number.
     */
    int getFirmwareMinor()
    {
      return m_fwRevLo;
    };

    /**
     * Enable or disable debugging output.  This primarily enables and
     * disables libmodbus debugging output.
     *
     * @param enable true to enable debugging, false otherwise
     */
    void setDebug(bool enable);

  protected:
    uint16_t readInputReg(int reg);
    int readInputRegs(int reg, int len, uint16_t *buf);

    // MODBUS context
    modbus_t *m_mbContext;

    // is the device reporting in C or F?
    bool m_isCelsius;

    // Is the device FW > than 2.44?
    bool m_isExtendedDataAvailable;

    int m_fwRevHi;
    int m_fwRevLo;

    std::string m_serialNumber;

  private:
    bool m_debugging;

    // data
    float m_temperature;
    float m_humidity; // relative
    float m_computedValue;

    // with FW versions > 2.44, these other computed values are
    // available.
    float m_dewPointTemperature;
    float m_absoluteHumidity;
    float m_specificHumidity;
    float m_mixingRatio;
    float m_specificEnthalpy;
  };
}
