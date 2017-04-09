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
   * @brief HWXPXX Hardware Protocol Humidity and Temperature Sensor
   * @defgroup hwxpxx libupm-hwxpxx
   * @ingroup uart temp
   */

  /**
   * @library hwxpxx
   * @sensor hwxpxx
   * @comname Veris HWXPXX Hardware Protocol Humidity and Temperature Sensor
   * @type temp
   * @man veris
   * @con uart
   * @web http://cpengineerscorp.net/veris-industries-hwxphtx.html
   *
   * @brief UPM API for the Veris HWXPXX Hardware Protocol Humidity and Temperature Sensor
   *
   * This module implements support for the Veris HWXPHTX Hardware
   * Protocol Humidity and Temperature Sensor family.  It uses MODBUS
   * over an RS485 interface.  You must have libmodbus v3.1.2 (or
   * greater) installed to compile and use this driver.
   *
   * This module was developed using libmodbus 3.1.2, and the HWXPHTX.
   * This sensor supports humidity, and optionally, temperature,
   * slider switch, and override switch reporting.  The HWXPHTX used to
   * develop this driver did not include the optional slider or
   * override switches, however support for them is provided.
   *
   * It was developed using an RS232->RS485 inteface.  You cannot use
   * the built in MCU TTL UART pins for accessing this device -- you
   * must use a full serial RS232->RS485 interface connected via USB.
   *
   * @snippet hwxpxx.cxx Interesting
   */

  class HWXPXX {
  public:
    // MODBUS input registers
    typedef enum {
      INPUT_HUMIDITY                        = 0x0000,
      // optional temp sensor
      INPUT_TEMPERATURE                     = 0x0001,
      // optional slider input
      INPUT_SLIDER                          = 0x0002
    } INPUT_REGS_T;

    // MODBUS coil registers
    typedef enum {
      // device reports in C or F?
      COIL_TEMP_SCALE                       = 0x0000,

      // optional override button status
      COIL_OVERRIDE                         = 0x0001
    } COIL_REGS_T;

    // MODBUS holding registers
    typedef enum {
      HOLDING_TEMP_OFFSET                   = 0x0000,
      HOLDING_HUM_OFFSET                    = 0x0001
    } HOLDING_REGS_T;

    /**
     * HWXPXX constructor
     *
     * @param device Path to the serial device
     * @param address The MODBUS slave address
     * @param baud The baudrate of the device.  Default: 19200
     * @param bits The number of bits per byte.  Default: 8
     * @param parity The parity of the connection, 'N' for None, 'E'
     * for Even, 'O' for Odd.  Default: 'N'
     * @param stopBits The number of stop bits.  Default: 2
     */
    HWXPXX(std::string device, int address, int baud=19200, int bits=8,
          char parity='N', int stopBits=2);

    /**
     * HWXPXX Destructor
     */
    ~HWXPXX();

    /**
     * Read current values from the sensor and update internal stored
     * values.  This method must be called prior to querying any
     * values, such as temperature, humidity, override switch status,
     * or slider switch status.
     */
    void update();

    /**
     * Get the current temperature.  update() must have been called
     * prior to calling this method.  If this option was not
     * installed, this method will always return 0C/0F, depending on
     * the scale the device is operating in natively.
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
     * Get the current slider switch position.  update() must have
     * been called prior to calling this method.  This returns a value
     * between 0-100 corresponding to the position of the slider
     * switch.  If this option is not installed, this method will
     * always return 0.
     *
     * @return The last slider switch reading
     */
    int getSlider();

    /**
     * Get the current override switch status.  update() must have
     * been called prior to calling this method.  This returns true if
     * the override switch was pressed.  Use clearOverrideSwitch() to
     * reset this value to false.  If this option is not installed,
     * then this method will always return false.  It is not possible
     * to programatically set this value to true - that can only be
     * done by physically pressing the override switch.
     *
     * @return The last overide switch status reading
     */
    bool getOverrideSwitchStatus();

    /**
     * Clear the override switch status (set it to false).  If this
     * option is not installed, then this method will have no effect
     * (the overide switch status will always be false).
     *
     */
    void clearOverrideSwitch();

    /**
     * Return the current temperature offset stored on the device.
     * This is a value between -50 and +50, specified in tenths of a
     * degree in whatever scale (Celsius or Fahrenheit) is in use.
     * This offset is applied to the returned temperature reading by the
     * device.
     *
     * @return The current temperature offset in tenths of a degree
     */
    int getTemperatureOffset();

    /**
     * Return the current humidity offset stored on the device.  This
     * is a value between -100 and +100, specified in tenths of a
     * percent.  This offset is applied to the returned humidity
     * reading by the device.
     *
     * @return The current humidity offset in tenths of a percent
     */
    int getHumidityOffset();

    /**
     * Set the stored temperature offset on the device.  This is a
     * value between -50 and +50, specified in tenths of a degree in
     * what ever scale (Celsius or Fahrenheit) is in use.  This offset
     * is applied to the returned temperature reading by the device.
     *
     * @param offset Offset in tenths of a degree with a range of -50 to +50
     */
    void setTemperatureOffset(int offset);

    /**
     * Set the stored humidity offset on the device.  This is a value
     * between -100 and +100, specified in tenths of a percent.  This
     * offset is applied to the returned humidity reading by the
     * device.
     *
     * @param offset Offset in tenths of a percent with a range of -100 to +100
     */
    void setHumidityOffset(int offset);

    /**
     * Set the temperature scale used by the device.  This driver
     * detects this setting automatically and adjusts itself
     * accordingly, so this is generally never needed.  It is used to
     * set the native reporting scale of the temperature either in
     * degrees Celsius or Fahrenheit.  Its setting will not affect
     * the operation of getTemperature().
     *
     * @param fahrenheit true to set Fahrenheit, false to set Celsius
     */
    void setTemperatureScale(bool fahrenheit);

    /**
     * Return a string corresponding the the device's MODBUS slave ID.
     * This includes information such as the manufacturer, device
     * model number and serial number.
     *
     * @return string represnting the MODBUS slave ID
     */
    std::string getSlaveID();

    /**
     * Set a new MODBUS slave address.  This is useful if you have
     * multiple HWXPXX devices on a single bus.  When this method is
     * called, the current temperature scale is re-read so that
     * further update() calls can work correctly.
     *
     * @param addr The new slave address to set
     */
    void setSlaveAddress(int addr);

    /**
     * Enable or disable debugging output.  This primarily enables and
     * disables libmodbus debugging output.
     *
     * @param enable true to enable debugging, false otherwise
     */
    void setDebug(bool enable);

  protected:
    // input registers
    int readInputRegs(INPUT_REGS_T reg, int len, uint16_t *buf);
    uint16_t readInputReg(INPUT_REGS_T reg);

    // coils
    int readCoils(COIL_REGS_T reg, int numBits, uint8_t *buf);
    bool readCoil(COIL_REGS_T reg);
    void writeCoil(COIL_REGS_T reg, bool val);

    // holding registers
    int readHoldingRegs(HOLDING_REGS_T reg, int len, uint16_t *buf);
    uint16_t readHoldingReg(HOLDING_REGS_T reg);
    void writeHoldingReg(HOLDING_REGS_T reg, int value);

    // MODBUS context
    modbus_t *m_mbContext;

    // is the device reporting in C or F?
    bool m_isCelsius;

  private:
    bool m_debugging;

    // data
    float m_temperature;
    float m_humidity; // relative
    int m_slider; // optional slider value
    bool m_override; // status of override switch
  };
}
