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
#include <map>

#include "bacnetmstp.hpp"
#include "bacnetutil.hpp"

namespace upm {

  /**
   * @brief Amphenol Telaire Ventostat T8100 Ventilation Controller
   * @defgroup t8100 libupm-t8100
   * @ingroup uart temp gaseous
   */

  /**
   * @library t8100
   * @sensor t8100
   * @comname Amphenol Telaire Ventostat T8100
   * Ventilation Controller
   * @type gaseous temp
   * @man amphenol
   * @con uart
   * @web https://www.instrumart.com/products/configure/18180?quantity=1
   *
   * @brief UPM API for the Amphenol Telaire Ventostat T8100
   * Ventilation Controller
   *
   * This module implements support for the Amphenol Telaire Ventostat
   * T8100 Ventilation Controller with BACnet interface.  It may also
   * support the T8200 and T8300 models, but they have not been
   * tested.
   *
   * The Telaire Ventostat T8100 reports Temperature, Humidity and CO2
   * concentrations.  It supports an optional relay with a settable
   * trigger point.  The unit this driver was tested under did not
   * support the optional relay.  The temperature range supported is
   * 0-50C, humidity is 0-100% non-condensing, and CO2 range is
   * appoximately 0-2000 PPM for the T8100.  Other sensors in this
   * family support wider ranges.
   *
   * This module was developed using the upm::BACNETMSTP library,
   * based on libbacnet-stack 0.8.3.  Both libbacnet 0.8.3 and the
   * upm::BACNETMSTP libraries must be present in order to build this
   * module.
   *
   * It was connected using an RS232->RS485 interface.  You cannot use
   * the built in MCU TTL UART pins for accessing this device -- you
   * must use a full Serial RS232->RS485 or USB-RS485 interface
   * connected via USB.
   *
   * @snippet t8100.cxx Interesting
   */

  class T8100 : public BACNETUTIL {
  public:

    // Supported Analog Value Objects.  These are readable and writable.
    typedef enum : uint32_t {
      AV_Temperature_Offset                   = 1,
      AV_RH_Offset                            = 2,
      AV_Relay_Set_Point                      = 3,
      AV_Relay_Hysteresis                     = 4,
      AV_Elevation                            = 5,
      AV_Calibration_Single_Point             = 6,
      AV_Baud_Rate                            = 7,
      AV_MAC_Address                          = 8
    } ANALOG_VALUES_T;

    // Supported Analog Input Objects.  These are read only.
    typedef enum : uint32_t {
      AI_CO2                                  = 1,
      AI_Relative_Humidity                    = 2,
      AI_Temperature_ChipCap                  = 3,
      AI_Temperature_Thermistor               = 4
    } ANALOG_INPUTS_T;

    // Supported Binary Value Objects.  These are readable and writable.
    typedef enum : uint32_t {
      BV_Temperature_Units                    = 1,
      BV_ABC_Logic_State                      = 2,
      BV_ABC_Logic_Reset                      = 3,
      BV_CO2_Calibration                      = 4
    } BINARY_VALUES_T;

    // Supported Binary Input Objects.  These are read only.
    typedef enum : uint32_t {
      BI_Relay_State                          = 1
    } BINARY_INPUTS_T;


    /**
     * T8100 constructor
     *
     * @param targetDeviceObjectID the unique Instance ID of the
     * Device Object.  This number is used to uniquely identify
     * devices on the BACnet network, and ranges from 1 to 4194302.
     * This is not the device's MAC address, though on some devices,
     * the MAC address may be used as part of this number.  On the
     * T8100, this number is 568XXX, where XXX are the 3 digits of the
     * set MAC address.  The MAC address is configured via DIP switches
     * within the device.
     */
    T8100(uint32_t targetDeviceObjectID);

    /**
     * T8100 Destructor
     */
    ~T8100();

    /**
     * Read current values from the sensor and update internal stored
     * values for temperature, humidity, CO2 concentration and relay
     * state.  This method must be called prior to querying any
     * of the aforementioned values.
     */
    void update();

    /**
     * Get the current relative humidity.  update() must have been
     * called prior to calling this method.
     *
     * @return The last humidity reading
     */
    float getHumidity()
    {
      return m_humidity;
    }

    /**
     * Get the current CO2 concentration in Parts per Million (PPM).
     * update() must have been called prior to calling this method.
     *
     * @return The last CO2 reading
     */
    float getCO2()
    {
      return m_co2;
    }

    /**
     * Get the current temperature.  update() must have been called
     * prior to calling this method.
     *
     * @param fahrenheit true to return the temperature in degrees
     * fahrenheit, false to return the temperature in degrees celsius.
     * The default is false (degrees Celsius).
     * @return The last temperature reading in Celsius or Fahrenheit.
     */
    float getTemperature(bool fahrenheit=false);

    /**
     * Return the current state of the relay.  This function will
     * always return false if the relay option is not installed.
     * update() must have been called prior to calling this method.
     *
     * @return true if the relay is active, false if inactive.
     */
    bool getRelayState()
    {
      return m_relayState;
    }

    /**
     * Set the device temperature scale to Celsius of Fahrenheit.  For
     * devices with an LCD display, this will affect which scale is
     * displayed.  When changing the scale, it may take several
     * seconds for the setting to take effect.
     *
     * @param fahrenheit true to set the scale to fahrenheit, false
     * for celsius.
     */
    void setTemperatureScale(bool fahrenheit);

    /**
     * Get the device temperature scale.
     *
     * @return true if scale is fahrenheit, false for celsius.
     */
    bool getTemperatureScale();

    /**
     * Get the current temperature offset.
     *
     * @return The configured temperature offset.
     */
    float getTemperatureOffset();

    /**
     * Set the device temperature offset.  The offset is applied by
     * the device internally to the temperature reading.  The offset
     * must always be specified in degrees Celsius.  Valid values must
     * be between -50 and 50.
     *
     * @param value The temperature offset to configure.
     */
    void setTemperatureOffset(float value);

    /**
     * Get the current humidity offset.
     *
     * @return The configured humidity offset.
     */
    float getHumidityOffset();

    /**
     * Set the device humidity offset.  The offset is applied by the
     * device internally to the humidity reading.  Valid values must
     * be between -100 and 100.
     *
     * @param value The humidity offset to configure.
     */
    void setHumidityOffset(float value);

    /**
     * Return the current relay set point (in PPM).  This set point is
     * the CO2 concentration point in PPM that causes the relay to
     * trigger.
     *
     * @return The relay set point value.
     */
    float getRelaySetPoint();

    /**
     * Set the relay set point in PPM.  This set point is the CO2
     * concentration point in PPM that causes the relay to trigger.
     * Valid values are between 0-65535.
     *
     * @param value The desired relay set point value.
     */
    void setRelaySetPoint(float value);

    /**
     * Return the current relay hysteresis.
     *
     * @return The relay hysteresis value.
     */
    float getRelayHysteresis();

    /**
     * Set the relay hysteresis.  Valid values are between 0-65535.
     *
     * @param value The desired relay set point value.
     */
    void setRelayHysteresis(float value);

    /**
     * Return the current elevation setting (in meters).
     *
     * @return The current elevation setting.
     */
    float getElevation();

    /**
     * Set the elevation setting in meters.  Valid values are between
     * 0-65535.
     *
     * @param value The desired elevation setting in meters.
     */
    void setElevation(float value);

    /**
     * Return the current calibration single point value (in PPM).
     *
     * @return The current calibration single point value.
     */
    float getCalibrationSinglePoint();

    /**
     * Set the calibration single point value in PPM.  Valid values
     * are between 0-65535.
     *
     * @param value The desired calibration single point value in PPM.
     */
    void setCalibrationSinglePoint(float value);

    /**
     * Return the current baud rate.
     *
     * @return The current baud rate.
     */
    float getBaudRate();

    /**
     * Return the current MAC address.  The MAC address is configured
     * via DIP switches within the device.
     *
     * @return The current MAC address.
     */
    float getMACAddress();

    /**
     * Return the current ABC (Automatic Background Calibration)
     * logic state.  See the datasheet for details.
     *
     * @return The current ABC logic state.
     */
    bool getABCLogicState();

    /**
     * Set the ABC (Automatic Background Calibration) logic state.
     * Valid values are true for ON, false for OFF.
     *
     * @param value The desired ABC logic state.
     */
    void setABCLogicState(bool value);

    /**
     * Return the current ABC (Automatic Background Calibration)
     * reset state.  See the datasheet for details.
     *
     * @return The current ABC reset state.
     */
    bool getABCLogicReset();

    /**
     * Set the ABC (Automatic Background Calibration) reset state.
     * Valid values are true for Reset, false for Normal.
     *
     * @param value The desired ABC reset state.
     */
    void setABCLogicReset(bool value);

    /**
     * Return the current CO2 calibration state.  See the datasheet
     * for details.
     *
     * @return The current CO2 calibration state.
     */
    bool getCO2Calibration();

    /**
     * Set the CO2 calibration state.
     * Valid values are true for Calibrate, false for Normal.
     *
     * @param value The desired ABC reset state.
     */
    void setCO2Calibration(bool value);


  protected:
    float m_humidity;
    // always stored in C
    float m_temperature;
    float m_co2;
    bool m_relayState;

  private:
    // Have we checked the device's temperature unit setting yet
    bool m_isTempInitialized;

    // Is the device configured for Celsius?
    bool m_isCelsius;
  };
}
