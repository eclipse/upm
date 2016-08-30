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
   * @brief E50HX Energy Meter
   * @defgroup e50hx libupm-e50hx
   * @ingroup uart electric
   */

  /**
   * @library e50hx
   * @sensor e50hx
   * @comname Veris E50HX Energy Meters
   * @type electic
   * @man veris
   * @con uart
   * @web http://www.veris.com/Item/E50H5.aspx
   *
   * @brief UPM API for the Veris E50HX Energy Meter
   *
   * This module implements support for the Veris E50H2 and E50H5
   * BACnet Energy Meters.
   *
   * From the datasheet: The E50H5 BACnet MS/TP DIN Rail Meter with
   * Data Logging combines exceptional performance and easy
   * installation to deliver a cost-effective solution for power
   * monitoring applications. Native serial communication via BACnet
   * MS/TP provides complete accessibility of all measurements to your
   * Building Automation System The data logging capability protects
   * data in the event of a power failure. The E50H5 can be easily
   * installed on standard DIN rail, surface mounted or contained in
   * an optional NEMA 4 enclosure, as needed. The front-panel LCD
   * display makes device installation and setup easy and provides
   * local access to the full set of detailed measurements.
   *
   * This module was developed using the upm::BACNETMSTP module, based
   * on libbacnet-stack 0.8.3.  Both libbacnet 0.8.3 and the
   * upm::BACNETMSTP libraries must be present in order to build this
   * module.  This driver was developed on the E50H5.  The Trend Log
   * functionality is not currently supported.
   *
   * The Binary Input Objects are also not supported as these are only
   * used for the Alarm bits which are already available from Analog
   * Input Object 52 as an alarm bitfield incorporating all of the
   * supported alarm indicators.
   *
   * It was connected using an RS232->RS485 interface.  You cannot use
   * the built in MCU TTL UART pins for accessing this device -- you
   * must use a full Serial RS232->RS485 or USB-RS485 interface
   * connected via USB.
   *
   * @snippet e50hx.cxx Interesting
   */

  class E50HX : public BACNETUTIL {
  public:

    // Supported Analog Value Objects.  These are readable and writable.
    typedef enum : uint32_t {
      AV_Config                               = 1, // always returns 0 on read
      AV_System_Type                          = 2,
      AV_CT_Ratio_Primary                     = 3,
      AV_CT_Ratio_Secondary                   = 4,
      AV_PT_Ratio                             = 5,
      AV_System_Voltage                       = 6,
      AV_Display_Units                        = 7,
      AV_Phase_Loss_Voltage_Threshold         = 8,
      AV_Phase_Loss_Imbalance_Threshold       = 9,
      AV_Subintervals                         = 10,
      AV_Subinterval_Length                   = 11
    } ANALOG_VALUES_T;

    // Supported Analog Input Objects.  These are read only.
    typedef enum : uint32_t {
      AI_Energy                               = 1,
      AI_kW_Total                             = 2,
      AI_kVAR_Total                           = 3,
      AI_kVA_Total                            = 4,
      AI_PF_Total                             = 5,
      AI_Volts_LL_Avg                         = 6,
      AI_Volts_LN_Avg                         = 7,
      AI_Current_Avg                          = 8,
      AI_kW_A                                 = 9,
      AI_kW_B                                 = 10,
      AI_kW_C                                 = 11,
      AI_PF_A                                 = 12,
      AI_PF_B                                 = 13,
      AI_PF_C                                 = 14,
      AI_Volts_AB                             = 15,
      AI_Volts_BC                             = 16,
      AI_Volts_AC                             = 17,
      AI_Volts_AN                             = 18,
      AI_Volts_BN                             = 19,
      AI_Volts_CN                             = 20,
      AI_Current_A                            = 21,
      AI_Current_B                            = 22,
      AI_Current_C                            = 23,
      // AI24 is reserved
      AI_Frequency                            = 25,
      AI_kVAh                                 = 26, // units = kVAh, not kWH
      AI_kVARh                                = 27, // units = kVAh, not kWH
      AI_kVA_A                                = 28,
      AI_kVA_B                                = 29,
      AI_kVA_C                                = 30,
      AI_kVAR_A                               = 31,
      AI_kVAR_B                               = 32,
      AI_kVAR_C                               = 33,
      AI_KW_Present_Demand                    = 34,
      AI_KVAR_Present_Demand                  = 35,
      AI_KWA_Present_Demand                   = 36,
      AI_KW_Max_Demand                        = 37,
      AI_KVAR_Max_Demand                      = 38,
      AI_KVA_Max_Demand                       = 39,
      AI_Pulse_Count_1                        = 40, // H2 & H5
      // AI41 is reserved on H2 variant
      AI_Pulse_Count_2                        = 41, // only on H5 variant
      AI_KWH_A                                = 42,
      AI_KWH_B                                = 43,
      AI_KWH_C                                = 44,
      AI_Max_Power                            = 45, // theoretical max power
      // AI46 reserved
      AI_Energy_Resets                        = 47,
      // AI48 and AI49 reserved
      AI_Power_Up_Count                       = 50,
      AI_Output_Config                        = 51, // H2 = 11, H5 = 10
      AI_Alarm_Bitmap                         = 52
    } ANALOG_INPUTS_T;

    // Alarm bits (AI52)
    typedef enum : uint16_t {
      ALARM_Volts_Error_A                     = 0x0001,
      ALARM_Volts_Error_B                     = 0x0002,
      ALARM_Volts_Error_C                     = 0x0004,

      ALARM_Current_Error_A                   = 0x0008,
      ALARM_Current_Error_B                   = 0x0010,
      ALARM_Current_Error_C                   = 0x0020,

      ALARM_Frequency_Error                   = 0x0040,

      ALARM_Reserved_0                        = 0x0080, // reserved

      ALARM_Phase_Loss_A                      = 0x0100,
      ALARM_Phase_Loss_B                      = 0x0200,
      ALARM_Phase_Loss_C                      = 0x0400,

      ALARM_Power_Factor_A                    = 0x0800,
      ALARM_Power_Factor_B                    = 0x1000,
      ALARM_Power_Factor_C                    = 0x2000,

      ALARM_RTC_RESET                         = 0x4000 // H5 only
    } ALARM_BITS_T;

    // valid config values to write to AV1
    typedef enum {
      CFG_CLR_ENERGY_ACCUM          = 30078, // clear energy accumulators
      CFG_NEW_DSI                   = 21211, // begin new demand subinterval
      CFG_RESET_MAX_TO_PRESENT      = 21212, // reset max vals to present vals
      CFG_CLEAR_PULSE_COUNTERS      = 16498  // clear the pulse counters
    } CFG_VALUES_T;

    // system type configuration
    typedef enum {
      SYSTYPE_SINGLE_PHASE_AN       = 10,
      SYSTYPE_SINGLE_PHASE_AB       = 11,
      SYSTYPE_SPLIT_PHASE_ABN       = 12,
      SYSTYPE_3PHASE_ABC            = 31,
      SYSTYPE_3PHASE_ABCN           = 40
    } SYSTEM_TYPES_T;

    // CT input ratio
    typedef enum {
      CT_RATIO_SECONDARY_1          = 1, // CT's w/ 1v outputs
      CT_RATIO_SECONDARY_3          = 3  // CT's w/ 0.3v outputs
    } CT_SECONDARY_T;

    // LCD display units
    typedef enum {
      DISP_UNITS_IEC                = 0, // IEC display units
      DISP_UNITS_IEEE               = 1  // IEEE display units
    } DISP_UNITS_T;

    /**
     * E50HX constructor
     *
     * @param targetDeviceObjectID the unique Instance ID of the
     * Device Object.  This number is used to uniquely identify
     * devices on the BACnet network, and ranges from 1 to 4194302.
     * This is not the device's MAC address, though on some devices,
     * the MAC address may be used as part of this number.  On the
     * E50HX, this number is randomly generated per device, and you
     * can see this number (or change it) on the BACnet config screens
     * on the LCD.
     */
    E50HX(uint32_t targetDeviceObjectID);

    /**
     * E50HX Destructor
     */
    ~E50HX();

    /**
     * Write one of several 'magic' numbers to the configuration
     * object (AV1).  This is used to clear certain counters, reset
     * the accumulated Energy consumption values, etc.  This method
     * will throw on error.
     *
     * @param config One of the CFG_VALUES_T values
     */
    void writeConfig(CFG_VALUES_T config);

    /**
     * Set the System Type of the device.  This defines the voltage
     * lines you have connected.  This method will throw on error.
     *
     * @param systype One of the SYSTEM_TYPES_T values.
     */
    void writeSystemType(SYSTEM_TYPES_T systype);

    /**
     * Set the Primary CT ratio.  See the datasheet for details.  This
     * method will throw on error.
     *
     * @param ctRatio A floating point value between 5-32000
     */
    void writeCTRatioPrimary(float ctRatio);

    /**
     * Set the Secondary CT ratio.  See the datasheet for details.
     * This method will throw on error.
     *
     * @param ctRatio One of the CT_SECONDARY_T values.
     */
    void writeCTRatioSecondary(CT_SECONDARY_T ctRatio);

    /**
     * Set the PT ratio.  See the datasheet for details.  This method
     * will throw on error.
     *
     * @param ptRatio A floating point value between 0.01-320.0
     */
    void writePTRatio(float ptRatio);

    /**
     * Set the System Voltage parmeter.  See the datasheet for
     * details.  This method will throw on error.
     *
     * @param sysVolts A floating point value between 82.0-32000.0
     */
    void writeSystemVoltage(float sysVolts);

    /**
     * Set the LCD Display Units in IEC or IEEE format.  This method
     * will throw on error.
     *
     * @param dispUnits One of the DISP_UNITS_T values.
     */
    void writeDisplayUnits(DISP_UNITS_T dispUnits);

    /**
     * Set the Phase Loss Voltage Threshold.  See the datasheet for
     * details.  This method will throw on error.
     *
     * @param dispUnits A floating point value between 1.0-99.0
     */
    void writePhaseLossVT(float phaseLoss);

    /**
     * Set the Phase Loss Imbalance Threshold.  See the datasheet for
     * details.  This method will throw on error.
     *
     * @param dispUnits A floating point value between 1.0-99.0
     */
    void writePhaseLossIT(float phaseLoss);

    /**
     * Query the AI52 Object and return a bitmask of current Alarms.
     * Compare against ALARM_BITS_T to determine what conditions are
     * signaling an alarm.  Alarm conditions will clear on their own
     * as soon as the cause is rectified.  This method will throw on
     * error.
     *
     * @return A bitmask of values from ALARM_BITS_T indicating
     * current alarm conditions.
     */
    uint16_t getAlarmBits();

  protected:
  private:
  };
}
