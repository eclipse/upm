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
#include <vector>

// we only support a BACnet RS-485 MS/TP datalink
#define BACDL_MSTP 1
#undef BACDL_ALL

// get a variety of bacnet-stack includes...
#include "bacdef.h"
#include "config.h"
#include "bactext.h"
#include "bacerror.h"
#include "iam.h"
#include "arf.h"
#include "tsm.h"
#include "address.h"
#include "npdu.h"
#include "apdu.h"
#include "device.h"
#include "datalink.h"
#include "whois.h"
#include "mstpdef.h"
#include "dlmstp.h"


namespace upm {

  /**
   * @brief BACNETMSTP base class
   * @defgroup bacnetmstp libupm-bacnetmstp
   * @ingroup uart
   */

  /**
   * @library bacnetmstp
   * @sensor bacnetmstp
   * @comname UPM API for BACNET MS/TP communications
   * @con uart
   * @web http://bacnet.sourceforge.net/
   * @brief UPM API for BACNETMSTP
   *
   * This is a singleton class that provides services to UPM BACnet
   * drivers (like E50HX) based on the bacnet-stack at
   * http://bacnet.sourceforge.net .  This class is implemented as a
   * singleton due to the fact that the bacnet-stack implementation
   * does not currently allow multiple simultaneous datalinks.  We are
   * using 0.8.3 of bacnet-stack.  In the future this restriction may
   * be lifted depending on bacnet-stack, but for now, you are
   * stuck with only a single BACnet MS/TP datalink.
   *
   * This driver is not intended to be used by end users.  It is
   * intended for use with other UPM drivers that require access to a
   * BACnet MS/TP (Master Slave/Token Passing) network over RS-485.
   *
   * For this reason, no examples are provided.  If you wish to
   * implement your own BACnet MS/TP driver, please look at the E50HX
   * driver to see how this class can be used.
   *
   * Currently, only readProperty and writeProperty BACnet requests
   * are supported.  In the future, any other BACnet requests could be
   * supported as well.  readProperty and writeProperty should provide
   * most of what you will need when communicating with BACnet
   * devices.  Since the source code is open, feel free to add other
   * services as you see fit :)
   *
   * In order to make requests over an MS/TP network, you must be a
   * BACnet master.  initMaster() is responsible for configuring your
   * underlying RS-485 network and starting a Master FSM (finite state
   * machine) thread that will be responsible for identifying other
   * Masters on the network and negotiating token passing.  Your
   * master can only transmit when it has the token.
   *
   * Fortunately, all of these messy details are handled for you by
   * this class, or the underlying bacnet-stack library this class
   * relies on.

   */

  class BACNETMSTP {
    // Constructor and destructor are protected

  public:

    // error types
    typedef enum {
      BACERR_TYPE_NONE                = 0,
      BACERR_TYPE_REJECT,
      BACERR_TYPE_ABORT,
      BACERR_TYPE_ERROR,
      BACERR_TYPE_UPM
    } BACERR_TYPE_T;

    // command types we currently support
    typedef enum {
      BACCMD_NONE                     = 0,
      BACCMD_READ_PROPERTY,
      BACCMD_WRITE_PROPERTY
    } BACCMD_TYPE_T;

    /**
     * Get our singleton instance, initializing it if neccessary.  All
     * requests to this class should be done through this instance
     * accessor.
     *
     * @return static pointer to our class instance
     */
    static BACNETMSTP* instance();

    /**
     * This function initializes the underlying BACNETMSTP Master
     * singleton in the event it has not already been initialized.  If
     * the BACNETMSTP Master singleton has already been initialized,
     * then this call will be ignored.  There can be only one.
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
     * Perform a BACnet readProperty transaction.  This function will
     * return when either the transaction has completed, or an error
     * has occurred.  It requests the value of a property, belonging
     * to a specific object instance on a specific device.
     *
     * @param targetDeviceInstanceID This is the Device Object
     * Instance ID of the device to send the request to.  This number
     * will be unique for every device on the network.  An address
     * lookup will be performed the first time a request is made to a
     * device using the WhoHas BACnet service.  The result will be
     * cached for further use.
     * @param objType This is the BACnet object type of the object you
     * wish to query.  It should be one of the BACNET_OBJECT_TYPE
     * values.
     * @param objInstance This is the instance number of the Object
     * you wish to access. It is an integer starting from 1.
     * @param objProperty This is the property of the Object and
     * instance you wish to access.  It should be one of the
     * BACNET_PROPERTY_ID values.
     * @param arrayIndex This specifies the index number of an array
     * property. The default is BACNET_ARRAY_ALL.
     * @return true if an error occurred, false otherwise.
     */
    bool readProperty(uint32_t targetDeviceInstanceID,
                      BACNET_OBJECT_TYPE objType,
                      uint32_t objInstance,
                      BACNET_PROPERTY_ID objProperty,
                      uint32_t arrayIndex=BACNET_ARRAY_ALL);

    /**
     * Perform a BACnet writeProperty transaction.  This function will
     * return when either the transaction has completed, or an error
     * has occurred.  It writes the supplied value to a property,
     * belonging to a specific object instance on a specific device.
     *
     * @param targetDeviceInstanceID This is the Device Object
     * Instance ID of the device to send the request to.  This number
     * will be unique for every device on the network.  An address
     * lookup will be performed the first time a request is made to a
     * device using the WhoHas BACnet service.  The result will be
     * cached for further use.
     * @param objType This is the BACnet object type of the object you
     * wish to query.  It should be one of the BACNET_OBJECT_TYPE
     * values.
     * @param objInstance This is the instance number of the Object
     * you wish to access. It is an integer starting from 1.
     * @param objProperty This is the property of the Object and
     * instance you wish to access.  It should be one of the
     * BACNET_PROPERTY_ID values.
     * @param propValue This is a pointer to a
     * BACNET_APPLICATION_DATA_VALUE structure containg the data value
     * to write to the property. Use the createData*() methods to
     * properly create these structures.
     * @param propPriority This specifies the priority of a
     * commandable property.  Leave it at the default unless you know
     * what you are doing.  In addition, there is conflicting
     * information in the bacnet-stack documentation as to whether
     * this is even supported.
     * @param arrayIndex This specifies the index number of an array
     * property. The default is BACNET_ARRAY_ALL.
     * @return true if an error occurred, false otherwise.
     */
    bool writeProperty(uint32_t targetDeviceInstanceID,
                       BACNET_OBJECT_TYPE objType,
                       uint32_t objInstance,
                       BACNET_PROPERTY_ID objProperty,
                       BACNET_APPLICATION_DATA_VALUE* propValue,
                       uint8_t propPriority=BACNET_NO_PRIORITY,
                       int32_t arrayIndex=BACNET_ARRAY_ALL);

    /**
     * After a successful readProperty request, this method can be used
     * to return a BACNET_APPLICATION_DATA_VALUE structure containing
     * the returned data.
     *
     * @param index into the list of returned data. 0 (first) is the
     * default.
     * @return a BACNET_APPLICATION_DATA_VALUE structure containing
     * the returned data.
     */
    BACNET_APPLICATION_DATA_VALUE getData(int index=0);

    /**
     * After a successful readProperty request, this method can be
     * used to return the number of data elements returned.  This will
     * usually be 1, unless reading an array.
     *
     * @return The number of data elements received.
     */
    int getDataNumElements();

    /**
     * After a successful readProperty request, this method can be
     * used to return the BACnet data type of the returned data.  It
     * will be one of the BACNET_APPLICATION_TAG_* values.
     *
     * @param index into the list of returned data. 0 (first) is the
     * default.
     * @return A BACNET_APPLICATION_TAG_* value
     */
    uint8_t getDataType(int index=0);

    /**
     * After a successful readProperty request, this method can be
     * used to return the BACnet dataype of the returned data as a
     * Real.  If the data type (getDataType()) is not a
     * BACNET_APPLICATION_TAG_REAL, and the value returned cannot be
     * safely converted, an exception is thrown.
     *
     * @param index into the list of returned data. 0 (first) is the
     * default.
     * @return A floating point value representing the returned data
     */
    float getDataTypeReal(int index=0);

    /**
     * After a successful readProperty request, this method can be
     * used to return the BACnet dataype of the returned data as a
     * Boolean.  If the data type (getDataType()) is not a
     * BACNET_APPLICATION_TAG_BOOLEAN, and the value returned cannot
     * be safely converted, an exception is thrown.
     *
     * @param index into the list of returned data. 0 (first) is the
     * default.
     * @return A boolean value representing the returned data
     */
    bool getDataTypeBoolean(int index=0);

    /**
     * After a successful readProperty request, this method can be
     * used to return the BACnet dataype of the returned data as a
     * unsigned int.  If the data type (getDataType()) is not a
     * BACNET_APPLICATION_TAG_UNSIGNED_INT, and the value returned
     * cannot be safely converted, an exception is thrown.
     *
     * @param index into the list of returned data. 0 (first) is the
     * default.
     * @return An unsigned int value representing the returned data
     */
    unsigned int getDataTypeUnsignedInt(int index=0);

    /**
     * After a successful readProperty request, this method can be
     * used to return the BACnet dataype of the returned data as a
     * signed int.  If the data type (getDataType()) is not a
     * BACNET_APPLICATION_TAG_SIGNED_INT, and the value returned
     * cannot be safely converted, an exception is thrown.
     *
     * @param index into the list of returned data. 0 (first) is the
     * default.
     * @return A signed int value representing the returned data
     */
    int getDataTypeSignedInt(int index=0);

    /**
     * After a successful readProperty request, this method can be
     * used to return the BACnet dataype of the returned data as a
     * string.  Most of the data types except Enum can be converted to
     * a string.  If the data type (getDataType()) is not a
     * BACNET_APPLICATION_TAG_CHARACTER_STRING, and the value returned
     * cannot be safely converted, an exception is thrown.
     *
     * @param index into the list of returned data. 0 (first) is the
     * default.
     * @return A string value representing the returned data
     */
    std::string getDataTypeString(int index=0);

    /**
     * After a successful readProperty request, this method can be
     * used to return the BACnet dataype of the returned data as an
     * enumeration.  If the data type (getDataType()) is not a
     * BACNET_APPLICATION_TAG_ENUMERATED an exception is thrown.
     *
     * @param index into the list of returned data. 0 (first) is the
     * default.
     * @return An unsigned int representing a BACnet enumerant
     */
    unsigned int getDataTypeEnum(int index=0);

#if defined(BACAPP_DOUBLE)
    /**
     * After a successful readProperty request, this method can be
     * used to return the BACnet dataype of the returned data as a
     * double.  If the data type (getDataType()) is not a
     * BACNET_APPLICATION_TAG_DOUBLE, and the value returned cannot be
     * safely converted, an exception is thrown.
     *
     * @param index into the list of returned data. 0 (first) is the
     * default.
     * @return A double floating point value representing the returned data
     */
    double getDataTypeDouble(int index=0);
#endif // BACAPP_DOUBLE

    /**
     * This method is used to create and return an initialized
     * BACNET_APPLICATION_DATA_VALUE containing a real (floating point
     * value).  A pointer to this returned structure can then be used
     * with writeProperty().
     *
     * @param value The floating point value to initialize the structure to.
     * @return An initialized structure containing the value
     */
    BACNET_APPLICATION_DATA_VALUE createDataReal(float Real);

    /**
     * This method is used to create and return an initialized
     * BACNET_APPLICATION_DATA_VALUE containing a boolean.  A pointer
     * to this returned structure can then be used with
     * writeProperty().
     *
     * @param value The boolean value to initialize the structure to.
     * @return An initialized structure containing the value
     */
    BACNET_APPLICATION_DATA_VALUE createDataBool(bool value);

    /**
     * This method is used to create and return an initialized
     * BACNET_APPLICATION_DATA_VALUE containing a signed integer.  A
     * pointer to this returned structure can then be used with
     * writeProperty().
     *
     * @param value The signed integer value to initialize the structure to.
     * @return An initialized structure containing the value
     */
    BACNET_APPLICATION_DATA_VALUE createDataSignedInt(int value);

    /**
     * This method is used to create and return an initialized
     * BACNET_APPLICATION_DATA_VALUE containing a unsigned integer.  A
     * pointer to this returned structure can then be used with
     * writeProperty().
     *
     * @param value The unsigned integer value to initialize the
     * structure to.
     * @return An initialized structure containing the value
     */
    BACNET_APPLICATION_DATA_VALUE createDataUnsignedInt(unsigned int value);

    /**
     * This method is used to create and return an initialized
     * BACNET_APPLICATION_DATA_VALUE containing a character string.  A
     * pointer to this returned structure can then be used with
     * writeProperty().  Strings are typically limited to 63 characters.
     *
     * @param value The character string value to initialize the
     * structure to.
     * @return An initialized structure containing the value
     */
    BACNET_APPLICATION_DATA_VALUE createDataString(std::string value);

    /**
     * This method is used to create and return an initialized
     * BACNET_APPLICATION_DATA_VALUE containing an enumeration.  A
     * pointer to this returned structure can then be used with
     * writeProperty().
     *
     * @param value The BACnet enumeration to initialize the
     * structure to.
     * @return An initialized structure containing the value
     */
    BACNET_APPLICATION_DATA_VALUE createDataEnum(uint32_t value);

    /**
     * Return an enumration of the last error type to occur.  The
     * value returned will be one of the BACERR_TYPE_T values.
     *
     * @return The last error type to occur, one of the BACERR_TYPE_T
     * values.
     */
    BACERR_TYPE_T getErrorType()
    {
      return m_errorType;
    };

    /**
     * In the event of a BACnet Reject error, return the error code.
     *
     * @return The Reject error code.
     */
    uint8_t getRejectReason()
    {
      return m_rejectReason;
    };

    /**
     * In the event of a BACnet Reject error, return the error string.
     *
     * @return The Reject error string.
     */
    std::string getRejectString()
    {
      return m_rejectString;
    };

    /**
     * In the event of a BACnet Abort error, return the Abort reason code.
     *
     * @return The Abort reason code.
     */
    uint8_t getAbortReason()
    {
      return m_abortReason;
    };

    /**
     * In the event of a BACnet Abort error, return the Abort string.
     *
     * @return The Abort error string.
     */
    std::string getAbortString()
    {
      return m_abortString;
    };

    /**
     * In the event of a general BACnet error, return the BACnet error class.
     *
     * @return One of the BACNET_ERROR_CLASS error class codes
     */
    BACNET_ERROR_CLASS getErrorClass()
    {
      return m_errorClass;
    };

    /**
     * In the event of a general BACnet error, return the BACnet error code.
     *
     * @return One of the BACNET_ERROR_CODE error codes
     */
    BACNET_ERROR_CODE getErrorCode()
    {
      return m_errorCode;
    };

    /**
     * In the event of a general BACnet error, return the BACnet error
     * string.
     *
     * @return A string representing the BACnet error class and code.
     */
    std::string getErrorString()
    {
      return m_errorString;
    };

    /**
     * In the event of a non-BACnet UPM error, return a string
     * describing the error.
     *
     * @return A string representing the UPM error.
     */
    std::string getUPMErrorString()
    {
      return m_upmErrorString;
    };

    /**
     * Check to see if initMaster) has already been called, and out
     * master is initialized.
     *
     * @return true if the master is initialized, false otherwise
     */
    bool isInitialized()
    {
      return m_initialized;
    };

    /**
     * Return the port that was specified to initMaster().
     *
     * @return The port specified to initMaster().
     */
    std::string getPort()
    {
      return m_port;
    };

    /**
     * Return the Object Device Instance ID for our Master was
     * specified to initMaster().
     *
     * @return The Object Device Instance ID for our Master.
     */
    uint32_t getDeviceInstanceID()
    {
      return m_deviceInstanceID;
    };

    /**
     * Return the maxInfoFrames parameter that was specified to initMaster().
     *
     * @return The maxInfoFrames parameter specified to initMaster().
     */
    int getMaxInfoFrames()
    {
      return m_maxInfoFrames;
    };

    /**
     * Return the maxMaster parameter that was specified to initMaster().
     *
     * @return The maxMaster parameter specified to initMaster().
     */
    int getMaxMaster()
    {
      return m_maxMaster;
    };

    /**
     * Return the baud rate parameter that was specified to initMaster().
     *
     * @return The baud rate parameter specified to initMaster().
     */
    int getBaudRate()
    {
      return m_baudRate;
    };

    /**
     * Return the MAC address parameter that was specified to initMaster().
     *
     * @return The MAC address parameter specified to initMaster().
     */
    int getMACAddress()
    {
      return m_macAddr;
    };

    /**
     * Enable or disable debugging output.
     *
     * @param enable true to enable debugging, false otherwise
     */
    void setDebug(bool enable);

  protected:
    /**
     * BACNETMSTP constructor
     */
    BACNETMSTP();

    /**
     * BACNETMSTP Destructor
     */
    ~BACNETMSTP();

    // clear/reset error states
    void clearErrors();

    // error handler
    static void handlerError(BACNET_ADDRESS * src,
                             uint8_t invoke_id,
                             BACNET_ERROR_CLASS error_class,
                             BACNET_ERROR_CODE error_code);

    // abort handler
    static void handlerAbort(BACNET_ADDRESS * src,
                             uint8_t invoke_id,
                             uint8_t abort_reason,
                             bool server);

    // reject handler
    static void handlerReject(BACNET_ADDRESS * src,
                              uint8_t invoke_id,
                              uint8_t reject_reason);


    // our handler for dealing with return data from a ReadProperty call
    static void handlerReadPropertyAck(uint8_t* service_request,
                                       uint16_t service_len,
                                       BACNET_ADDRESS* src,
                                       BACNET_CONFIRMED_SERVICE_ACK_DATA* service_data);

    // our handler for writeProp acks
    static void handlerWritePropertyAck(BACNET_ADDRESS* src,
                                        uint8_t invoke_id);

    // initialize our service handlers
    void initServiceHandlers();

    // utility function
    std::string string2HexString(std::string input);

    // responsible for dispatching a request to the BACnet network
    bool dispatchRequest();

  private:
    // prevent copying and assignment
    BACNETMSTP(BACNETMSTP const &) = delete;
    BACNETMSTP& operator=(BACNETMSTP const&) = delete;

    // our class instance
    static BACNETMSTP* m_instance;

    // has the class been created yet?
    bool m_initialized;

    // Some items we can set for our master
    std::string m_port;
    int m_maxInfoFrames;
    int m_maxMaster;
    int m_baudRate;
    int m_macAddr;

    // the unique Instance Number of our Master Device Object
    uint32_t m_deviceInstanceID;

    // adpu timeout in milliseconds
    uint16_t m_adpuTimeoutMS;

    // buffer used for receiving data
    uint8_t m_rxBuffer[MAX_MPDU];

    // our error classfication
    BACERR_TYPE_T m_errorType;

    // BACnet reject info
    uint8_t m_rejectReason;
    std::string m_rejectString;

    // BACnet abort info
    uint8_t m_abortReason;
    std::string m_abortString;

    // BACnet error info
    BACNET_ERROR_CLASS m_errorClass;
    BACNET_ERROR_CODE  m_errorCode;
    std::string m_errorString;

    // generic UPM related errors - we just set the error text to
    // something informative.
    std::string m_upmErrorString;

    // our returned data from readProperty()
    std::vector<BACNET_APPLICATION_DATA_VALUE> m_returnedValue;

    // current bound target address of dispatched service request
    // (read/write prop, etc)
    BACNET_ADDRESS m_targetAddress;

    // current invokeID (for transaction handling)
    uint8_t m_invokeID;

    // error detected flag
    bool m_errorDetected;

    // Commands - we create a struct to hold the arguments for each
    // command type we support.  Then, we create a command struct
    // which contains the command type and a union containing the
    // relevant arguments.  This is used by dispatchRequest() to issue
    // the correct request.

    // these may generate SWIG warnings, but they can be ignored as we
    // do not expose these outside the class
    typedef struct {
      uint32_t targetDeviceInstanceID;
      BACNET_OBJECT_TYPE objType;
      uint32_t objInstance;
      BACNET_PROPERTY_ID objProperty;
      uint32_t arrayIndex;
    } READ_PROPERTY_ARGS_T;

    typedef struct {
      uint32_t targetDeviceInstanceID;
      BACNET_OBJECT_TYPE objType;
      uint32_t objInstance;
      BACNET_PROPERTY_ID objProperty;
      BACNET_APPLICATION_DATA_VALUE* propValue;
      uint8_t propPriority;
      int32_t arrayIndex;
    } WRITE_PROPERTY_ARGS_T;

    struct {
      BACCMD_TYPE_T cmd;

      union {
        READ_PROPERTY_ARGS_T readPropArgs;
        WRITE_PROPERTY_ARGS_T writePropArgs;
      };
    } m_command;

    bool m_debugging;
  };
}
