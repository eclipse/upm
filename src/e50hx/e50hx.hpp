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

namespace upm {

  /**
   * @brief E50HX Energy Meter
   * @defgroup e50hx libupm-e50hx
   * @ingroup uart electric
   */

  /**
   * @library e50hx
   * @sensor e50hx
   * @comname UPM API for the Veris E50HX Energy Meters
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

  class E50HX {
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

    // Since none of the legal values returned by getAnalogValue() or
    // getAnalogInput() will ever be negative, we use these two values
    // to indicate either an error (BACnet or UPM), or to indicate
    // that the value is unreliable if checkReliability() has been
    // enabled.
    const float RETURN_ERROR = -1.0;
    const float RETURN_UNRELIABLE = -2.0;

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
     * This function initializes the underlying BACNETMSTP Master
     * singleton in the event it has not already been initialized.  If
     * the BACNETMSTP Master singleton has already been initialized,
     * then this call will be ignored.
     *
     * @param port The serial port that the RS-485 interface is
     * connected to.
     * @param baudRate The baudrate of the RS-485 interface.  All
     * devices on a BACnet RS-485 bus must run at the same baudrate.
     * Valid values are 9600, 19200, 38400, 57600, 76800, and 115200.
     * @param deviceInstanceNumber This is the unique Device Object
     * Instance number that will be used for our BACnet Master in
     * order to communicate over the BACnet interface.  This number
     * must be between 1-4194302 and must be unique on the BACnet
     * network.
     * @param macAddr This is the MAC address of our BACnet Master.
     * It must be unique on the BACnet segment, and must be between
     * 1-127.
     * @param maxMaster This specifies to our Master the maximum MAC
     * address used by any other Masters on the BACnet network.  This
     * must be between 1-127, the default is 127.  Do not change this
     * unless you know what you are doing or you could introduce
     * token passing errors on the BACnet network.
     * @param maxInfoFrames This number specifies the maximum number
     * of transmissions (like requests for data) our Master is allowed
     * to make before passing the token to the next Master.  The
     * default is 1.
     */
    void initMaster(std::string port, int baudRate, int deviceInstanceNumber,
                    int macAddr, int maxMaster=DEFAULT_MAX_MASTER,
                    int maxInfoFrames=1);

    /**
     * Enable some debugging output in this module as well as the
     * BACNETMSTP module.  Debugging is disabled by default.
     *
     * @param enable true to enable, false to disable.
     */
    void setDebug(bool enable);

    /**
     * Retrieve the Present_Value property of an Analog Value object.
     * If checkReliability() has been enabled, then the Reliability
     * property of the object will be retrieved first.  If the
     * Reliability property is anything other than
     * RELIABILITY_NO_FAULT_DETECTED, then the RETURN_UNRELIABLE value
     * will be returned.  Reliability checking is disabled by default
     * for performance reasons.
     *
     * @param objInstance One of the ANALOG_VALUES_T values.
     * @return the floating point value requested
     */
    float getAnalogValue(ANALOG_VALUES_T objInstance);

    /**
     * Retrieve the Present_Value property of an Analog Input object.
     * If checkReliability() has been enabled, then the Reliability
     * property of the object will be retrieved first.  If the
     * Reliability property is anything other than
     * RELIABILITY_NO_FAULT_DETECTED, then the RETURN_UNRELIABLE value
     * will be returned.  Reliability checking is disabled by default
     * for performance reasons.
     *
     * @param objInstance One of the ANALOG_INPUTS_T values.
     * @return the floating point value requested
     */
    float getAnalogInput(ANALOG_INPUTS_T objInstance);

    /**
     * Write one of several 'magic' numbers to the configuration
     * object (AV1).  This is used to clear certain counters, reset
     * the accumulated Energy consumption values, etc.
     *
     * @param config One of the CFG_VALUES_T values
     * @return true if the operation suceeded, false if there was an
     * error.
     */
    bool writeConfig(CFG_VALUES_T config);

    /**
     * Set the System Type of the device.  This defines the voltage
     * lines you have connected.
     *
     * @param systype One of the SYSTEM_TYPES_T values.
     * @return true if the operation suceeded, false if there was an
     * error.
     */
    bool writeSystemType(SYSTEM_TYPES_T systype);

    /**
     * Set the Primary CT ratio.  See the datasheet for details.
     *
     * @param ctRatio A floating point value between 5-32000
     * @return true if the operation suceeded, false if there was an
     * error.
     */
    bool writeCTRatioPrimary(float ctRatio);

    /**
     * Set the Secondary CT ratio.  See the datasheet for details.
     *
     * @param ctRatio One of the CT_SECONDARY_T values.
     * @return true if the operation suceeded, false if there was an
     * error.
     */
    bool writeCTRatioSecondary(CT_SECONDARY_T ctRatio);

    /**
     * Set the PT ratio.  See the datasheet for details.
     *
     * @param ptRatio A floating point value between 0.01-320.0
     * @return true if the operation suceeded, false if there was an
     * error.
     */
    bool writePTRatio(float ptRatio);

    /**
     * Set the System Voltage parmeter.  See the datasheet for details.
     *
     * @param sysVolts A floating point value between 82.0-32000.0
     * @return true if the operation suceeded, false if there was an
     * error.
     */
    bool writeSystemVoltage(float sysVolts);

    /**
     * Set the LCD Display Units in IEC or IEEE format.
     *
     * @param dispUnits One of the DISP_UNITS_T values.
     * @return true if the operation suceeded, false if there was an
     * error.
     */
    bool writeDisplayUnits(DISP_UNITS_T dispUnits);

    /**
     * Set the Phase Loss Voltage Threshold.  See the datasheet for
     * details.
     *
     * @param dispUnits A floating point value between 1.0-99.0
     * @return true if the operation suceeded, false if there was an
     * error.
     */
    bool writePhaseLossVT(float phaseLoss);

    /**
     * Set the Phase Loss Imbalance Threshold.  See the datasheet for
     * details.
     *
     * @param dispUnits A floating point value between 1.0-99.0
     * @return true if the operation suceeded, false if there was an
     * error.
     */
    bool writePhaseLossIT(float phaseLoss);

    /**
     * Query an Analog Value object for the unit code, translate it
     * into a string and cache the result for future use.  Return the
     * BACnet text for the Unit enumeration.  Unit enumerations are
     * things like 'kilowatt-hours', 'volts', etc.  For Objects which
     * do not have a Units property defined for them, or for which
     * Units has no meaning, 'no-units' will typically be returned and
     * cached.
     *
     * @param objInstance One of the ANALOG_VALUES_T values.
     * @return A string representing the Object's Unit property.
     */
    std::string getAnalogValueUnits(ANALOG_VALUES_T objInstance);

    /**
     * Query an Analog Input object for the unit code, translate it
     * into a string and cache the result for future use.  Return the
     * BACnet text for the Unit enumeration.  Unit enumerations are
     * things like 'kilowatt-hours', 'volts', etc.  For Objects which
     * do not have a Units property defined for them, or for which
     * Units has no meaning, 'no-units' will typically be returned and
     * cached.
     *
     * @param objInstance One of the ANALOG_INPUTS_T values.
     * @return A string representing the Object's Unit property.
     */
    std::string getAnalogInputUnits(ANALOG_INPUTS_T objInstance);

    /**
     * Query the AI52 Object and return a bitmask of current Alarms.
     * Compare against ALARM_BITS_T to determine what conditions are
     * signaling an alarm.  Alarm conditions will clear on their own
     * as soon as the cause is rectified.
     *
     * @return A bitmask of values from ALARM_BITS_T indicating
     * current alarm conditions.
     */
    uint16_t getAlarmBits();

    /**
     * Enable or disable reliability checking.  By default, when using
     * getAnalogValue() or getAnalogInput() the Present_Value property
     * is returned.  There is also a property called Reliability that
     * can be checked to ensure that the Present_Value property is
     * currently valid.
     *
     * Enabling Reliability Checking has these functions check for a
     * RELIABILITY_NO_FAULT_DETECTED value for the Reliability
     * property before querying the Present_Value property.  If
     * anything other than RELIABILITY_NO_FAULT_DETECTED is set, then
     * these functions will return RETURN_UNRELIABLE rather than the
     * Present_Value.
     *
     * This checking is disabled by default since it will double the
     * number of queries needed to retrieve a given value.  However,
     * if you need to ensure that the values returned are always
     * completely valid as determined by the device firmware, you
     * should enable this.
     *
     * @param enable true to check Reliability before returning a
     * value, false otherwise.
     */
    void checkReliability(bool enable)
    {
      m_checkReliability = enable;
    };

    /**
     * Query the Device Object of the device and return it's
     * Description property.  This typically contains information like
     * the Vendor, model and serial number of a device.
     *
     * @return A string containing the Device Object's Description property.
     */
    std::string getDescription();

    /**
     * Query the Device Object of the device and return it's Location
     * property.  This typically contains a string indication a
     * customer specific value.  Use setLocation() to change.
     *
     * @return A string containing the Device Object's Location property.
     */
    std::string getLocation();

    /**
     * Set the Device Object's Location property.  This must be a
     * string containing no more than 63 characters.
     *
     * @return true if the operation succeeded, false otherwise
     */
    bool setLocation(std::string location);

    /**
     * This is a utility function that will return a string reporting
     * on the various types of errors that can occur in BACnet
     * operation.
     *
     * @return A string containing the last error message.
     */
    std::string getAllErrorString();

    /**
     * Return an enumration of the last error type to occur.  The
     * value returned will be one of the BACNETMSTP::BACERR_TYPE_T
     * values.
     *
     * @return The last error type to occur, one of the
     * BACNETMSTP::BACERR_TYPE_T values.
     */
    BACNETMSTP::BACERR_TYPE_T getErrorType();

    /**
     * In the event of a BACnet Reject error, return the error code.
     *
     * @return The Reject error code.
     */
    uint8_t getRejectReason();

    /**
     * In the event of a BACnet Reject error, return the error string.
     *
     * @return The Reject error string.
     */
    std::string getRejectString();

    /**
     * In the event of a BACnet Abort error, return the Abort reason code.
     *
     * @return The Abort reason code.
     */
    uint8_t getAbortReason();

    /**
     * In the event of a BACnet Abort error, return the Abort string.
     *
     * @return The Abort error string.
     */
    std::string getAbortString();

    /**
     * In the event of a general BACnet error, return the BACnet error class.
     *
     * @return One of the BACNET_ERROR_CLASS error class codes
     */
    BACNET_ERROR_CLASS getErrorClass();

    /**
     * In the event of a general BACnet error, return the BACnet error code.
     *
     * @return One of the BACNET_ERROR_CODE error codes
     */
    BACNET_ERROR_CODE getErrorCode();

    /**
     * In the event of a general BACnet error, return the BACnet error
     * string.
     *
     * @return A string representing the BACnet error class and code.
     */
    std::string getErrorString();

    /**
     * In the event of a non-BACnet UPM error, return a string
     * describing the error.
     *
     * @return A string representing the UPM error.
     */
    std::string getUPMErrorString();

  protected:
    // a copy of the BACNETMSTP singleton instance pointer
    BACNETMSTP* m_instance;

    // unique device object ID of e50hx
    uint32_t m_targetDeviceObjectID;

    // are we initialized?
    bool m_initialized;

  private:
    bool m_debugging;

    // whether or not to verify reliability before reading a value.
    bool m_checkReliability;

    // Unit cache for AV
    typedef std::map<ANALOG_VALUES_T, std::string> avCacheMap_t;
    avCacheMap_t m_avUnitCache;

    // Unit cache for AI
    typedef std::map<ANALOG_INPUTS_T, std::string> aiCacheMap_t;
    aiCacheMap_t m_aiUnitCache;
  };
}
