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
   * @brief Honeywell TB7300 Communicating Fan Coil Thermostat
   * @defgroup tb7300 libupm-tb7300
   * @ingroup uart temp
   */

  /**
   * @library tb7300
   * @sensor tb7300
   * @comname Honeywell TB7300 Communicating Fan Coil
   * Thermostat
   * @type temp
   * @man honeywell
   * @con uart
   * @web https://parts-hvac.com/tb7300c5014b.html
   *
   * @brief Honeywell TB7300 Communicating Fan Coil Thermostat
   *
   * This module implements support for the Honeywell TB7300
   * Communicating Fan Coil Thermostat.  It may also support the
   * TB7200, though only the TB7300 was available for development of
   * this driver.
   *
   * The TB7200 Series PI thermostats are designed for zoning
   * applications, and the TB7300 Series PI thermostats are designed
   * for fan coil control. Both Series are communicating thermostats
   * with models available in BACnet(r) MS/TP protocol and can be easily
   * integrated into a WEBs-AX building automation system based on the
   * NiagaraAX(r) platform.
   *
   * TB7200 and TB7300 Series thermostats are compatible with the
   * Honeywell Occupancy Sensor Cover. Thermostats equipped with an
   * occupancy sensor cover provide advanced active occupancy logic,
   * which will automatically switch occupancy levels from Occupied to
   * Stand-By and Unoccupied as required by local activity being
   * present or not. This advanced occupancy functionality provides
   * advantageous energy savings during occupied hours without
   * sacrificing occupant comfort. All thermostats can be ordered with
   * or without a factory installed PIR cover.
   *
   * If you intend to work with this device and driver, it is
   * strongly suggested you get the BACnet Intergration Guide document
   * for this device: 63-4524.pdf as recommended in the device
   * documentation.
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
   * @snippet tb7300.cxx Interesting
   */

  class TB7300 : public BACNETUTIL {
  public:

    // Supported Analog Value Objects.  These are readable and writable.
    typedef enum : uint32_t {
      AV_Room_Temperature                     = 7,

      // GRP 20 Control Output
      AV_PI_Heating_Demand                    = 21,
      AV_PI_Cooling_Demand                    = 22,

      // GRP 38 Temperature Setpoints
      AV_Occupied_Heat_Setpoint               = 39,
      AV_Occupied_Cool_Setpoint               = 40,
      AV_Standby_Heat_Setpoint                = 41,
      AV_Standby_Cool_Setpoint                = 42,
      AV_Unoccupied_Heat_Setpoint             = 43,
      AV_Unoccupied_Cool_Setpoint             = 44,

      // GRP 55 General Options 2
      AV_Password_Value                       = 56,
      AV_Heating_Setpoint_Limit               = 58,
      AV_Cooling_Setpoint_Limit               = 59,
      AV_Deadband                             = 63,
      AV_Standby_Time                         = 67,
      AV_Unoccupied_Time                      = 68
    } ANALOG_VALUES_T;

    // Supported Analog Input Objects.  These are read only.
    typedef enum : uint32_t {
      AI_Supply_Temperature                   = 12
    } ANALOG_INPUTS_T;

    // Supported Binary Value Objects.  These are readable and writable.
    typedef enum : uint32_t {
      BV_Temp_Override                        = 8,
      BV_Aux_Command                          = 14,

      // GRP 45 General Options 1
      BV_Menu_Scroll                          = 49,
      BV_Auto_Mode_Enable                     = 50,
      BV_Temperature_Scale                    = 51,

      // GRP 55 General Option 2
      BV_Setpoint_Type                        = 60,
      BV_Setpoint_Function                    = 61,
      BV_Reheat_Timebase                      = 64,
      BV_Auto_Fan                             = 66,

      // GRP 74 Output Configuration Options
      BV_Control_type                         = 75,
      BV_Direct_Reverse_Acting                = 78
    } BINARY_VALUES_T;

    // Supported Binary Input Objects.  These are read only.
    typedef enum : uint32_t {
      // GRP 24 Controller Status
      BI_AUX_Status                           = 25,
      BI_BI1_Status                           = 29,
      BI_BI2_Status                           = 30,
      BI_UI3_Status                           = 31,
      BI_Local_Motion                         = 32,

      // GRP 34 Controller Alarms
      BI_Window_Alarm                         = 35,
      BI_Filter_Alarm                         = 36,
      BI_Service_Alarm                        = 37
    } BINARY_INPUTS_T;

    // Supported Multi-State Value Objects.  These are readable and
    // writable.
    typedef enum : uint32_t {
      MV_Sequence_of_Operation                = 15,
      MV_System_Mode                          = 16,
      MV_Fan_Mode                             = 17,
      MV_Occupancy_Command                    = 18,
      MV_Keypad_Lockout                       = 19,

      // GRP 24 Controller Status
      MV_Heating_Valve_Status                 = 26,
      MV_Cooling_Valve_Status                 = 27,
      MV_Fan_Status                           = 28,
      MV_Effective_Occupancy                  = 33,

      // GRP 45 General Options 1
      MV_BI1_Configuration                    = 46,
      MV_BI2_Configuration                    = 47,
      MV_UI1_Configuration                    = 48,
      MV_Pipe_Number                          = 52,
      MV_Out1_Config                          = 53,
      MV_AUX_Configuration                    = 54,

      // GRP 55 General Option 2
      MV_Fan_Mode_Sequence                    = 58,
      MV_Temporary_Occupancy_Time             = 62,
      MV_Proportional_Band                    = 65,

      // GRP 74 Output Configuration Options
      MV_Floating_Motor_Timing                = 76,
      MV_On_Off_Control_CPH                   = 77
    } MULTISTATE_VALUES_T;

    /**
     * TB7300 constructor
     *
     * @param targetDeviceObjectID the unique Instance ID of the
     * Device Object.  This number is used to uniquely identify
     * devices on the BACnet network, and ranges from 1 to 4194302.
     * This is not the device's MAC address, though on some devices,
     * the MAC address may be used as part of this number.  On the
     * TB7300, this number depends on the model and the MAC address.
     */
    TB7300(uint32_t targetDeviceObjectID);

    /**
     * TB7300 Destructor
     */
    ~TB7300();

    /**
     * Read current temperature from the sensor and update internal
     * stored value.  This method must be called prior to querying the
     * temperature.  All other values in the device must be queried
     * directly via the appropriate BACNETUTIL::get*() methods
     * depending on the object of interest.
     */
    void update();

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

  protected:
    // always stored in C
    float m_temperature;

    bool m_isTempInitialized;
    bool m_isCelsius;

  private:
  };
}
