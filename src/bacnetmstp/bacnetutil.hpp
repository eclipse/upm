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
#include <vector>

#include "bacnetmstp.hpp"

namespace upm {

  /**
   * @library bacnetmstp
   * @comname UPM Utility API for BACnet
   * @con uart
   *
   * @brief UPM Utility API for BACnet
   *
   * This class implements some common access functions that are
   * useful to any driver making use of the bacnetmstp driver.
   *
   * It provides some basic functionality for reading and writing a
   * proprty (with and without relability checking) as well as access
   * to error conditions.  It is intended to be inherited by your
   * driver class.
   */

  class BACNETUTIL {
  public:

    /**
     * BACNETUTIL constructor
     *
     */
    BACNETUTIL(uint32_t targetDeviceObjectID);

    /**
     * BACNETUTIL Destructor
     */
    virtual ~BACNETUTIL();

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
    virtual void initMaster(std::string port, int baudRate,
                            int deviceInstanceNumber,
                            int macAddr, int maxMaster=DEFAULT_MAX_MASTER,
                            int maxInfoFrames=1);

    /**
     * Enable some debugging output in this module as well as the
     * BACNETMSTP module.  Debugging is disabled by default.
     *
     * @param enable true to enable, false to disable.
     */
    virtual void setDebug(bool enable);

    /**
     * Retrieve the Present_Value property of an Analog Value object.
     * If checkReliability() has been enabled, then the Reliability
     * property of the object will be retrieved first.  If the
     * Reliability property is anything other than
     * RELIABILITY_NO_FAULT_DETECTED, then the method will throw.
     * Reliability checking is disabled by default for performance
     * reasons.
     *
     * @param objInstance The Analog Value Object instance.
     * @return The floating point value requested.
     */
    virtual float getAnalogValue(uint32_t objInstance);

    /**
     * Set the Present_Value property of an Analog Value object.  This
     * method will throw on an error.
     *
     * @param objInstance The Analog Value Object instance.
     * @param value The data value to write.
     */
    virtual void setAnalogValue(uint32_t objInstance,
                                float value);

    /**
     * Retrieve the Present_Value property of an Analog Input object.
     * If checkReliability() has been enabled, then the Reliability
     * property of the object will be retrieved first.  If the
     * Reliability property is anything other than
     * RELIABILITY_NO_FAULT_DETECTED, then the method will throw.
     * Reliability checking is disabled by default for performance
     * reasons.
     *
     * @param objInstance The Analog Input Object instance.
     * @return the floating point value requested.
     */
    virtual float getAnalogInput(uint32_t objInstance);

    /**
     * Query an Analog Value object for the unit code, translate it
     * into a string and cache the result for future use.  Return the
     * BACnet text for the Unit enumeration.  Unit enumerations are
     * things like 'kilowatt-hours', 'volts', etc.  For Objects which
     * do not have a Units property defined for them, or for which
     * Units has no meaning, 'no-units' will typically be returned and
     * cached for the object.
     *
     * @param objInstance The Analog Value Object instance.
     * @return A string representing the Object's Unit property.
     */
    virtual std::string getAnalogValueUnits(uint32_t objInstance);

    /**
     * Query an Analog Input object for the unit code, translate it
     * into a string and cache the result for future use.  Return the
     * BACnet text for the Unit enumeration.  Unit enumerations are
     * things like 'kilowatt-hours', 'volts', etc.  For Objects which
     * do not have a Units property defined for them, or for which
     * Units has no meaning, 'no-units' will typically be returned and
     * cached for the object.
     *
     * @param objInstance The Analog Input Object instance.
     * @return A string representing the Object's Unit property.
     */
    virtual std::string getAnalogInputUnits(uint32_t objInstance);

    /**
     * Retrieve the Present_Value property of a Binary Input object.
     * If checkReliability() has been enabled, then the Reliability
     * property of the object will be retrieved first.  If the
     * Reliability property is anything other than
     * RELIABILITY_NO_FAULT_DETECTED, then the method will throw.
     * Reliability checking is disabled by default for performance
     * reasons.
     *
     * @param objInstance The Object Instance number to query
     * @return the boolean point value requested
     */
    virtual bool getBinaryInput(uint32_t objInstance);

    /**
     * Lookup (retrieve if necessary) the Inactive_Text and
     * Active_Text properties of a Binary Input object.  These text
     * properties are optional and can provide a string representing a
     * given state (true/false) that can be more informational than
     * just the boolean value the object represents.  This is useful
     * in applications that display this data to a user for example.
     * If this text is not present in the object (as it is not
     * required), then a string representation of the value will be
     * returned instead ("active" and "inactive").
     *
     * @param objInstance The Object Instance number of the object
     * @param value The value you want to lookup the text
     * representation for.
     * @return The string representing the value.
     */
    virtual std::string lookupBinaryInputText(uint32_t objInstance, bool value);

    /**
     * Return a string representation of the Present_Value property of
     * a BinaryInput object.  This method just calls getBinaryInput()
     * on the object, uses lookupBinaryInputText() to lookup the
     * corresponding text value, and returns the result.
     *
     * @param objInstance The Object Instance number of the object.
     * @return The string representing the value.
     */
    virtual std::string getBinaryInputText(uint32_t objInstance);

    /**
     * Retrieve the Present_Value property of a Binary Value object.
     * If checkReliability() has been enabled, then the Reliability
     * property of the object will be retrieved first.  If the
     * Reliability property is anything other than
     * RELIABILITY_NO_FAULT_DETECTED, then the method will throw.
     * Reliability checking is disabled by default for performance
     * reasons.
     *
     * @param objInstance The Object Instance number to query
     * @return the boolean point value requested
     */
    virtual bool getBinaryValue(uint32_t objInstance);

    /**
     * Set the Present_Value property of a Binary Value object.  This
     * method will throw on an error.
     *
     * @param objInstance The Analog Value Object instance.
     * @param value The data value to write
     */
    virtual void setBinaryValue(uint32_t objInstance,
                                bool value);

    /**
     * Lookup (retrieve if necessary) the Inactive_Text and
     * Active_Text properties of a Binary Value object.  These text
     * properties are optional and can provide a string representing a
     * given state (true/false) that can be more informational than
     * just the boolean value the object represents.  This is useful
     * in applications that display this data to a user for example.
     * If this text is not present in the object (as it is not
     * required), then a string representation of the value will be
     * returned instead ("active" and "inactive").
     *
     * @param objInstance The Object Instance number of the object.
     * @param value The value you want to lookup the text
     * representation for.
     * @return The string representing the value.
     */
    virtual std::string lookupBinaryValueText(uint32_t objInstance, bool value);

    /**
     * Return a string representation of the Present_Value property of
     * a Binary Value object.  This method just calls getBinaryValue()
     * on the object, uses lookupBinaryValueText() to lookup the
     * corresponding text value, and returns the result.
     *
     * @param objInstance The Object Instance number of the object.
     * @return The string representing the value.
     */
    virtual std::string getBinaryValueText(uint32_t objInstance);

    /**
     * Retrieve the Present_Value property of a Multi State Value
     * object.  If checkReliability() has been enabled, then the
     * Reliability property of the object will be retrieved first.  If
     * the Reliability property is anything other than
     * RELIABILITY_NO_FAULT_DETECTED, then the method will throw.
     * Reliability checking is disabled by default for performance
     * reasons.
     *
     * @param objInstance The Object Instance number to query.
     * @return The Present_Value property of the object.
     */
    virtual unsigned int getMultiStateValue(uint32_t objInstance);

    /**
     * Lookup (retrieve if necessary) the State_Text strings
     * corresponding to the supplied value of a MultiStateValue
     * object.  State_Text is an optional property that can provide
     * strings representing a given state that can be more
     * informational than just the unsigned integer the object
     * represents.  This is useful in applications that display this
     * data to a user for example.  If this text is not present in the
     * object (as it is not required), then a string representation of
     * the integer value will be returned instead.
     *
     * @param objInstance The Object Instance number of the object.
     * @param value The value you want to lookup the text
     * representation for.
     * @return The string representing the value.
     */
    virtual std::string lookupMultiStateValueText(uint32_t objInstance,
                                                  unsigned int value);

    /**
     * Return a string representation of the Present_Value property of
     * a MultiStateValue object.  This method just calls
     * getMultiStateValue() on the object, uses
     * lookupMultiStateValueText() to lookup the corresponding
     * State_Text value, and returns the result.
     *
     * @param objInstance The Object Instance number of the object.
     * @return The string representing the value.
     */
    virtual std::string getMultiStateValueText(uint32_t objInstance);

    /**
     * Return the maximum legal value of a Multi State Value Object.
     * The value represents the highest value the Present_Value
     * porperty of the object will allow.
     *
     * @param objInstance The Object Instance number of the object.
     * @return The highest Present_Value the object supports.
     */
    virtual unsigned int getMultiStateValueMaxStates(uint32_t objInstance);

    /**
     * Set the Present_Value property of a Multi State Value object.
     * The value provided must not be 0, and must not exceed the
     * object's Number_Of_States property. Use
     * getMultiStateValueMaxStates() to determine the maximum value
     * the object supports. This method will throw on an error.
     *
     * @param objInstance The MultiStateValue Object instance.
     * @param value The data value to write.
     */
    virtual void setMultiStateValue(uint32_t objInstance,
                                    unsigned int value);

    /**
     * Enable or disable reliability checking.  When retrieving data,
     * the Present_Value property is returned.  There is also an
     * optional property called Reliability that can be checked to
     * ensure that the Present_Value property is currently valid.
     *
     * Enabling Reliability Checking has the data retrieval functions
     * check for a RELIABILITY_NO_FAULT_DETECTED value for the
     * Reliability property before querying the Present_Value
     * property.  If anything other than RELIABILITY_NO_FAULT_DETECTED
     * is set, then the method will throw.
     *
     * This checking is disabled by default since it will double the
     * number of queries needed to retrieve a given value.  In
     * addition, since it is an optional property, calling it for an
     * object that does not support it will also throw.  However, if
     * you need to ensure that the values returned are always
     * completely valid as determined by the device firmware, and the
     * objects you are querying support the reliability property, you
     * can enable this.
     *
     * @param enable true to check Reliability before returning a
     * value, false otherwise.
     */
    virtual void checkReliability(bool enable)
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
    virtual std::string getDeviceDescription();

    /**
     * Query the Device Object of the device and return it's Location
     * property.  This typically contains a string indication of a
     * customer specific value.  Use setLocation() to change.
     *
     * @return A string containing the Device Object's Location property.
     */
    virtual std::string getDeviceLocation();

    /**
     * Set the Device Object's Location property.  This must be a
     * string containing no more than 63 characters.  Not all devices
     * allow setting the location.
     *
     * @param location The new location to set, if supported.
     * @return true if the operation succeeded, otherwise false.
     */
    virtual bool setDeviceLocation(std::string location);

    /**
     * Query the Device Object of the device and return it's Name
     * property.  This should contain a unique string value.  Use
     * setName() to change.  Note, according to the spec, the Device
     * Object Name must be unique within a given BACnet network.
     *
     * @return A string containing the Object's Name property.
     */
    virtual std::string getDeviceName();

    /**
     * Set the Device Object's Name property.  This must be a string
     * containing at least one character and no more than 63
     * characters.  Note, according to the spec, the Device Object
     * Name must be unique within a given BACnet network.
     *
     * @param name The name to set.
     * @return true if the operation succeeded, false otherwise
     */
    virtual bool setDeviceName(std::string name);

    /**
     * This is a utility function that will return a string reporting
     * on the various types of errors that can occur in BACnet
     * operation.
     *
     * @return A string containing the last error message.
     */
    virtual std::string getAllErrorString();

    /**
     * Return an enumration of the last error type to occur.  The
     * value returned will be one of the BACNETMSTP::BACERR_TYPE_T
     * values.
     *
     * @return The last error type to occur, one of the
     * BACNETMSTP::BACERR_TYPE_T values.
     */
    virtual BACNETMSTP::BACERR_TYPE_T getErrorType();

    /**
     * In the event of a BACnet Reject error, return the error code.
     *
     * @return The Reject error code.
     */
    virtual uint8_t getRejectReason();

    /**
     * In the event of a BACnet Reject error, return the error string.
     *
     * @return The Reject error string.
     */
    virtual std::string getRejectString();

    /**
     * In the event of a BACnet Abort error, return the Abort reason code.
     *
     * @return The Abort reason code.
     */
    virtual uint8_t getAbortReason();

    /**
     * In the event of a BACnet Abort error, return the Abort string.
     *
     * @return The Abort error string.
     */
    virtual std::string getAbortString();

    /**
     * In the event of a general BACnet error, return the BACnet error class.
     *
     * @return One of the BACNET_ERROR_CLASS error class codes
     */
    virtual BACNET_ERROR_CLASS getErrorClass();

    /**
     * In the event of a general BACnet error, return the BACnet error code.
     *
     * @return One of the BACNET_ERROR_CODE error codes
     */
    virtual BACNET_ERROR_CODE getErrorCode();

    /**
     * In the event of a general BACnet error, return the BACnet error
     * string.
     *
     * @return A string representing the BACnet error class and code.
     */
    virtual std::string getErrorString();

    /**
     * In the event of a non-BACnet UPM error, return a string
     * describing the error.
     *
     * @return A string representing the UPM error.
     */
    virtual std::string getUPMErrorString();

  protected:
    // update our stored info for an MSV
    virtual void updateMultiStateValueInfo(uint32_t objInstance);
    // delete our stored info for an MSV
    virtual void deleteMultiStateValueInfo(uint32_t objInstance);

    // update our stored info for a BV
    virtual void updateBinaryValueInfo(uint32_t objInstance);
    // delete our stored info for a BV
    virtual void deleteBinaryValueInfo(uint32_t objInstance);

    // update our stored info for a BI
    virtual void updateBinaryInputInfo(uint32_t objInstance);
    // delete our stored info for a BI
    virtual void deleteBinaryInputInfo(uint32_t objInstance);

    // also enable mstp debugging in BACNETMSTP
    bool m_debugging;

    // whether or not to verify reliability before reading a value.
    bool m_checkReliability;

    // our target Device Object ID
    uint32_t m_targetDeviceObjectID;

    // a copy of the BACNETMSTP singleton instance pointer
    BACNETMSTP* m_instance;

    // are we initialized?
    bool m_initialized;

    // storage for Binary Input and Binary Value Data.  This will
    // generate SWIG warnings which can be ignored as we do not expose
    // this struct outside the class.
    typedef struct {
      std::string inactiveText;
      std::string activeText;
    } binaryData_t;

    typedef std::map<uint32_t, binaryData_t> binaryInfo_t;

    // storage for binary input/value information
    binaryInfo_t m_bvInfo;
    binaryInfo_t m_biInfo;

    // storage for multi-state data.  This will generate SWIG
    // warnings which can be ignored as we do not expose this struct
    // outside the class.
    typedef struct {
      unsigned int numStates;
      std::vector<std::string>stateList;
    } multiStateData_t;

    // our information storage for MSV's
    typedef std::map<uint32_t, multiStateData_t> multiStateInfo_t;

    multiStateInfo_t m_msvInfo;

    // Unit cache for AV
    typedef std::map<uint32_t, std::string> avCacheMap_t;
    avCacheMap_t m_avUnitCache;

    // Unit cache for AI
    typedef std::map<uint32_t, std::string> aiCacheMap_t;
    aiCacheMap_t m_aiUnitCache;

  private:
  };
}
